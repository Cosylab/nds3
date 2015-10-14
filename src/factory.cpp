#include "../include/nds3/factory.h"
#include "factoryBaseImpl.h"
#include "ndsFactoryImpl.h"

namespace nds
{

Factory::Factory(std::shared_ptr<FactoryBaseImpl> impl): m_pFactory(impl)
{
}

Factory::Factory(const std::string& controlSystemName)
{
    NdsFactoryImpl& factory = NdsFactoryImpl::getInstance();
    m_pFactory = factory.getControlSystem(controlSystemName);
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
