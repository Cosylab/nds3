#ifdef NDS3_EPICS

#ifndef NDSFACTORYIMPL_CPP
#define NDSFACTORYIMPL_CPP

#include "../include/nds3/definitions.h"
#include "factoryBaseImpl.h"
#include "logStreamGetterImpl.h"
#include <dbStaticLib.h>
#include <vector>
#include <list>
#include <string>
#include <set>
#include <sstream>

#include <iocsh.h>

#include <registryCommon.h>

namespace nds
{

/**
 * @brief Takes care of registering everything with EPICS
 *
 */
class EpicsFactoryImpl: public FactoryBaseImpl, public LogStreamGetterImpl
{
    friend class EpicsLogStreamBufferImpl;
public:
    static EpicsFactoryImpl& getInstance();

    EpicsFactoryImpl();

    static void createNdsDevice(const iocshArgBuf * arguments);

    static void ndsUserCommand(const iocshArgBuf * arguments);

    virtual std::thread createThread(const std::string &name, threadFunction_t function);

    virtual InterfaceBaseImpl* getNewInterface(const std::string& fullName);

    virtual void run(int argc,char *argv[]);

    virtual std::ostream* getLogStream(const BaseImpl& baseImpl, const logLevel_t logLevel);

    virtual LogStreamGetterImpl* getLogStreamGetter();

    virtual void registerCommand(const BaseImpl& node,
                                 const std::string& command,
                                 const std::string& usage,
                                 const size_t numParameters, command_t commandFunction);

    virtual void deregisterCommand(const BaseImpl& node,
                                   const std::string& command);

    void log(const std::string& nodeName, const std::string& logString, logLevel_t logLevel);


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

    struct nodeCommand_t
    {
        nodeCommand_t() {}
        std::string m_commandName;
        std::string m_usage;
        size_t m_argumentsNumber;
        std::map<std::string, command_t> m_delegates;
    };
    typedef std::map<std::string, nodeCommand_t> nodeCommands_t;
    nodeCommands_t m_nodeCommands;
};

class EpicsLogStreamBufferImpl: public std::stringbuf
{
public:
    EpicsLogStreamBufferImpl(const std::string& nodeName, const logLevel_t logLevel, EpicsFactoryImpl* pEpicsFactory);

protected:
    virtual int sync();

    std::string m_nodeName;
    logLevel_t m_logLevel;
    EpicsFactoryImpl* m_pFactory;
};

class EpicsLogStream: public std::ostream
{
public:
    EpicsLogStream(const std::string& nodeName, const logLevel_t logLevel, EpicsFactoryImpl* pEpicsFactory);

protected:
    EpicsLogStreamBufferImpl m_buffer;

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
