#include "ndsfactorybaseimpl.h"

namespace nds
{

FactoryBaseImpl::~FactoryBaseImpl()
{
    // Deallocate drivers
    /////////////////////
    for(allocatedDrivers_t::iterator scanAllocated(m_allocatedDrivers.begin()), endAllocated(m_allocatedDrivers.end()); scanAllocated != endAllocated; ++scanAllocated)
    {
        m_driversAllocDealloc[(*scanAllocated).first].second((*scanAllocated).second);
    }

}

void FactoryBaseImpl::registerDriver(const std::string& driverName, allocateDriver_t allocateFunction, deallocateDriver_t deallocateFunction)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_driversAllocDealloc[driverName] = std::pair<allocateDriver_t, deallocateDriver_t>(allocateFunction, deallocateFunction);
}

void* FactoryBaseImpl::createDriver(const std::string& name, const std::string& parameter)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    void* driver = m_driversAllocDealloc[name].first(parameter);

    m_allocatedDrivers.insert(std::pair<std::string, void*> (name, driver));

    return driver;
}

}
