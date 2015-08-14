#ifndef NDSFACTORYIMPL_CPP
#define NDSFACTORYIMPL_CPP

#include <dbStaticLib.h>
#include <vector>
#include <list>
#include <string>

#include <iocsh.h>

#include "registryCommon.h"

namespace nds
{

/**
 * @brief Takes care of registering everything with EPICS
 *
 */
class FactoryImpl
{
public:
    static FactoryImpl& getInstance();

    FactoryImpl();

    void registrationCommand(const std::string& registrationCommandName);

    void registerRecordTypes(DBBASE* pDatabase);

private:
    std::string m_commandName;   // Keep the name of the registered command
    iocshFuncDef m_commandDefinition;

    std::vector<recordTypeLocation> m_recordTypeFunctions;
    std::list<std::string> m_recordTypeNames;
    std::vector<const char*> m_recordTypeNamesCstr;


};


class DBEntry
{
public:
    DBEntry(DBBASE* pDatabase);
    ~DBEntry();

    DBENTRY* m_pDBEntry;
};

class DynamicModule
{
public:
    DynamicModule();
    ~DynamicModule();
    void* getAddress(const std::string& name);

private:
    void* m_moduleHandle;
};

}
#endif // NDSFACTORYIMPL_CPP
