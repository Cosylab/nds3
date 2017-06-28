/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include <sstream>
#include <cstdio>
#include <algorithm>

#include "nds3/exceptions.h"
#include "nds3/factory.h"
#include "nds3/impl/factoryBaseImpl.h"
#include "nds3/impl/ndsFactoryImpl.h"
#include "nds3/impl/baseImpl.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/threadStd.h"
#include "nds3/impl/iniFileParserImpl.h"

namespace nds
{

FactoryBaseImpl::FactoryBaseImpl()
{

}

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


ThreadBaseImpl* FactoryBaseImpl::runInThread(const std::string &name, threadFunction_t function)
{
    return new ThreadStd(this, name, function);
}


void FactoryBaseImpl::holdNode(void* pDeviceObject, std::shared_ptr<NodeImpl> pHoldNode)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_heldNodes[pDeviceObject].push_back(pHoldNode);

}

const std::string& FactoryBaseImpl::getSeparator(const std::uint32_t nodeLevel) const
{
    if(m_namingRules.get() == 0 || m_namingRulesName.empty())
    {
        return getDefaultSeparator(nodeLevel);
    }
    if(nodeLevel == 0)
    {
        return m_namingRules->getString(m_namingRulesName, "separator0", getDefaultSeparator(0));
    }

    std::ostringstream thisLevelKey;
    thisLevelKey << "separator" << nodeLevel;
    if(m_namingRules->keyExists(m_namingRulesName, "separator1") || m_namingRules->keyExists(m_namingRulesName, "separator0"))
    {
        // At leas one separator rule has been defined
        return m_namingRules->getString(m_namingRulesName, thisLevelKey.str(), getSeparator(nodeLevel - 1));
    }
    return m_namingRules->getString(m_namingRulesName, thisLevelKey.str(), getDefaultSeparator(nodeLevel));
}

void FactoryBaseImpl::loadNamingRules(std::istream& rules)
{
    m_namingRules.reset(new IniFileParserImpl(rules));

    IniFileParserImpl::sectionsList_t sections(m_namingRules->getSections());
    if(sections.size() == 1)
    {
        m_namingRulesName = sections.front();
    }
    else
    {
        m_namingRulesName.clear();
    }
}

void FactoryBaseImpl::setNamingRules(const std::string& rulesName)
{
    m_namingRulesName = rulesName;
}


std::string FactoryBaseImpl::getRootNodeName(const std::string& name) const
{
    return buildNameFromRule(name, "rootNode", "genericNode");
}

std::string FactoryBaseImpl::getGenericChannelName(const std::string& name) const
{
    return buildNameFromRule(name, "genericNode");
}

std::string FactoryBaseImpl::getInputChannelName(const std::string& name) const
{
    return buildNameFromRule(name, "inputNode", "sourceNode","genericNode");
}

std::string FactoryBaseImpl::getOutputChannelName(const std::string& name) const
{
    return buildNameFromRule(name, "outputNode", "sinkNode","genericNode");
}

std::string FactoryBaseImpl::getSourceChannelName(const std::string& name) const
{
    return buildNameFromRule(name, "sourceNode", "inputNode","genericNode");
}

std::string FactoryBaseImpl::getSinkChannelName(const std::string& name) const
{
    return buildNameFromRule(name, "sinkNode", "outputNode","genericNode");
}

std::string FactoryBaseImpl::getInputPVName(const std::string& name) const
{
    return buildNameFromRule(name, "inputPV", "genericPV");
}

std::string FactoryBaseImpl::getOutputPVName(const std::string& name) const
{
    return buildNameFromRule(name, "outputPV", "genericPV");
}

std::string FactoryBaseImpl::getStateMachineNodeName(const std::string& name) const
{
    return buildNameFromRule(name, "stateMachineNode", "genericNode");
}

std::string FactoryBaseImpl::getStateMachineSetStateName(const std::string& name) const
{
    return buildNameFromRule(name, "setStatePV", "outputPV", "genericPV");
}

std::string FactoryBaseImpl::getStateMachineGetStateName(const std::string& name) const
{
    return buildNameFromRule(name, "getStatePV", "inputPV", "genericPV");
}

std::string FactoryBaseImpl::getStateMachineGetGlobalStateName(const std::string& name) const
{
    return buildNameFromRule(name, "getGlobalStatePV", "inputPV", "genericPV");
}

std::string FactoryBaseImpl::getDecimationPVName(const std::string& name) const
{
    return buildNameFromRule(name, "setDecimationPV", "outputPV", "genericPV");
}

std::string FactoryBaseImpl::buildNameFromRule(const std::string& name,
                                               const std::string& rule0,
                                               const std::string& rule1,
                                               const std::string& rule2,
                                               const std::string& rule3,
                                               const std::string& rule4
                                               ) const
{
    if(m_namingRules.get() == 0 || m_namingRulesName.empty())
    {
        return name;
    }

    const std::string defaultRule("%s");
    const std::string& rule = m_namingRules->getString(m_namingRulesName, rule0,
                               m_namingRules->getString(m_namingRulesName, rule1,
                                m_namingRules->getString(m_namingRulesName, rule2,
                                 m_namingRules->getString(m_namingRulesName, rule3,
                                  m_namingRules->getString(m_namingRulesName, rule4, defaultRule)))));

    std::string adjustedName(name);
    bool toUpper = m_namingRules->getString(m_namingRulesName, "toUpper", "0") == "1";
    bool toLower = m_namingRules->getString(m_namingRulesName, "toLower", "0") == "1";
    if(toUpper)
    {
        std::transform(name.begin(), name.end(), adjustedName.begin(), ::toupper);
    }
    if(toLower)
    {
        std::transform(name.begin(), name.end(), adjustedName.begin(), ::tolower);
    }

    char buffer[1024];
    snprintf(buffer, sizeof(buffer), rule.c_str(), adjustedName.c_str());

    // Replace spaces with 0
    std::string returnString(buffer);
    std::replace(returnString.begin(), returnString.end(), ' ', '0');

    return returnString;


}




}
