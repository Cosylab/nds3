#include "../include/nds3impl/factoryBaseImpl.h"
#include "../include/nds3impl/ndsFactoryImpl.h"
#include "../include/nds3impl/baseImpl.h"
#include "../include/nds3impl/nodeImpl.h"
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
    std::lock_guard<std::mutex> lock(m_mutex);

    // Deregister all the PVs
    /////////////////////////
    for(heldNodes_t::iterator scanDevices(m_heldNodes.begin()), endScan(m_heldNodes.end()); scanDevices != endScan; ++scanDevices)
    {
        for(nodesList_t::iterator scanNodes(scanDevices->second.begin()), endNodes(scanDevices->second.end()); scanNodes != endNodes; ++scanNodes)
        {
            (*scanNodes)->deinitializeRootNode();
        }
    }

    // Release counter on the held root nodes
    /////////////////////////////////////////
    m_heldNodes.clear();

    // Deallocate devices
    /////////////////////
    for(allocatedDevices_t::iterator scanAllocated(m_allocatedDevices.begin()), endAllocated(m_allocatedDevices.end()); scanAllocated != endAllocated; ++scanAllocated)
    {
        scanAllocated->second.m_deallocationFunction(scanAllocated->second.m_pDevice);
    }

}


/*
 * @brief Load a dynamic library containing a device driver
 */
void FactoryBaseImpl::loadDriver(const std::string &libraryName)
{
    NdsFactoryImpl::getInstance().loadDriver(libraryName);
}


/*
 * Deregister a device driver from the control system
 */
void* FactoryBaseImpl::createDevice(const std::string& driverName, const std::string& deviceName, const namedParameters_t& parameters)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if(m_allocatedDevices.find(deviceName) != m_allocatedDevices.end())
        {
            std::ostringstream errorMessage;
            errorMessage << "A device with named " << deviceName << " for the control system " << getName() << " has already been created";
            throw DeviceAlreadyCreated(errorMessage.str());
        }
        m_allocatedDevices[deviceName].m_pDevice = 0;
    }

    std::pair<void*, deallocateDriver_t> newDevice;
    try
    {
        newDevice = NdsFactoryImpl::getInstance().createDevice(*this, driverName, deviceName, parameters);
    }
    catch(...)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_allocatedDevices.erase(deviceName);
        throw;
    }

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_allocatedDevices[deviceName].m_pDevice = newDevice.first;
        m_allocatedDevices[deviceName].m_deallocationFunction = newDevice.second;
    }

    return newDevice.first;
}



void FactoryBaseImpl::destroyDevice(void* pDevice)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Deregister all the PVs
    /////////////////////////
    for(heldNodes_t::iterator scanDevices(m_heldNodes.begin()), endScan(m_heldNodes.end()); scanDevices != endScan; ++scanDevices)
    {
        if((*scanDevices).first == pDevice)
        {
            for(nodesList_t::iterator scanNodes(scanDevices->second.begin()), endNodes(scanDevices->second.end()); scanNodes != endNodes; ++scanNodes)
            {
                (*scanNodes)->deinitializeRootNode();
            }
        }
    }

    // Deallocate drivers
    /////////////////////
    for(allocatedDevices_t::iterator scanAllocated(m_allocatedDevices.begin()), endAllocated(m_allocatedDevices.end()); scanAllocated != endAllocated; ++scanAllocated)
    {
        if(scanAllocated->second.m_pDevice == pDevice)
        {
            scanAllocated->second.m_deallocationFunction(scanAllocated->second.m_pDevice);
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
    void* pDevice(0);

    if(!deviceName.empty())
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        allocatedDevices_t::const_iterator findDevice = m_allocatedDevices.find(deviceName);
        if(findDevice == m_allocatedDevices.end())
        {
            std::ostringstream errorMessage;
            errorMessage << "The device " << deviceName << " was never allocated or has already been destroyed";
            throw DeviceNotAllocated(errorMessage.str());
        }
        pDevice = findDevice->second.m_pDevice;
    }

    destroyDevice(pDevice);
}


std::thread FactoryBaseImpl::createThread(const std::string &name, threadFunction_t function)
{
    std::thread newThread(function);
    pthread_setname_np(newThread.native_handle(), name.c_str());
    return newThread;
}


void FactoryBaseImpl::holdNode(void* pDeviceObject, std::shared_ptr<NodeImpl> pHoldNode)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_heldNodes[pDeviceObject].push_back(pHoldNode);

}

}
