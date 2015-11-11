/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "../include/nds3/factory.h"
#include "../include/nds3impl/factoryBaseImpl.h"
#include "../include/nds3impl/ndsFactoryImpl.h"
#include "../include/nds3impl/threadBaseImpl.h"
#include "../include/nds3/thread.h"


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
    NdsFactoryImpl::getInstance().registerDriver(driverName, allocateFunction, deallocateFunction);
}

void Factory::registerControlSystem(Factory &factory)
{
    NdsFactoryImpl::getInstance().registerControlSystem(factory.m_pFactory);
}

void Factory::run(int argc,char *argv[])
{
    m_pFactory->run(argc, argv);
}

void Factory::createDevice(const std::string& driverName, const std::string& deviceName, const namedParameters_t& parameters)
{
    m_pFactory->createDevice(driverName, deviceName, parameters);
}

void Factory::destroyDevice(const std::string& deviceName)
{
    m_pFactory->destroyDevice(deviceName);
}

void Factory::subscribe(const std::string& pushFrom, const std::string& pushTo)
{
    NdsFactoryImpl::getInstance().subscribe(pushFrom, pushTo);
}

void Factory::unsubscribe(const std::string& pushTo)
{
    NdsFactoryImpl::getInstance().unsubscribe(pushTo);
}

void Factory::replicate(const std::string &replicateSource, const std::string &replicateDestination)
{
    NdsFactoryImpl::getInstance().replicate(replicateSource, replicateDestination);
}

void Factory::stopReplicationTo(const std::string &destination)
{
    NdsFactoryImpl::getInstance().stopReplicationTo(destination);
}

Thread Factory::runInThread(const std::string &name, threadFunction_t function)
{
    return Thread(std::shared_ptr<ThreadBaseImpl>(m_pFactory->runInThread(name, function)));
}

void Factory::loadNamingRules(std::istream& rules)
{
    m_pFactory->loadNamingRules(rules);
}

void Factory::setNamingRules(const std::string& rulesName)
{
    m_pFactory->setNamingRules(rulesName);
}


}
