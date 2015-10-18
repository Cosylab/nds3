#include "../include/nds3/pvVariableIn.h"
#include "../include/nds3impl/pvVariableInImpl.h"

#include <cstdint>
#include <vector>

namespace nds
{

template <typename T>
PVVariableIn<T>::PVVariableIn()
{

}

template <typename T>
PVVariableIn<T>::PVVariableIn(const std::string& name):
    PVBaseIn(std::shared_ptr<PVBaseInImpl>(new PVVariableInImpl<T>(name)))
{}

template <typename T>
void PVVariableIn<T>::setValue(const T& value)
{
    return std::static_pointer_cast<PVVariableInImpl<T> >(m_pImplementation)->setValue(value);
}

template <typename T>
void PVVariableIn<T>::setValue(const timespec& timestamp, const T& value)
{
    return std::static_pointer_cast<PVVariableInImpl<T> >(m_pImplementation)->setValue(timestamp, value);
}

template class PVVariableIn<std::int32_t>;
template class PVVariableIn<double>;
template class PVVariableIn<std::vector<std::int8_t> >;
template class PVVariableIn<std::vector<std::uint8_t> >;
template class PVVariableIn<std::vector<std::int32_t> >;
template class PVVariableIn<std::vector<double> >;


}



