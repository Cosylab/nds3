#include "../include/nds3/factory.h"
#include "epicsFactoryImpl.h"
#include "tangoFactoryImpl.h"
#include "factoryBaseImpl.h"

namespace nds
{

Factory::Factory(const controlSystem_t controlSystemType)
{
    switch(controlSystemType)
    {
    case controlSystem_t::test:
        throw;
    case controlSystem_t::epics:
#ifdef NDS3_EPICS
        m_pFactory = &(EpicsFactoryImpl::getInstance());
        break;
#else
        throw;
#endif
    case controlSystem_t::tango:
#ifdef NDS3_TANGO
        m_pFactory = &(TangoFactoryImpl::getInstance());
        break;
#else
        throw;
#endif
    case controlSystem_t::defaultSystem:
#ifdef NDS3_TANGO
        m_pFactory = &(TangoFactoryImpl::getInstance());
        break;
#elif defined(NDS3_EPICS)
        m_pFactory = &(EpicsFactoryImpl::getInstance());
        break;
#else
        throw;
#endif
    }

}

void Factory::registerDriver(const std::string &driverName, allocateDriver_t allocateFunction, deallocateDriver_t deallocateFunction)
{
    m_pFactory->registerDriver(driverName, allocateFunction, deallocateFunction);
}

void Factory::run(int argc,char *argv[])
{
    m_pFactory->run(argc, argv);
}

std::thread Factory::createThread(const std::string &name, threadFunction_t function)
{
    return m_pFactory->createThread(name, function);
}

}
