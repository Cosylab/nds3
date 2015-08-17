#include <base/include/epicsStdlib.h>
#include <base/include/iocshRegisterCommon.h>
#include <base/include/registryCommon.h>
#include <base/include/dbStaticPvt.h>

#include "ndsfactoryimpl.h"
#include "../include/nds3/ndsbase.h"

#include <iostream>
#include <link.h>
#include <elf.h>
#include <dlfcn.h>

#include <list>


namespace nds
{

extern "C" {

static void rrddCallFunc(const iocshArgBuf *)
{
    FactoryImpl::getInstance().registerRecordTypes(*iocshPpdbbase);
}

}

FactoryImpl& FactoryImpl::getInstance()
{
    static FactoryImpl factory;

    return factory;
}

FactoryImpl::FactoryImpl()
{
}

int retrieveSymbolNames(struct dl_phdr_info* info, size_t info_size, void* symbol_names_vector)
{

    /* ElfW is a macro that creates proper typenames for the used system architecture
     * (e.g. on a 32 bit system, ElfW(Dyn*) becomes "Elf32_Dyn*") */
    ElfW(Dyn*) dyn;
    ElfW(Sym*) sym;
    ElfW(Word*) hash;

    char* strtab;
    char* sym_name;
    ElfW(Word) sym_cnt;

    /* the void pointer (3rd argument) should be a pointer to a vector<string>
     * in this example -> cast it to make it usable */
    std::list<std::string>* symbol_names = reinterpret_cast<std::list<std::string>*>(symbol_names_vector);

    /* Iterate over all headers of the current shared lib
     * (first call is for the executable itself) */
    for (size_t header_index = 0; header_index < info->dlpi_phnum; header_index++)
    {

        /* Further processing is only needed if the dynamic section is reached */
        if (info->dlpi_phdr[header_index].p_type == PT_DYNAMIC)
        {

            /* Get a pointer to the first entry of the dynamic section.
             * It's address is the shared lib's address + the virtual address */
            dyn = (ElfW(Dyn)*)(info->dlpi_addr +  info->dlpi_phdr[header_index].p_vaddr);

            sym_cnt = 0;

            /* Iterate over all entries of the dynamic section until the
             * end of the symbol table is reached. This is indicated by
             * an entry with d_tag == DT_NULL.
             *
             * Only the following entries need to be processed to find the
             * symbol names:
             *  - DT_HASH   -> second word of the hash is the number of symbols
             *  - DT_STRTAB -> pointer to the beginning of a string table that
             *                 contains the symbol names
             *  - DT_SYMTAB -> pointer to the beginning of the symbols table
             */
            while(dyn->d_tag != DT_NULL)
            {
                if (dyn->d_tag == DT_HASH)
                {
                    /* Get a pointer to the hash */
                    hash = (ElfW(Word*))dyn->d_un.d_ptr;

                    /* The 2nd word is the number of symbols */
                    sym_cnt = hash[1];

                }
                else if (dyn->d_tag == DT_STRTAB)
                {
                    /* Get the pointer to the string table */
                    strtab = (char*)dyn->d_un.d_ptr;
                }
                else if (dyn->d_tag == DT_SYMTAB)
                {
                    /* Get the pointer to the first entry of the symbol table */
                    sym = (ElfW(Sym*))dyn->d_un.d_ptr;


                }

                /* move pointer to the next entry */
                dyn++;
            }
            /* Iterate over the symbol table */
            for (ElfW(Word) sym_index = 0; sym_index < sym_cnt; sym_index++)
            {
                /* get the name of the i-th symbol.
                 * This is located at the address of st_name
                 * relative to the beginning of the string table. */
                sym_name = &strtab[sym[sym_index].st_name];

                std::string name(sym_name);
                if(name.find("pvar_") == 0)
                {
                    symbol_names->push_back(std::string(sym_name));
                }
            }

        }
    }

    /* Returning something != 0 stops further iterations,
     * since only the first entry, which is the executable itself, is needed
     * 1 is returned after processing the first entry.
     *
     * If the symbols of all loaded dynamic libs shall be found,
     * the return value has to be changed to 0.
     */
    return 1;

}

void fillSymbols(ElfW(Dyn*) dyn, ElfW(Addr) pBase, std::list<std::string>* pList)
{
    //Dl_info info;
    //Elf64_Sym* extraInfo;
    //dladdr1((void*)pBase, &info, (void**)&extraInfo, RTLD_DL_SYMENT);
    ElfW(Sym*) sym;
    ElfW(Word*) hash;

    char* strtab;
    char* sym_name;
    ElfW(Word) sym_cnt;
    sym_cnt = 0;

    /* Iterate over all entries of the dynamic section until the
     * end of the symbol table is reached. This is indicated by
     * an entry with d_tag == DT_NULL.
     *
     * Only the following entries need to be processed to find the
     * symbol names:
     *  - DT_HASH   -> second word of the hash is the number of symbols
     *  - DT_STRTAB -> pointer to the beginning of a string table that
     *                 contains the symbol names
     *  - DT_SYMTAB -> pointer to the beginning of the symbols table
     */
    while(dyn->d_tag != DT_NULL)
    {
        if (dyn->d_tag == DT_HASH)
        {
            /* Get a pointer to the hash */
            hash = (ElfW(Word*))(dyn->d_un.d_ptr + pBase);

            /* The 2nd word is the number of symbols */
            sym_cnt = hash[1];

        }
        else if(dyn->d_tag == DT_GNU_HASH)
        {
            /* Get a pointer to the hash */
            hash = (ElfW(Word*))(dyn->d_un.d_ptr + pBase);

            /* The 2nd word is the number of symbols */
            sym_cnt = hash[1];

        }
        else if (dyn->d_tag == DT_STRTAB)
        {
            /* Get the pointer to the string table */
            strtab = (char*)(dyn->d_un.d_ptr + pBase);
        }
        else if (dyn->d_tag == DT_SYMTAB)
        {
            /* Get the pointer to the first entry of the symbol table */
            sym = (ElfW(Sym*))(dyn->d_un.d_ptr + pBase);


        }

        /* move pointer to the next entry */
        dyn++;
    }
    /* Iterate over the symbol table */
    for (ElfW(Word) sym_index = 0; sym_index < sym_cnt; sym_index++)
    {
        /* get the name of the i-th symbol.
         * This is located at the address of st_name
         * relative to the beginning of the string table. */
        sym_name = &strtab[sym[sym_index].st_name];

        std::string name(sym_name);
        if(name.find("pvar_") == 0)
        {
            pList->push_back(std::string(sym_name));
        }
    }

}

void FactoryImpl::registrationCommand(const std::string& registrationCommandName)
{
    iocshRegisterCommon();

    static const iocshArg commandArgument = {"pdbbase", iocshArgPdbbase};
    static const iocshArg* commandArguments[] = {&commandArgument};

    m_commandName = registrationCommandName;
    m_commandDefinition.arg = commandArguments;
    m_commandDefinition.nargs = 1;
    m_commandDefinition.name = m_commandName.c_str();
    iocshRegister(&m_commandDefinition, rrddCallFunc);

}


void FactoryImpl::registerRecordTypes(DBBASE* pDatabase)
{
    DBEntry dbEntry(pDatabase);

    DynamicModule thisModule;

    std::list<std::string> symbolNames;

    //dl_iterate_phdr(retrieveSymbolNames, &symbolNames);


    ::link_map* pMap = 0;
    for(dlinfo(thisModule.m_moduleHandle, RTLD_DI_LINKMAP, &pMap); pMap != 0; pMap = pMap->l_next)
    {
        std::string libName(pMap->l_name);
        if(libName.find("/lib64") == 0)
        {
            continue;
        }
        std::cout << pMap->l_name;
        fillSymbols(pMap->l_ld, pMap->l_addr, &symbolNames);

    }


    for(long scanRecords(dbFirstField(dbEntry.m_pDBEntry, 1)); scanRecords == 0; scanRecords = dbNextField(dbEntry.m_pDBEntry, 1))
    {
        std::cout << dbGetFieldName(dbEntry.m_pDBEntry) << "\n";
    }

    for(long scanTypes(dbFirstRecordType(dbEntry.m_pDBEntry)); scanTypes == 0; scanTypes = dbNextRecordType(dbEntry.m_pDBEntry))
    {
        std::string recordName(dbGetRecordTypeName(dbEntry.m_pDBEntry));

        void* resetFunction = thisModule.getAddress(recordName + "RSET");
        void* sizeOffset = thisModule.getAddress("pvar_func_" + recordName + "RecordSizeOffset");

        m_recordTypeFunctions.emplace_back(recordTypeLocation());
        m_recordTypeFunctions.back().prset = (rset*)resetFunction;
        m_recordTypeFunctions.back().sizeOffset = *((computeSizeOffset*)sizeOffset);

        m_recordTypeNames.push_back(recordName);
        m_recordTypeNamesCstr.emplace_back(recordName.c_str());

        for(int scanFields = dbFirstField(dbEntry.m_pDBEntry, 0); scanFields == 0; scanFields = dbNextField(dbEntry.m_pDBEntry, 0))
        {
            if(dbGetFieldType(dbEntry.m_pDBEntry) == DCT_MENUFORM)
            {
                std::cout << " -----" << dbGetFieldName(dbEntry.m_pDBEntry) << "\n";
            }
        }

        for(int scanLinks= 0; scanLinks < dbGetNLinks(dbEntry.m_pDBEntry); ++scanLinks)
        {
            dbGetLinkField(dbEntry.m_pDBEntry, scanLinks);
            std::string linkName(dbGetFieldName(dbEntry.m_pDBEntry));
            //std::cout << dbGetFieldName(dbEntry.m_pDBEntry);
        }
        for(long scanRecords(dbFirstInfo(dbEntry.m_pDBEntry)); scanRecords == 0; scanRecords = dbNextInfo(dbEntry.m_pDBEntry))
        {
            std::cout << dbGetInfoName(dbEntry.m_pDBEntry) << "\n";
        }
        //dbFirs

    }

    ::registerRecordTypes(pDatabase, m_recordTypeNames.size(), m_recordTypeNamesCstr.data(), m_recordTypeFunctions.data());

}

DBEntry::DBEntry(DBBASE* pDatabase)
{
    m_pDBEntry = dbAllocEntry(pDatabase);
}

DBEntry::~DBEntry()
{
    dbFreeEntry(m_pDBEntry);
}

DynamicModule::DynamicModule(): m_moduleHandle(dlopen(0, RTLD_NOW | RTLD_GLOBAL))
{
}

DynamicModule::~DynamicModule()
{
    if(m_moduleHandle != 0)
    {
        dlclose(m_moduleHandle);
    }
}

void* DynamicModule::getAddress(const std::string& name)
{
    return dlsym(m_moduleHandle, name.c_str());
}





}
