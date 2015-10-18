#include "../include/nds3/pvDelegateOut.h"
#include "../include/nds3impl/pvDelegateOutImpl.h"

#include <cstdint>
#include <vector>

namespace nds
{

template <typename T>
PVDelegateOut<T>::PVDelegateOut(const std::string& name, write_t writeFunction, initValue_t initValueFunction):
    PVBaseOut(std::shared_ptr<PVBaseOutImpl>(new PVDelegateOutImpl<T>(name, writeFunction, initValueFunction)))
{}


template class PVDelegateOut<std::int32_t>;
template class PVDelegateOut<double>;
template class PVDelegateOut<std::vector<std::int8_t> >;
template class PVDelegateOut<std::vector<std::uint8_t> >;
template class PVDelegateOut<std::vector<std::int32_t> >;
template class PVDelegateOut<std::vector<double> >;


}

