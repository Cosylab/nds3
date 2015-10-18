#include "../include/nds3impl/pvDelegateInImpl.h"
#include <type_traits>
namespace nds
{

template <typename T>
PVDelegateInImpl<T>::PVDelegateInImpl(const std::string& name, read_t readFunction): PVBaseInImpl(name),
    m_reader(readFunction)
{}

template <typename T>
void PVDelegateInImpl<T>::read(timespec* pTimestamp, T* pValue) const
{
    m_reader(pTimestamp, pValue);
}


template <typename T>
dataType_t PVDelegateInImpl<T>::getDataType() const
{
    return getDataTypeForCPPType<T>();
}

template class PVDelegateInImpl<std::int32_t>;
template class PVDelegateInImpl<double>;
template class PVDelegateInImpl<std::vector<std::int8_t> >;
template class PVDelegateInImpl<std::vector<std::uint8_t> >;
template class PVDelegateInImpl<std::vector<std::int32_t> >;
template class PVDelegateInImpl<std::vector<double> >;

}


