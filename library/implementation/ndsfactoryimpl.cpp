#include <epicsStdlib.h>
#include <iocshRegisterCommon.h>
#include <registryCommon.h>


#include "ndsfactoryimpl.h"
#include "../include/nds3/ndsbase.h"

#include <iostream>

#include <dlfcn.h>

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

        for(int scanLinks= 0; scanLinks < dbGetNLinks(dbEntry.m_pDBEntry); ++scanLinks)
        {
            dbGetLinkField(dbEntry.m_pDBEntry, scanLinks);
            std::cout << dbGetFieldName(dbEntry.m_pDBEntry);
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

DynamicModule::DynamicModule(): m_moduleHandle(dlopen(0, RTLD_LAZY | RTLD_GLOBAL))
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
