#include "pvDelegateImpl.h"
#include <type_traits>
namespace nds
{

template <typename T>
PVDelegateImpl<T>::PVDelegateImpl(const std::string& name, read_t readFunction, write_t writeFunction): PVBaseImpl(name),
    m_reader(readFunction), m_writer(writeFunction)
{}

template <typename T>
PVDelegateImpl<T>::PVDelegateImpl(const std::string& name, read_t readFunction): PVBaseImpl(name),
    m_reader(readFunction), m_writer(std::bind(&PVDelegateImpl::dontWrite, this, std::placeholders::_1, std::placeholders::_2))
{}

template <typename T>
void PVDelegateImpl<T>::read(timespec* pTimestamp, T* pValue)
{
    m_reader(pTimestamp, pValue);
}

template <typename T>
void PVDelegateImpl<T>::write(const timespec& timestamp, const T& value)
{
    m_writer(timestamp, value);
}

template <typename T>
dataType_t PVDelegateImpl<T>::getDataType()
{
    return getDataTypeForCPPType<T>();
}

template <typename T>
void PVDelegateImpl<T>::dontWrite(const timespec&, const T&)
{

}

template class PVDelegateImpl<std::int32_t>;
template class PVDelegateImpl<double>;
template class PVDelegateImpl<std::vector<std::int8_t> >;
template class PVDelegateImpl<std::vector<std::uint8_t> >;
template class PVDelegateImpl<std::vector<std::int32_t> >;
template class PVDelegateImpl<std::vector<double> >;

}

