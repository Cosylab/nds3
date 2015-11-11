/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "../include/nds3impl/pvBaseInImpl.h"
#include "../include/nds3impl/pvBaseOutImpl.h"
#include "../include/nds3impl/portImpl.h"
#include "../include/nds3impl/ndsFactoryImpl.h"
#include "../include/nds3impl/factoryBaseImpl.h"
#include <sstream>
#include <cstring>

namespace nds
{

PVBaseInImpl::PVBaseInImpl(const std::string& name, const inputPvType_t pvType): PVBaseImpl(name), m_pvType(pvType),
    m_decimationFactor(1), m_decimationCount(1)
{
    defineCommand("replicate", "replicate destination source", 1, std::bind(&PVBaseInImpl::commandReplicate,this, std::placeholders::_1));
    defineCommand("decimation", "decimation node decimationFactor", 1, std::bind(&PVBaseInImpl::commandDecimation,this, std::placeholders::_1));
}

void PVBaseInImpl::initialize(FactoryBaseImpl &controlSystem)
{
    PVBaseImpl::initialize(controlSystem);
    NdsFactoryImpl::getInstance().registerInputPV(this);
}

void PVBaseInImpl::deinitialize()
{
    NdsFactoryImpl::getInstance().deregisterInputPV(this);
    PVBaseImpl::deinitialize();
}

void PVBaseInImpl::replicateFrom(const std::string &sourceInputPVName)
{
    NdsFactoryImpl::getInstance().replicate(sourceInputPVName, this);
}

void PVBaseInImpl::read(timespec* /* pTimestamp */, std::int32_t* /* pValue */) const
{
    throw;
}

void PVBaseInImpl::read(timespec* /* pTimestamp */, double* /* pValue */) const
{
    throw;
}

void PVBaseInImpl::read(timespec* pTimestamp, std::vector<std::int8_t>* pValue) const
{
    // TODO
    // Epics calls this also for unsigned-int and strings
    // If we arrive here maybe we really wanted to call the unsigned int function.
    // This is as ugly as it can get: consider modifying this
    read(pTimestamp, (std::vector<std::uint8_t>*) pValue);
}

void PVBaseInImpl::read(timespec* pTimestamp, std::vector<std::uint8_t>* pValue) const
{
    // TODO
    // Epics calls this also for strings.
    // If we arrive here maybe we really wanted to call the string function.
    // This is as ugly as it can get: consider modifying this
    std::string temporaryValue;
    read(pTimestamp, &temporaryValue);
    pValue->resize(temporaryValue.size());
    ::memcpy(pValue->data(), temporaryValue.data(), temporaryValue.size());
}

void PVBaseInImpl::read(timespec* /* pTimestamp */, std::vector<std::int32_t>* /* pValue */) const
{
    throw;
}

void PVBaseInImpl::read(timespec* /* pTimestamp */, std::vector<double>* /* pValue */) const
{
    throw;
}

void PVBaseInImpl::read(timespec* /* pTimestamp */, std::string* /* pValue */) const
{
    throw;
}


template<typename T>
void PVBaseInImpl::push(const timespec& timestamp, const T& value)
{
    // Find the port then push the value
    ////////////////////////////////////
    std::shared_ptr<PortImpl> pPort(getPort());
    if(--m_decimationCount == 0) // push can only happen from one thread. No sync needed
    {
        m_decimationCount = m_decimationFactor;
        pPort->push(std::static_pointer_cast<PVBaseImpl>(shared_from_this()), timestamp, value);
    }

    // Push the value to the outputs (subscription) and inputs (replication)
    ////////////////////////////////////////////////////////////////////////
    std::lock_guard<std::mutex> lock(m_lockSubscribersList);

    for(subscribersList_t::iterator scanOutputs(m_subscriberOutputPVs.begin()), endOutputs(m_subscriberOutputPVs.end());
        scanOutputs != endOutputs;
        ++scanOutputs)
    {
        (*scanOutputs)->write(timestamp, value);
    }

    for(destinationList_t::iterator scanInputs(m_replicationDestinationPVs.begin()), endInputs(m_replicationDestinationPVs.end());
        scanInputs != endInputs;
        ++scanInputs)
    {
        (*scanInputs)->push(timestamp, value);
    }
}

void PVBaseInImpl::subscribeReceiver(PVBaseOutImpl* pReceiver)
{
    std::lock_guard<std::mutex> lock(m_lockSubscribersList);
    m_subscriberOutputPVs.insert(pReceiver);
}

void PVBaseInImpl::unsubscribeReceiver(PVBaseOutImpl* pReceiver)
{
    std::lock_guard<std::mutex> lock(m_lockSubscribersList);

    subscribersList_t::const_iterator findReceiver = m_subscriberOutputPVs.find(pReceiver);
    if(findReceiver != m_subscriberOutputPVs.end())
    {
        m_subscriberOutputPVs.erase(findReceiver);
    }
}

void PVBaseInImpl::replicateTo(PVBaseInImpl *pDestination)
{
    std::lock_guard<std::mutex> lock(m_lockSubscribersList);
    m_replicationDestinationPVs.insert(pDestination);
}

void PVBaseInImpl::stopReplicationTo(PVBaseInImpl* pDestination)
{
    std::lock_guard<std::mutex> lock(m_lockSubscribersList);

    destinationList_t::const_iterator findDestination = m_replicationDestinationPVs.find(pDestination);
    if(findDestination != m_replicationDestinationPVs.end())
    {
        m_replicationDestinationPVs.erase(findDestination);
    }
}


void PVBaseInImpl::setDecimation(const std::uint32_t decimation)
{
    m_decimationFactor = decimation;
    m_decimationCount = decimation;
}


dataDirection_t PVBaseInImpl::getDataDirection() const
{
    return dataDirection_t::input;
}

parameters_t PVBaseInImpl::commandReplicate(const parameters_t &parameters)
{
    replicateFrom(parameters[0]);
    return parameters_t();
}

parameters_t PVBaseInImpl::commandDecimation(const parameters_t &parameters)
{
    std::uint32_t decimationFactor;
    std::istringstream convertParameter(parameters[0]);
    convertParameter >> decimationFactor;
    setDecimation(decimationFactor);
    return parameters_t();
}


std::string PVBaseInImpl::buildFullExternalName(const FactoryBaseImpl& controlSystem) const
{
    return buildFullExternalName(controlSystem, false);

}
std::string PVBaseInImpl::buildFullExternalNameFromPort(const FactoryBaseImpl& controlSystem) const
{
    return buildFullExternalName(controlSystem, true);
}

std::string PVBaseInImpl::buildFullExternalName(const FactoryBaseImpl& controlSystem, const bool bStopAtPort) const
{
    std::shared_ptr<NodeImpl> temporaryPointer = m_pParent.lock();
    if(temporaryPointer == 0)
    {
        return controlSystem.getSeparator(0) + controlSystem.getRootNodeName(m_externalName);
    }

    std::string name;
    switch(m_pvType)
    {
    case inputPvType_t::generic:
        name = controlSystem.getInputPVName(m_externalName);
        break;
    case inputPvType_t::getLocalState:
        name = controlSystem.getStateMachineGetStateName(m_externalName);
        break;
    case inputPvType_t::getGlobalState:
        name = controlSystem.getStateMachineGetGlobalStateName(m_externalName);
        break;
    }

    if(name.empty())
    {
        if(bStopAtPort)
        {
            return temporaryPointer->buildFullExternalNameFromPort(controlSystem);
        }
        else
        {
            return temporaryPointer->buildFullExternalName(controlSystem);
        }
    }

    if(bStopAtPort)
    {
        return temporaryPointer->buildFullExternalNameFromPort(controlSystem) + controlSystem.getSeparator(m_nodeLevel) + name;
    }

    return temporaryPointer->buildFullExternalName(controlSystem) + controlSystem.getSeparator(m_nodeLevel) + name;
}


template void PVBaseInImpl::push<std::int32_t>(const timespec&, const std::int32_t&);
template void PVBaseInImpl::push<double>(const timespec&, const double&);
template void PVBaseInImpl::push<std::vector<std::int8_t> >(const timespec&, const std::vector<std::int8_t>&);
template void PVBaseInImpl::push<std::vector<std::uint8_t> >(const timespec&, const std::vector<std::uint8_t>&);
template void PVBaseInImpl::push<std::vector<std::int32_t> >(const timespec&, const std::vector<std::int32_t>&);
template void PVBaseInImpl::push<std::vector<double> >(const timespec&, const std::vector<double>&);
template void PVBaseInImpl::push<std::string >(const timespec&, const std::string&);

}

