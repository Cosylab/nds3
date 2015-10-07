#include "pvDelegateOutImpl.h"
#include <type_traits>
namespace nds
{

template <typename T>
PVDelegateOutImpl<T>::PVDelegateOutImpl(const std::string& name, write_t writeFunction, initValue_t initValueFunction): PVBaseOutImpl(name),
    m_writer(writeFunction),
    m_initializer(initValueFunction),
    m_bInitialize(true)
{}

template <typename T>
PVDelegateOutImpl<T>::PVDelegateOutImpl(const std::string& name, write_t writeFunction): PVBaseOutImpl(name),
    m_writer(writeFunction),
    m_initializer(std::bind(&PVDelegateOutImpl::dontInitialize, this, std::placeholders::_1, std::placeholders::_2)),
    m_bInitialize(false)
{}

template <typename T>
void PVDelegateOutImpl<T>::read(timespec* pTimestamp, T* pValue) const
{
    m_initializer(pTimestamp, pValue);
}

template <typename T>
void PVDelegateOutImpl<T>::write(const timespec& timestamp, const T& value)
{
    m_writer(timestamp, value);
}

template <typename T>
dataType_t PVDelegateOutImpl<T>::getDataType() const
{
    return getDataTypeForCPPType<T>();
}

template <typename T>
void PVDelegateOutImpl<T>::dontInitialize(timespec*, T*)
{

}

template class PVDelegateOutImpl<std::int32_t>;
template class PVDelegateOutImpl<double>;
template class PVDelegateOutImpl<std::vector<std::int8_t> >;
template class PVDelegateOutImpl<std::vector<std::uint8_t> >;
template class PVDelegateOutImpl<std::vector<std::int32_t> >;
template class PVDelegateOutImpl<std::vector<double> >;

}

