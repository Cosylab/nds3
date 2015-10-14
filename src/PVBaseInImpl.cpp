#include "pvBaseInImpl.h"
#include "pvBaseOutImpl.h"
#include "portImpl.h"

namespace nds
{

PVBaseInImpl::PVBaseInImpl(const std::string& name): PVBaseImpl(name)
{

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
    // Epics calls this also for unsigned-int.
    // If we arrive here maybe we really wanted to call the unsigned int function.
    // This is as ugly as it can get: consider modifying this
    read(pTimestamp, (std::vector<std::uint8_t>*) pValue);
}

void PVBaseInImpl::read(timespec* /* pTimestamp */, std::vector<std::uint8_t>* /* pValue */) const
{
    throw;
}

void PVBaseInImpl::read(timespec* /* pTimestamp */, std::vector<std::int32_t>* /* pValue */) const
{
    throw;
}

void PVBaseInImpl::read(timespec* /* pTimestamp */, std::vector<double>* /* pValue */) const
{
    throw;
}


template<typename T>
void PVBaseInImpl::push(const timespec& timestamp, const T& value)
{
    // Find the port then push the value
    ////////////////////////////////////
    std::shared_ptr<PortImpl> pPort(getPort());
    pPort->push(std::static_pointer_cast<PVBaseImpl>(shared_from_this()), timestamp, value);

    // Push the value to the outputs
    ////////////////////////////////
    std::lock_guard<std::mutex> lock(m_lockSubscribersList);
    for(subscribersList_t::iterator scanOutputs(m_subscriberOutputPVs.begin()), endOutputs(m_subscriberOutputPVs.end());
        scanOutputs != endOutputs;
        ++scanOutputs)
    {
        (*scanOutputs)->write(timestamp, value);
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
    m_subscriberOutputPVs.erase(pReceiver);
}



dataDirection_t PVBaseInImpl::getDataDirection() const
{
    return dataDirection_t::input;
}

template void PVBaseInImpl::push<std::int32_t>(const timespec&, const std::int32_t&);
template void PVBaseInImpl::push<double>(const timespec&, const double&);
template void PVBaseInImpl::push<std::vector<std::int8_t> >(const timespec&, const std::vector<std::int8_t>&);
template void PVBaseInImpl::push<std::vector<std::uint8_t> >(const timespec&, const std::vector<std::uint8_t>&);
template void PVBaseInImpl::push<std::vector<std::int32_t> >(const timespec&, const std::vector<std::int32_t>&);
template void PVBaseInImpl::push<std::vector<double> >(const timespec&, const std::vector<double>&);

}

