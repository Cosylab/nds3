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
m_commandArgument = {"pdbbase", iocshArgPdbbase};
}

void FactoryImpl::registrationCommand(const std::string& registrationCommandName)
{
    iocshRegisterCommon();

    m_commandName = registrationCommandName;
    m_commandDefinition.arg = &m_commandArgument;
    m_commandDefinition.nargs = 1;
    m_commandDefinition.name = m_commandName.c_str();
    iocshRegister(&m_commandDefinition, rrddCallFunc);

}


void FactoryImpl::registerRecordTypes(DBBASE* pDatabase)
{
    DBEntry dbEntry(pDatabase);

    DynamicModule thisModule;

    for(long scanTypes(dbFirstRecordType(dbEntry.m_pDBEntry)); scanTypes == 0; scanTypes = dbNextRecordType(dbEntry.m_pDBEntry))
    {
        std::string recordName(dbGetRecordTypeName(dbEntry.m_pDBEntry));

        void* resetFunction = thisModule.getAddress(recordName + "RSET");
        void* sizeOffset = thisModule.getAddress("pvar_func_" + recordName + "RecordSizeOffset");

        recordTypeFunctions.emplace_back(recordTypeLocation());
        recordTypeFunctions.back().prset = (rset*)resetFunction;
        recordTypeFunctions.back().sizeOffset = (computeSizeOffset)sizeOffset;

        recordTypeNames.push_back(recordName);
        recordTypeNamesCstr.emplace_back(recordName.c_str());
    }

    ::registerRecordTypes(pDatabase, recordTypeNames.size(), recordTypeNamesCstr.data(), recordTypeFunctions.data());

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
