#include "../include/nds3/pvDelegate.h"
#include "pvDelegateImpl.h"

#include <cstdint>
#include <vector>

namespace nds
{

template <typename T>
PVDelegate<T>::PVDelegate(const std::string& name, read_t readFunction, write_t writeFunction):
    PVBase(std::shared_ptr<PVBaseImpl>(new PVDelegateImpl<T>(name, readFunction, writeFunction)))
{}

template <typename T>
PVDelegate<T>::PVDelegate(const std::string& name, read_t readFunction):
    PVBase(std::shared_ptr<PVBaseImpl>(new PVDelegateImpl<T>(name, readFunction)))
{}

template class PVDelegate<std::int32_t>;
template class PVDelegate<double>;
template class PVDelegate<std::vector<std::int32_t> >;


}
