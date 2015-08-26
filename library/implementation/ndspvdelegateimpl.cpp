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

}

template class PVDelegateImpl<std::int32_t>;
template class PVDelegateImpl<double>;

}

