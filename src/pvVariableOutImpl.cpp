#include "../include/nds3impl/pvVariableOutImpl.h"

namespace nds
{

template <typename T>
PVVariableOutImpl<T>::PVVariableOutImpl(const std::string& name): PVBaseOutImpl(name), m_value()
{
    m_timestamp.tv_sec = 0;
    m_timestamp.tv_nsec = 0;

}

template <typename T>
void PVVariableOutImpl<T>::read(timespec* pTimestamp, T* pValue) const
{
    std::unique_lock<std::mutex> lock(m_pvMutex);
    *pValue = m_value;
    *pTimestamp = m_timestamp;
}

template <typename T>
void PVVariableOutImpl<T>::write(const timespec& timestamp, const T& value)
{
    std::unique_lock<std::mutex> lock(m_pvMutex);
    m_value =value;
    m_timestamp = timestamp;
}

template <typename T>
dataType_t PVVariableOutImpl<T>::getDataType() const
{
    return getDataTypeForCPPType<T>();
}

template <typename T>
T PVVariableOutImpl<T>::getValue() const
{
    std::unique_lock<std::mutex> lock(m_pvMutex);
    return m_value;
}

template <typename T>
void PVVariableOutImpl<T>::getValue(timespec* pTime, T* pValue) const
{
    std::unique_lock<std::mutex> lock(m_pvMutex);
    *pValue = m_value;
    *pTime = m_timestamp;
}

template class PVVariableOutImpl<std::int32_t>;
template class PVVariableOutImpl<double>;
template class PVVariableOutImpl<std::vector<std::int8_t> >;
template class PVVariableOutImpl<std::vector<std::uint8_t> >;
template class PVVariableOutImpl<std::vector<std::int32_t> >;
template class PVVariableOutImpl<std::vector<double> >;

}

