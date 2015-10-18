#include "../include/nds3impl/pvVariableInImpl.h"
#include "../include/nds3impl/pvBaseOutImpl.h"

namespace nds
{

template <typename T>
PVVariableInImpl<T>::PVVariableInImpl(const std::string& name): PVBaseInImpl(name), m_value()
{
    m_timestamp.tv_sec = 0;
    m_timestamp.tv_nsec = 0;

}

template <typename T>
void PVVariableInImpl<T>::read(timespec* pTimestamp, T* pValue) const
{
    std::unique_lock<std::mutex> lock(m_pvMutex);
    *pValue = m_value;
    *pTimestamp = m_timestamp;
}

template <typename T>
void PVVariableInImpl<T>::setValue(const timespec& timestamp, const T& value)
{
    {
        std::unique_lock<std::mutex> lock(m_pvMutex);
        m_value =value;
        m_timestamp = timestamp;
    }

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

template <typename T>
void PVVariableInImpl<T>::setValue(const T& value)
{
    setValue(getTimestamp(), value);
}

template <typename T>
dataType_t PVVariableInImpl<T>::getDataType() const
{
    return getDataTypeForCPPType<T>();
}

template class PVVariableInImpl<std::int32_t>;
template class PVVariableInImpl<double>;
template class PVVariableInImpl<std::vector<std::int8_t> >;
template class PVVariableInImpl<std::vector<std::uint8_t> >;
template class PVVariableInImpl<std::vector<std::int32_t> >;
template class PVVariableInImpl<std::vector<double> >;

}
