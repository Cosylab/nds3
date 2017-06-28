/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include <type_traits>

#include "nds3/impl/pvDelegateInImpl.h"

namespace nds
{

/*
 * Constructor
 *
 *************/
template <typename T>
PVDelegateInImpl<T>::PVDelegateInImpl(const std::string& name, read_t readFunction, const inputPvType_t pvType): PVBaseInImpl(name, pvType),
    m_reader(readFunction)
{}


/*
 * Called when the control system wants to read data
 *
 ***************************************************/
template <typename T>
void PVDelegateInImpl<T>::read(timespec* pTimestamp, T* pValue) const
{
    m_reader(pTimestamp, pValue);
}


/*
 * Returns the data type
 *
 ***********************/
template <typename T>
dataType_t PVDelegateInImpl<T>::getDataType() const
{
    return getDataTypeForCPPType<T>();
}


// Instantiate all the needed data types
////////////////////////////////////////
template class PVDelegateInImpl<std::int32_t>;
template class PVDelegateInImpl<double>;
template class PVDelegateInImpl<std::vector<std::int8_t> >;
template class PVDelegateInImpl<std::vector<std::uint8_t> >;
template class PVDelegateInImpl<std::vector<std::int32_t> >;
template class PVDelegateInImpl<std::vector<double> >;
template class PVDelegateInImpl<std::string>;

}


