#include "factoryBaseImpl.h"
#include "baseImpl.h"
#include <thread>
#include <sstream>
#include "../include/nds3/exceptions.h"
#include "../include/nds3/factory.h"

namespace nds
{

/*
 * Destructor
 */
FactoryBaseImpl::~FactoryBaseImpl()
{

}


/*
 * Deinitialize the nodes before deleting the control system
 */
void FactoryBaseImpl::preDelete()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    // Deregister all the PVs
    /////////////////////////
    for(heldNodes_t::iterator scanDevices(m_heldNodes.begin()), endScan(m_heldNodes.end()); scanDevices != endScan; ++scanDevices)
    {
        for(nodesList_t::iterator scanNodes(scanDevices->second.begin()), endNodes(scanDevices->second.end()); scanNodes != endNodes; ++scanNodes)
        {
            (*scanNodes)->deinitialize();
        }
    }

    // Release counter on the held nodes
    ////////////////////////////////////
    m_heldNodes.clear();

    // Deallocate devices
    /////////////////////
    for(allocatedDevices_t::iterator scanAllocated(m_allocatedDevices.begin()), endAllocated(m_allocatedDevices.end()); scanAllocated != endAllocated; ++scanAllocated)
    {
        m_driversAllocDealloc[scanAllocated->second.m_driverName].second(scanAllocated->second.m_pDevice);
    }

}


/*
 * Register a device driver with the control system
 */
void FactoryBaseImpl::registerDriver(const std::string& driverName, allocateDriver_t allocateFunction, deallocateDriver_t deallocateFunction)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    m_driversAllocDealloc[driverName] = std::pair<allocateDriver_t, deallocateDriver_t>(allocateFunction, deallocateFunction);
}


/*
 * Deregister a device driver from the control system
 */
void* FactoryBaseImpl::createDevice(const std::string& driverName, const std::string& deviceName, const namedParameters_t& parameters)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    if(m_allocatedDevices.find(deviceName) != m_allocatedDevices.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "A device with named " << deviceName << " for the control system " << getName() << " has already been created";
        throw DeviceAlreadyCreated(errorMessage.str());
    }

    driverAllocDeallocMap_t::const_iterator findDriver = m_driversAllocDealloc.find(driverName);
    if(findDriver == m_driversAllocDealloc.end())
    {
        std::ostringstream error;
        error << "The driver " << driverName << " has not been registered";
        throw DriverNotFound(error.str());
    }

    Factory factory(shared_from_this());
    void* device = findDriver->second.first(factory, deviceName, parameters);

    m_allocatedDevices[deviceName].m_pDevice = device;
    m_allocatedDevices[deviceName].m_driverName = driverName;

    return device;
}



void FactoryBaseImpl::destroyDevice(void* pDevice)
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
        if(scanAllocated->second.m_pDevice == pDevice)
        {
            m_driversAllocDealloc[scanAllocated->second.m_driverName].second(scanAllocated->second.m_pDevice);
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

void FactoryBaseImpl::destroyDevice(const std::string& deviceName)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    allocatedDevices_t::const_iterator findDevice = m_allocatedDevices.find(deviceName);
    if(findDevice == m_allocatedDevices.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "The device " << deviceName << " was never allocated or has already been destroyed";
        throw DeviceNotAllocated(errorMessage.str());
    }

    destroyDevice(findDevice->second.m_pDevice);
}


std::thread FactoryBaseImpl::createThread(const std::string &name, threadFunction_t function)
{
    std::thread newThread(function);
    pthread_setname_np(newThread.native_handle(), name.c_str());
    return newThread;
}


void FactoryBaseImpl::holdNode(void* pDeviceObject, std::shared_ptr<BaseImpl> pHoldNode)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    m_heldNodes[pDeviceObject].push_back(pHoldNode);

}

FactoryBaseImpl::driversList_t FactoryBaseImpl::getDriversList()
{
    driversList_t drivers;
    for(driverAllocDeallocMap_t::const_iterator scanDrivers(m_driversAllocDealloc.begin()), endDrivers(m_driversAllocDealloc.end());
        scanDrivers != endDrivers;
        ++scanDrivers)
    {
        drivers.push_back(scanDrivers->first);
    }
    return drivers;
}

}
