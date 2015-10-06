#include "../include/nds3/pvVariable.h"
#include "pvVariableImpl.h"

#include <cstdint>
#include <vector>

namespace nds
{

template <typename T>
PVVariable<T>::PVVariable()
{

}

template <typename T>
PVVariable<T>::PVVariable(const std::string& name):
    PVBase(std::shared_ptr<PVBaseImpl>(new PVVariableImpl<T>(name)))
{}


template class PVVariable<std::int32_t>;
template class PVVariable<double>;
template class PVVariable<std::vector<std::int8_t> >;
template class PVVariable<std::vector<std::uint8_t> >;
template class PVVariable<std::vector<std::int32_t> >;
template class PVVariable<std::vector<double> >;


}

