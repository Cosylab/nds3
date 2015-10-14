#ifndef  SYMBOLS_H
#ifndef _GNU_SOURCE
#error You must define _GNU_SOURCE!
#endif
#define  SYMBOLS_H
#include <stdlib.h>

#include <map>
#include <string>

typedef enum {
    UNKNOWN_BIND  = 0,
    LOCAL_SYMBOL  = 1,
    GLOBAL_SYMBOL = 2,
    WEAK_SYMBOL   = 3
} symbol_bind;

typedef enum {
    UNKNOWN_TYPE = 0,
    FUNC_SYMBOL   = 4,
    OBJECT_SYMBOL = 5,
    COMMON_SYMBOL = 6,
    THREAD_SYMBOL = 7
} symbol_type;

struct symbolProperties
{
    std::string m_libraryPath;
    std::string m_libraryName;
    symbol_bind m_bind;
    symbol_type m_type;
    const void* m_pAddress;
};

typedef std::multimap<std::string, symbolProperties> symbolsList_t;

symbolsList_t getSymbols();

#endif /* SYMBOLS_H */
