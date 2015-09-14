#include "../include/nds3/ndsfactory.h"
#include "ndsepicsfactoryimpl.h"

namespace nds
{

Factory::Factory(const controlSystem_t controlSystemType)
{
    switch(controlSystemType)
    {
    case controlSystem_t::test:
        throw;
    case controlSystem_t::epics:
        m_pFactory = &(EpicsFactoryImpl::getInstance());
        break;
    case controlSystem_t::tango:
        throw;
    case controlSystem_t::defaultSystem:
        m_pFactory = &(EpicsFactoryImpl::getInstance());
        break;
    }

}

void Factory::registerDriver(const std::string &driverName, allocateDriver_t allocateFunction, deallocateDriver_t deallocateFunction)
{
    m_pFactory->registerDriver(driverName, allocateFunction, deallocateFunction);
}

void Factory::run()
{
    m_pFactory->run();
}

}
