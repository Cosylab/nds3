#include "factoryBaseImpl.h"
#include "baseImpl.h"
#include <thread>

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

void* FactoryBaseImpl::createDevice(const std::string& name, const std::string& parameter)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    void* driver = m_driversAllocDealloc[name].first(parameter);

    m_allocatedDevices.insert(std::pair<std::string, void*> (name, driver));

    return driver;
}

std::thread FactoryBaseImpl::createThread(const std::string &name, threadFunction_t function)
{
    std::thread newThread(function);
    pthread_setname_np(newThread.native_handle(), name.c_str());
    return newThread;
}

void FactoryBaseImpl::destroyDriver(void* pDevice)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    // Deregister all the PVs
    /////////////////////////
    for(heldNodes_t::iterator scanDevices(m_heldNodes.begin()), endScan(m_heldNodes.end()); scanDevices != endScan; ++scanDevices)
    {
        if((*scanDevices).first == pDevice)
        {
            for(nodesList_t::iterator scanNodes(scanDevices->second.begin()), endNodes(scanDevices->second.end()); scanNodes != endNodes; ++scanNodes)
            {
                (*scanNodes)->deinitialize();
            }
        }
    }

    // Deallocate drivers
    /////////////////////
    for(allocatedDevices_t::iterator scanAllocated(m_allocatedDevices.begin()), endAllocated(m_allocatedDevices.end()); scanAllocated != endAllocated; ++scanAllocated)
    {
        if((*scanAllocated).second == pDevice)
        {
            m_driversAllocDealloc[(*scanAllocated).first].second((*scanAllocated).second);
            m_allocatedDevices.erase(scanAllocated);
            break;
        }
    }

    // Deallocate all the PVs
    /////////////////////////
    for(heldNodes_t::iterator scanDevices(m_heldNodes.begin()), endScan(m_heldNodes.end()); scanDevices != endScan; ++scanDevices)
    {
        if((*scanDevices).first == pDevice)
        {
            m_heldNodes.erase(scanDevices);
            break;
        }
    }

}

void FactoryBaseImpl::holdNode(void* pDeviceObject, std::shared_ptr<BaseImpl> pHoldNode)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    m_heldNodes[pDeviceObject].push_back(pHoldNode);

}

}
