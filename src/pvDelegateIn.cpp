#include "../include/nds3/pvDelegateIn.h"
#include "../include/nds3impl/pvDelegateInImpl.h"

#include <cstdint>
#include <vector>

namespace nds
{

template <typename T>
PVDelegateIn<T>::PVDelegateIn(const std::string& name, read_t readFunction):
    PVBaseIn(std::shared_ptr<PVBaseInImpl>(new PVDelegateInImpl<T>(name, readFunction)))
{}

template class PVDelegateIn<std::int32_t>;
template class PVDelegateIn<double>;
template class PVDelegateIn<std::vector<std::int8_t> >;
template class PVDelegateIn<std::vector<std::uint8_t> >;
template class PVDelegateIn<std::vector<std::int32_t> >;
template class PVDelegateIn<std::vector<double> >;
template class PVDelegateIn<std::string>;


}

