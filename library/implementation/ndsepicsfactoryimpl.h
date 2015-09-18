#ifdef NDS3_EPICS

#ifndef NDSFACTORYIMPL_CPP
#define NDSFACTORYIMPL_CPP

#include "../include/nds3/definitions.h"
#include "ndsfactorybaseimpl.h"
#include <dbStaticLib.h>
#include <vector>
#include <list>
#include <string>
#include <set>

#include <iocsh.h>

#include <registryCommon.h>

namespace nds
{

/**
 * @brief Takes care of registering everything with EPICS
 *
 */
class EpicsFactoryImpl: public FactoryBaseImpl
{
public:
    static EpicsFactoryImpl& getInstance();

    EpicsFactoryImpl();

    static void createNdsDevice(const iocshArgBuf * arguments);

    virtual InterfaceBaseImpl* getNewInterface(const std::string& fullName);

    virtual void run(int argc,char *argv[]);

private:
    void registerRecordTypes(DBBASE* pDatabase);

    void addToSet(const std::string symbolName, std::set<std::string>* pSet, const std::string& prefix, const std::string& postfix);

    std::string compareString(const std::string& string, const std::string& prefix, const std::string& postfix);

    std::string m_commandName;   // Keep the name of the registered command
    iocshFuncDef m_commandDefinition;

    std::vector<recordTypeLocation> m_recordTypeFunctions;
    std::list<std::string> m_recordTypeNames;
    std::vector<const char*> m_recordTypeNamesCstr;

    std::vector<dset*> m_deviceFunctions;
    std::list<std::string> m_deviceNames;
    std::vector<const char*> m_deviceNamesCstr;

    std::vector<drvet*> m_driverFunctions;
    std::list<std::string> m_driverNames;
    std::vector<const char*> m_driverNamesCstr;

    std::vector<iocshVarDef> m_variableFunctions;
    std::list<std::string> m_variableNames;
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


public:
    void* m_moduleHandle;
};

}
#endif // NDSFACTORYIMPL_CPP

#endif // NDS3_EPICS
