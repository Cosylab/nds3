#define _GNU_SOURCE
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <fnmatch.h>
#include <dlfcn.h>
#include <errno.h>
#include "/usr/include/link.h"


#include "scansymbols.h"

#define UINTS_PER_WORD (__WORDSIZE / (CHAR_BIT * sizeof (unsigned int)))

static ElfW(Word) gnu_hashtab_symbol_count(const unsigned int *const table)
{
    const unsigned int *const bucket = table + 4 + table[2] * (unsigned int)(UINTS_PER_WORD);
    unsigned int              b = table[0];
    unsigned int              max = 0U;

    while (b-->0U)
        if (bucket[b] > max)
            max = bucket[b];

    return (ElfW(Word))max;
}

static symbol_bind elf_symbol_binding(const unsigned char st_info)
{
#if __WORDSIZE == 32
    switch (ELF32_ST_BIND(st_info)) {
#elif __WORDSIZE == 64
    switch (ELF64_ST_BIND(st_info)) {
#else
    switch (ELF_ST_BIND(st_info)) {
#endif
    case STB_LOCAL:  return LOCAL_SYMBOL;
    case STB_GLOBAL: return GLOBAL_SYMBOL;
    case STB_WEAK:   return WEAK_SYMBOL;
    default:         return UNKNOWN_BIND;
    }
}

static symbol_type elf_symbol_type(const unsigned char st_info)
{
#if __WORDSIZE == 32
    switch (ELF32_ST_TYPE(st_info)) {
#elif __WORDSIZE == 64
    switch (ELF64_ST_TYPE(st_info)) {
#else
    switch (ELF_ST_TYPE(st_info)) {
#endif
    case STT_OBJECT: return OBJECT_SYMBOL;
    case STT_FUNC:   return FUNC_SYMBOL;
    case STT_COMMON: return COMMON_SYMBOL;
    case STT_TLS:    return THREAD_SYMBOL;
    default:         return UNKNOWN_TYPE;
    }
}

static void *dynamic_pointer(const ElfW(Addr) addr,
                             const ElfW(Addr) base, const ElfW(Phdr) *const header, const ElfW(Half) headers)
{
    if (addr) {
        ElfW(Half) h;

        for (h = 0; h < headers; h++)
            if (header[h].p_type == PT_LOAD)
                if (addr >= base + header[h].p_vaddr &&
                    addr <  base + header[h].p_vaddr + header[h].p_memsz)
                    return (void *)addr;
    }

    return NULL;
}

/*
 * Called by dl_iterate_phdr while scanning the loaded modules.
 * Used to find the functions that have to be registered with
 *  EPICS
 *
 *************************************************************/
int iterate_phdr(struct dl_phdr_info *info, size_t size, void *dataref)
{
    const ElfW(Addr)                 base = info->dlpi_addr;
    const ElfW(Phdr) *const          header = info->dlpi_phdr;
    const ElfW(Half)                 headers = info->dlpi_phnum;
    const char                      *libpath, *libname;
    ElfW(Half)                       h;

    symbolsList_t* pList = (symbolsList_t*) dataref;

    if (info->dlpi_name && info->dlpi_name[0])
        libpath = info->dlpi_name;
    else
        libpath = "";

    libname = strrchr(libpath, '/');
    if (libname && libname[0] == '/' && libname[1])
        libname++;
    else
        libname = libpath;

    for (h = 0; h < headers; h++)
        if (header[h].p_type == PT_DYNAMIC) {
            const ElfW(Dyn)  *entry = (const ElfW(Dyn) *)(base + header[h].p_vaddr);
            const ElfW(Word) *hashtab;
            const ElfW(Sym)  *symtab = NULL;
            const char       *strtab = NULL;
            ElfW(Word)        symbol_count = 0;

            for (; entry->d_tag != DT_NULL; entry++)
                switch (entry->d_tag) {
                case DT_HASH:
                    hashtab = (ElfW(Word) *) dynamic_pointer(entry->d_un.d_ptr, base, header, (ElfW(Half)) headers);
                    if (hashtab)
                        symbol_count = hashtab[1];
                    break;
                case DT_GNU_HASH:
                    hashtab = (ElfW(Word) *) dynamic_pointer(entry->d_un.d_ptr, base, header, (ElfW(Half))headers);
                    if (hashtab) {
                        ElfW(Word) count = gnu_hashtab_symbol_count(hashtab);
                        if (count > symbol_count)
                            symbol_count = count;
                    }
                    break;
                case DT_STRTAB:
                    strtab = (char*)dynamic_pointer(entry->d_un.d_ptr, base, header, (ElfW(Half))headers);
                    break;
                case DT_SYMTAB:
                    symtab = (ElfW(Sym)  *)dynamic_pointer(entry->d_un.d_ptr, base, header, (ElfW(Half))headers);
                    break;
                }

            if (symtab && strtab && symbol_count > 0) {
                ElfW(Word)  s;

                for (s = 0; s < symbol_count; s++) {
                    const char *name;
                    void *const ptr = dynamic_pointer(base + symtab[s].st_value, base, header, (ElfW(Half))headers);
                    symbol_bind bind;
                    symbol_type type;
                    int         result;

                    if (!ptr)
                        continue;

                    type = elf_symbol_type(symtab[s].st_info);
                    bind = elf_symbol_binding(symtab[s].st_info);
                    if (symtab[s].st_name)
                        name = strtab + symtab[s].st_name;
                    else
                        name = "";


                    std::string symbolName(name);
                    (*pList)[symbolName].m_bind = bind;
                    (*pList)[symbolName].m_type = type;
                    (*pList)[symbolName].m_pAddress = ptr;

                    if (result)
                        return result;
                }
            }
        }

    return 0;
}

symbolsList_t getSymbols()
{
    symbolsList_t list;
    dl_iterate_phdr(iterate_phdr, &list);
    return list;
}
