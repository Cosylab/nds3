#include "ndsfactorybaseimpl.h"

namespace nds
{

FactoryBaseImpl::~FactoryBaseImpl()
{
    m_heldNodes.clear();

    // Deallocate drivers
    /////////////////////
    for(allocatedDevices_t::iterator scanAllocated(m_allocatedDevices.begin()), endAllocated(m_allocatedDevices.end()); scanAllocated != endAllocated; ++scanAllocated)
    {
        m_driversAllocDealloc[(*scanAllocated).first].second((*scanAllocated).second);
    }

}

void FactoryBaseImpl::registerDriver(const std::string& driverName, allocateDriver_t allocateFunction, deallocateDriver_t deallocateFunction)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    m_driversAllocDealloc[driverName] = std::pair<allocateDriver_t, deallocateDriver_t>(allocateFunction, deallocateFunction);
}

void* FactoryBaseImpl::createDriver(const std::string& name, const std::string& parameter)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    void* driver = m_driversAllocDealloc[name].first(parameter);

    m_allocatedDevices.insert(std::pair<std::string, void*> (name, driver));

    return driver;
}

void FactoryBaseImpl::holdNode(void* pDeviceObject, std::shared_ptr<BaseImpl> pHoldNode)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    m_heldNodes[pDeviceObject].push_back(pHoldNode);

}

}
