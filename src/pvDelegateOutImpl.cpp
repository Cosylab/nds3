/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include <type_traits>

#include "nds3/impl/pvDelegateOutImpl.h"

namespace nds
{

/*
 * Constructor (with initialization function)
 *
 ********************************************/
template <typename T>
PVDelegateOutImpl<T>::PVDelegateOutImpl(const std::string& name, write_t writeFunction, initValue_t initValueFunction, outputPvType_t pvType): PVBaseOutImpl(name, pvType),
    m_writer(writeFunction),
    m_initializer(initValueFunction)
{
    // Since we have a read method that is used to read the initial value then we
    //  set the "ProcessAtInit" flag
    /////////////////////////////////////////////////////////////////////////////
    processAtInit(true);
}


/*
 * Constructor (without initialization function)
 *
 ***********************************************/
template <typename T>
PVDelegateOutImpl<T>::PVDelegateOutImpl(const std::string& name, write_t writeFunction, const outputPvType_t pvType): PVBaseOutImpl(name, pvType),
    m_writer(writeFunction),
    m_initializer(std::bind(&PVDelegateOutImpl::dontInitialize, this, std::placeholders::_1, std::placeholders::_2))
{
    processAtInit(false);
}


/*
 * Called to read the initial value
 *
 **********************************/
template <typename T>
void PVDelegateOutImpl<T>::read(timespec* pTimestamp, T* pValue) const
{
    m_initializer(pTimestamp, pValue);
}


/*
 * Called to write a value in the PV
 *
 ***********************************/
template <typename T>
void PVDelegateOutImpl<T>::write(const timespec& timestamp, const T& value)
{
    m_writer(timestamp, value);
}


/*
 * Return the PV's data type
 *
 ***************************/
template <typename T>
dataType_t PVDelegateOutImpl<T>::getDataType() const
{
    return getDataTypeForCPPType<T>();
}


/*
 * Called to read a value when the reading function has not been declared
 *
 ************************************************************************/
template <typename T>
void PVDelegateOutImpl<T>::dontInitialize(timespec*, T*)
{

}


// Instantiate all the needed data types
////////////////////////////////////////
template class PVDelegateOutImpl<std::int32_t>;
template class PVDelegateOutImpl<double>;
template class PVDelegateOutImpl<std::vector<std::int8_t> >;
template class PVDelegateOutImpl<std::vector<std::uint8_t> >;
template class PVDelegateOutImpl<std::vector<std::int32_t> >;
template class PVDelegateOutImpl<std::vector<double> >;
template class PVDelegateOutImpl<std::string>;

}

