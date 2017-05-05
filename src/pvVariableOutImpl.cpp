/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/impl/pvVariableOutImpl.h"

namespace nds
{

/*
 * Constructor
 *
 *************/
template <typename T>
PVVariableOutImpl<T>::PVVariableOutImpl(const std::string& name, const outputPvType_t pvType): PVBaseOutImpl(name, pvType), m_value()
{
    m_timestamp.tv_sec = 0;
    m_timestamp.tv_nsec = 0;

}


/*
 * Called when the control system wants to read the value stored in the PV
 *
 *************************************************************************/
template <typename T>
void PVVariableOutImpl<T>::read(timespec* pTimestamp, T* pValue) const
{
    std::unique_lock<std::mutex> lock(m_pvMutex);
    *pValue = m_value;
    *pTimestamp = m_timestamp;
}


/*
 * Called when the control system wants to write a value into the PV
 *
 *******************************************************************/
template <typename T>
void PVVariableOutImpl<T>::write(const timespec& timestamp, const T& value)
{
    std::unique_lock<std::mutex> lock(m_pvMutex);
    m_value =value;
    m_timestamp = timestamp;
}


/*
 * Returns the PV data type
 *
 **************************/
template <typename T>
dataType_t PVVariableOutImpl<T>::getDataType() const
{
    return getDataTypeForCPPType<T>();
}


/*
 * Return the value stored in the PV
 *
 ***********************************/
template <typename T>
T PVVariableOutImpl<T>::getValue() const
{
    std::unique_lock<std::mutex> lock(m_pvMutex);
    return m_value;
}


/*
 * Return the value and timestamp stored in the PV
 *
 *************************************************/
template <typename T>
void PVVariableOutImpl<T>::getValue(timespec* pTime, T* pValue) const
{
    std::unique_lock<std::mutex> lock(m_pvMutex);
    *pValue = m_value;
    *pTime = m_timestamp;
}


// Instantiate all the needed data types
////////////////////////////////////////
template class PVVariableOutImpl<std::int32_t>;
template class PVVariableOutImpl<double>;
template class PVVariableOutImpl<std::vector<std::int8_t> >;
template class PVVariableOutImpl<std::vector<std::uint8_t> >;
template class PVVariableOutImpl<std::vector<std::int32_t> >;
template class PVVariableOutImpl<std::vector<double> >;
template class PVVariableOutImpl<std::string>;

}

