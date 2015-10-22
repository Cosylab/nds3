#include "../include/nds3/pvVariableOut.h"
#include "../include/nds3impl/pvVariableOutImpl.h"

#include <cstdint>
#include <vector>

namespace nds
{

template <typename T>
PVVariableOut<T>::PVVariableOut()
{

}

template <typename T>
PVVariableOut<T>::PVVariableOut(const std::string& name):
    PVBaseOut(std::shared_ptr<PVBaseOutImpl>(new PVVariableOutImpl<T>(name)))
{}

template <typename T>
T PVVariableOut<T>::getValue() const
{
    return std::static_pointer_cast<PVVariableOutImpl<T> >(m_pImplementation)->getValue();
}

template <typename T>
void PVVariableOut<T>::getValue(timespec* pTime, T* pValue) const
{
    std::static_pointer_cast<PVVariableOutImpl<T> >(m_pImplementation)->getValue(pTime, pValue);
}

template class PVVariableOut<std::int32_t>;
template class PVVariableOut<double>;
template class PVVariableOut<std::vector<std::int8_t> >;
template class PVVariableOut<std::vector<std::uint8_t> >;
template class PVVariableOut<std::vector<std::int32_t> >;
template class PVVariableOut<std::vector<double> >;
template class PVVariableOut<std::string>;


}


