#include "../include/nds3/ndspvdelegate.h"
#include "ndspvdelegateimpl.h"

namespace nds
{

template <typename T>
PVDelegate<T>::PVDelegate(const std::string& name, read_t readFunction, write_t writeFunction):
    PVBase(std::shared_ptr<PVBaseImpl>(new PVDelegateImpl<T>(name, readFunction, writeFunction)))
{}


template class PVDelegate<std::int32_t>;


}
