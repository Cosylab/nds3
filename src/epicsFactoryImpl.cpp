#ifdef NDS3_EPICS

#include <epicsStdlib.h>
#include <iocshRegisterCommon.h>
#include <registryCommon.h>
#include <dbStaticPvt.h>
#include <epicsThread.h>
#include <errlog.h>

#include "epicsFactoryImpl.h"
#include "epicsInterfaceImpl.h"
#include "../include/nds3/base.h"

#include <iostream>
#include <fstream>
#include "/usr/include/link.h"
#include <elf.h>
#include <dlfcn.h>

#include <set>
#include <string>
#include <sstream>
#include "scansymbols.h"

// Include embedded dbd file
#include "../dbd/dbdfile.h"

typedef void (*reg_func)(void);
epicsShareExtern reg_func pvar_func_arrInitialize, pvar_func_asSub,
    pvar_func_asynInterposeEosRegister,
    pvar_func_asynInterposeFlushRegister, pvar_func_asynRegister,
    pvar_func_dbndInitialize, pvar_func_ndsRegister,
    pvar_func_syncInitialize, pvar_func_tsInitialize;

namespace nds
{

extern "C" {

void EpicsFactoryImpl::createNdsDevice(const iocshArgBuf * arguments)
{
    if(arguments[0].sval == 0)
    {
        errlogSevPrintf(errlogInfo, "Usage of command createNdsDevice: createNdsDevice deviceName [parameters]\n");
        return;
    }
    std::string parameter;
    if(arguments[1].sval != 0)
    {
        parameter = arguments[1].sval;
    }
    EpicsFactoryImpl::getInstance().createDevice(arguments[0].sval, parameter);
}

void EpicsFactoryImpl::ndsUserCommand(const iocshArgBuf * arguments)
{
    if(arguments[0].sval == 0 || arguments[1].sval == 0)
    {
        errlogSevPrintf(errlogInfo, "Usage of command nds: nds nodeCommand nodeName [parameters]\n");
        return;
    }

    EpicsFactoryImpl& epicsFactory = EpicsFactoryImpl::getInstance();

    // Retrieve the command definition
    //////////////////////////////////
    nodeCommands_t::const_iterator findCommand = epicsFactory.m_nodeCommands.find(arguments[0].sval);
    if(findCommand == epicsFactory.m_nodeCommands.end())
    {
        std::ostringstream commandsList;
        commandsList << "Command not found. Registered commands:" << std::endl;
        for(nodeCommands_t::const_iterator scanCommands(epicsFactory.m_nodeCommands.begin()), endCommands(epicsFactory.m_nodeCommands.end()); scanCommands != endCommands; ++scanCommands)
        {
            commandsList << " " << scanCommands->first << std::endl;
        }
        errlogSevPrintf(errlogInfo, commandsList.str().c_str());
        return;
    }

    // Check the node name
    //////////////////////
    std::map<std::string, command_t>::const_iterator findNode = findCommand->second.m_delegates.find(arguments[1].sval);
    if(findNode == findCommand->second.m_delegates.end())
    {
        std::ostringstream nodesList;
        nodesList << "Node not found. Registered nodes for this command:" << std::endl;
        for(std::map<std::string, command_t>::const_iterator scanNodes(findCommand->second.m_delegates.begin()), endNodes(findCommand->second.m_delegates.end());
            scanNodes != endNodes; ++scanNodes)
        {
            nodesList << " " << scanNodes->first << std::endl;
        }
        errlogSevPrintf(errlogInfo, nodesList.str().c_str());
        return;

    }

    // Check the number of parameters
    /////////////////////////////////
    parameters_t parameters;
    for(size_t argument(2); arguments[argument].sval != 0; ++argument)
    {
        parameters.push_back(arguments[argument].sval);
    }
    if(parameters.size() != findCommand->second.m_argumentsNumber)
    {
        std::ostringstream errorString;
        errorString << "Expected " << (findCommand->second.m_argumentsNumber) << " arguments but found " << parameters.size() << " instead" << std::endl;
        errlogSevPrintf(errlogInfo, errorString.str().c_str());
        return;
    }

    try
    {
        findNode->second(parameters);
    }
    catch(const std::runtime_error& e)
    {
        std::ostringstream errorString;
        errorString << e.what() << std::endl;
        errlogSevPrintf(errlogInfo, errorString.str().c_str());
    }

}

}

std::thread EpicsFactoryImpl::createThread(const std::string &name, threadFunction_t function)
{
    std::thread thread = FactoryBaseImpl::createThread(name, function);
/*
    epicsThreadOSD * pThreadInfo = ::calloc(sizeof(epicsThreadOSD) + name.size());

    ::memcpy(&(pThreadInfo->name[0]), name.c_str(), name.size());
    pThreadInfo->suspendEvent = epicsEventCreate(epicsEventEmpty);
    pThreadInfo->tid = thread.native_handle();
    pthreadInfo->osiPriority = 0;

    struct sched_param param;
    int policy;
    if(pthread_getschedparam(pThreadInfo->tid, &policy, &param) == 0)
    {
        pthreadInfo->osiPriority =
                 (param.sched_priority - pcommonAttr->minPriority) * 100.0 /
                    (pcommonAttr->maxPriority - pcommonAttr->minPriority + 1);
    }

    pthread_setspecific(getpthreadInfo ,(void *)pThreadInfo);
*/
    return thread;

}

EpicsFactoryImpl& EpicsFactoryImpl::getInstance()
{
    static EpicsFactoryImpl factory;

    return factory;
}

EpicsFactoryImpl::EpicsFactoryImpl()
{
    static const iocshArg nameArgument = {"driver", iocshArgString};
    static const iocshArg parameterArgument = {"parameter", iocshArgString};
    static const iocshArg* commandArguments[] = {&nameArgument, &parameterArgument};

    static const std::string commandName("createNdsDevice");
    m_commandDefinition.arg = commandArguments;
    m_commandDefinition.nargs = sizeof(commandArguments) / sizeof(commandArguments[0]);
    m_commandDefinition.name = commandName.c_str();
    iocshRegister(&m_commandDefinition, createNdsDevice);

    // Register the nds command for the nodes
    /////////////////////////////////////////
    static const std::string ndsName("nds");
    static const iocshArg ndsArguments[] = {
        {"commandName", iocshArgString},
        {"nodeName", iocshArgString},
        {"arg0", iocshArgString},
        {"arg1", iocshArgString},
        {"arg2", iocshArgString},
        {"arg3", iocshArgString},
        {"arg4", iocshArgString},
        {"arg5", iocshArgString},
        {"arg6", iocshArgString},
        {"arg7", iocshArgString},
        {"arg8", iocshArgString},
        {"arg9", iocshArgString} };

    static const iocshArg* argumentPointers[] = {
        &(ndsArguments[0]),
        &(ndsArguments[1]),
        &(ndsArguments[2]),
        &(ndsArguments[3]),
        &(ndsArguments[4]),
        &(ndsArguments[5]),
        &(ndsArguments[6]),
        &(ndsArguments[7]),
        &(ndsArguments[8]),
        &(ndsArguments[9]),
        &(ndsArguments[10]),
        &(ndsArguments[11])
    };

    static iocshFuncDef m_ndsDefinition;
    m_ndsDefinition.name = ndsName.c_str();
    m_ndsDefinition.nargs = 12;
    m_ndsDefinition.arg = argumentPointers;
    iocshRegister(&m_ndsDefinition, ndsUserCommand);


}

InterfaceBaseImpl* EpicsFactoryImpl::getNewInterface(const std::string& fullName)
{
    return new EpicsInterfaceImpl(fullName);
}


void EpicsFactoryImpl::run(int argc,char * argv[])
{
    iocshRegisterCommon();

    // Save and load the dbd file
    /////////////////////////////
    char tmpBuffer[L_tmpnam];

    std::string tmpFileName(tmpnam_r(tmpBuffer));

    std::string fileName(tmpFileName);
    std::ofstream outputStream(fileName.c_str());
    outputStream.write(dbdfile, sizeof(dbdfile));

    std::string command("dbLoadDatabase ");
    command += tmpFileName;
    iocshCmd(command.c_str());

    registerRecordTypes(*iocshPpdbbase);

    iocsh(0);
}


void EpicsFactoryImpl::registerRecordTypes(DBBASE* pDatabase)
{
    DBEntry dbEntry(pDatabase);

    DynamicModule thisModule;

    symbolsList_t symbols = getSymbols();

    // Find all the recordTypes, devices, variables
    std::set<std::string> recordTypes;
    std::set<std::string> devices;
    std::set<std::string> drivers;
    std::set<std::string> intVariables;
    std::set<std::string> doubleVariables;
    std::set<std::string> stringVariables;

    static const std::string epicsFuncPrefix("pvar_func_");
    static const std::string epicsDsetPrefix("pvar_dset_");
    static const std::string epicsDrvetPrefix("pvar_drvet_");
    static const std::string epicsRsetPostfix("RSET");
    static const std::string epicsSizeOffsetPostfix("RecordSizeOffset");

    static const std::string epicsIntPrefix("pvar_int_");
    static const std::string epicsDoublePrefix("pvar_double_");
    static const std::string epicsStringPrefix("pvar_string_");

    struct variablesHelpStruct
    {
        iocshArgType type;
        std::string prefix;
        std::set<std::string>* list;
    };

    variablesHelpStruct variablesHelp[] =
    {
        {iocshArgInt, epicsIntPrefix, &intVariables},
        {iocshArgDouble, epicsDoublePrefix, &doubleVariables},
        {iocshArgString, epicsStringPrefix, &stringVariables}
    };

    for(symbolsList_t::const_iterator scanSymbols(symbols.begin()), endSymbols(symbols.end()); scanSymbols != endSymbols; ++scanSymbols)
    {
        addToSet(scanSymbols->first, &recordTypes, epicsFuncPrefix, epicsSizeOffsetPostfix);
        addToSet(scanSymbols->first, &devices, epicsDsetPrefix, "");
        addToSet(scanSymbols->first, &drivers, epicsDrvetPrefix, "");
        addToSet(scanSymbols->first, &intVariables, epicsIntPrefix, "");
        addToSet(scanSymbols->first, &doubleVariables, epicsDoublePrefix, "");
        addToSet(scanSymbols->first, &stringVariables, epicsStringPrefix, "");
    }

    // Register all the record types
    ////////////////////////////////
    for(std::set<std::string>::const_iterator scanTypes(recordTypes.begin()), endTypes(recordTypes.end()); scanTypes != endTypes; ++scanTypes)
    {
        symbolsList_t::iterator findRset = symbols.find(*scanTypes + epicsRsetPostfix);
        if(findRset == symbols.end())
        {
            continue;
        }

        std::cout << "Registering type " << *scanTypes << "\n";

        // Collect both the rset and the sizeoffset functions
        const void* sizeOffset = symbols[epicsFuncPrefix + *scanTypes + epicsSizeOffsetPostfix].m_pAddress;
        const void* resetFunction = findRset->second.m_pAddress;

        m_recordTypeFunctions.emplace_back(recordTypeLocation());
        m_recordTypeFunctions.back().prset = (rset*)resetFunction;
        m_recordTypeFunctions.back().sizeOffset = *((computeSizeOffset*)sizeOffset);

        m_recordTypeNames.push_back(*scanTypes);
        m_recordTypeNamesCstr.emplace_back(m_recordTypeNames.back().c_str());
    }

    // Register all the devices
    ///////////////////////////
    for(std::set<std::string>::const_iterator scanDevices(devices.begin()), endDevices(devices.end()); scanDevices != endDevices; ++scanDevices)
    {
        std::cout << "Registering device " << *scanDevices << "\n";

        const void* deviceFunction = symbols[epicsDsetPrefix + *scanDevices].m_pAddress;
        m_deviceFunctions.push_back((dset*)*(dset**)deviceFunction);
        m_deviceNames.push_back(*scanDevices);
        m_deviceNamesCstr.emplace_back(m_deviceNames.back().c_str());
    }

    // Register all the drivers
    ///////////////////////////
    for(std::set<std::string>::const_iterator scanDrivers(drivers.begin()), endDrivers(drivers.end()); scanDrivers != endDrivers; ++scanDrivers)
    {
        std::cout << "Registering driver " << *scanDrivers << "\n";

        const void* driverFunction = symbols[epicsDrvetPrefix + *scanDrivers].m_pAddress;
        m_driverFunctions.push_back((drvet*)driverFunction);
        m_driverNames.push_back(*scanDrivers);
        m_driverNamesCstr.emplace_back(m_driverNames.back().c_str());
    }

    // Register all the variables
    /////////////////////////////
    for(int scanVariablesHelp(0); scanVariablesHelp != sizeof(variablesHelp) / sizeof(variablesHelp[0]); ++scanVariablesHelp)
    {
        std::set<std::string>* pList = variablesHelp[scanVariablesHelp].list;
        for(std::set<std::string>::const_iterator scanVariables(pList->begin()), endVariables(pList->end()); scanVariables != endVariables; ++scanVariables)
        {
            const void* variableFunction = symbols[variablesHelp[scanVariablesHelp].prefix + *scanVariables].m_pAddress;
            m_variableNames.push_back(*scanVariables);
            m_variableFunctions.emplace_back(iocshVarDef());
            m_variableFunctions.back().name = m_variableNames.back().c_str();
            m_variableFunctions.back().type = variablesHelp[scanVariablesHelp].type;
            m_variableFunctions.back().pval = (void*)variableFunction;
        }
    }

    m_variableFunctions.emplace_back(iocshVarDef());
    m_variableFunctions.back().name = 0;
    m_variableFunctions.back().type = iocshArgInt;
    m_variableFunctions.back().pval = 0;

    ::registerRecordTypes(pDatabase, m_recordTypeNames.size(), m_recordTypeNamesCstr.data(), m_recordTypeFunctions.data());

    ::registerDevices(pDatabase, m_deviceNames.size(), m_deviceNamesCstr.data(), m_deviceFunctions.data());

    ::registerDrivers(pDatabase, m_driverNames.size(), m_driverNamesCstr.data(), m_driverFunctions.data());

    pvar_func_arrInitialize();
    pvar_func_asSub();
    pvar_func_asynInterposeEosRegister();
    pvar_func_asynInterposeFlushRegister();
    pvar_func_asynRegister();
    pvar_func_dbndInitialize();
    pvar_func_syncInitialize();
    pvar_func_tsInitialize();

    ::iocshRegisterVariable(m_variableFunctions.data());

}

void EpicsFactoryImpl::addToSet(const std::string symbolName, std::set<std::string>* pSet, const std::string& prefix, const std::string& postfix)
{
    std::string name = compareString(symbolName, prefix, postfix);
    if(!name.empty())
    {
        pSet->insert(name);
    }
}

std::string EpicsFactoryImpl::compareString(const std::string& string, const std::string& prefix, const std::string& postfix)
{
    if(string.size() < prefix.size() || string.size() < postfix.size())
    {
        return "";
    }

    if(string.substr(0, prefix.size()) == prefix && string.substr(string.size() - postfix.size()) == postfix)
    {
        return string.substr(prefix.size(), string.size() - prefix.size() - postfix.size());
    }

    return "";
}

void EpicsFactoryImpl::log(const std::string &nodeName, const std::string &logString, logLevel_t logLevel)
{
    switch(logLevel)
    {
    case logLevel_t::debug:
        errlogSevPrintf(errlogInfo, "Node %s : %s", nodeName.c_str(), logString.c_str());
        break;
    case logLevel_t::info:
        errlogSevPrintf(errlogInfo, "Node %s : %s", nodeName.c_str(), logString.c_str());
        break;
    case logLevel_t::warning:
        errlogSevPrintf(errlogMinor, "Node %s : %s", nodeName.c_str(), logString.c_str());
        break;
    case logLevel_t::error:
        errlogSevPrintf(errlogMajor, "Node %s : %s", nodeName.c_str(), logString.c_str());
        break;
    default:
        throw std::logic_error("Cannot log with severity level set to none");
    }
}

LogStreamGetterImpl* EpicsFactoryImpl::getLogStreamGetter()
{
    return this;
}

std::ostream* EpicsFactoryImpl::getLogStream(const BaseImpl& baseImpl, const logLevel_t logLevel)
{
    return new EpicsLogStream(baseImpl.getFullName(), logLevel, this);
}

void EpicsFactoryImpl::registerCommand(const BaseImpl& node,
                             const std::string& command,
                             const std::string& usage,
                             const size_t numParameters, command_t commandFunction)
{
    // Find for an already existing command
    ///////////////////////////////////////
    nodeCommands_t::iterator findCommand = m_nodeCommands.find(command);
    if(findCommand != m_nodeCommands.end())
    {
        // Check compatibility of the command definition
        if(findCommand->second.m_argumentsNumber != numParameters)
        {
            std::ostringstream errorString;
            errorString << "The number of parameters for the node " << node.getFullName() << " is different from the number of parameters already defined for the node "
                        << findCommand->second.m_delegates.begin()->first;

            throw std::logic_error(errorString.str());
        }
        findCommand->second.m_delegates[node.getFullName()] = commandFunction;
        return;
    }

    std::pair<nodeCommands_t::iterator, bool> commandInsert = m_nodeCommands.insert(std::pair<std::string, nodeCommand_t>(command, nodeCommand_t()));
    commandInsert.first->second.m_commandName = command;
    commandInsert.first->second.m_usage = usage;
    commandInsert.first->second.m_argumentsNumber = numParameters;
    commandInsert.first->second.m_delegates[node.getFullName()] = commandFunction;

}

void EpicsFactoryImpl::deregisterCommand(const BaseImpl& node,
                               const std::string& command)
{
    // Find the command
    ///////////////////
    nodeCommands_t::iterator findCommand = m_nodeCommands.find(command);
    if(findCommand == m_nodeCommands.end())
    {
        throw;
    }

    // Find the node
    ////////////////
    std::map<std::string, command_t>::iterator findNode = findCommand->second.m_delegates.find(node.getFullName());
    if(findNode == findCommand->second.m_delegates.end())
    {
        throw;
    }

    findCommand->second.m_delegates.erase(findNode);

    if(findCommand->second.m_delegates.empty())
    {
        m_nodeCommands.erase(findCommand);
    }
}



EpicsLogStreamBufferImpl::EpicsLogStreamBufferImpl(const std::string& nodeName, const logLevel_t logLevel, EpicsFactoryImpl* pEpicsFactory):
    m_nodeName(nodeName), m_logLevel(logLevel), m_pFactory(pEpicsFactory)
{
}

int EpicsLogStreamBufferImpl::sync()
{
    std::string string(std::string(pbase(), pptr() - pbase()));
    m_pFactory->log(m_nodeName, string, m_logLevel);
    seekpos(0);
    return 0;
}

EpicsLogStream::EpicsLogStream(const std::string& nodeName, const logLevel_t logLevel, EpicsFactoryImpl* pEpicsFactory):
    std::ostream(&m_buffer), m_buffer(nodeName, logLevel, pEpicsFactory)
{

}





DBEntry::DBEntry(DBBASE* pDatabase)
{
    m_pDBEntry = dbAllocEntry(pDatabase);
}

DBEntry::~DBEntry()
{
    dbFreeEntry(m_pDBEntry);
}

DynamicModule::DynamicModule(): m_moduleHandle(dlopen(0, RTLD_NOW | RTLD_GLOBAL | RTLD_NODELETE))
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

#endif // NDS3_EPICS
