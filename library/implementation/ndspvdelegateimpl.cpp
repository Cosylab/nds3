#include "ndspvdelegateimpl.h"
#include <type_traits>
namespace nds
{

template <typename T>
dataType_t PVDelegateImpl<T>::getDataType()
{
    if(std::is_same<T, std::int32_t>::value)
    {
        return dataInt32;
    }
    if(std::is_same<T, double>::value)
    {
        return dataFloat64;
    }
    if(std::is_same<T, std::vector<std::int32_t> >::value)
    {
        return dataInt32Array;
    }

}

template class PVDelegateImpl<std::int32_t>;
template class PVDelegateImpl<double>;
template class PVDelegateImpl<std::vector<std::int32_t> >;

}

