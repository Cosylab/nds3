/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2016 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "../include/nds3/pvVariableOut.h"
#include "../include/nds3impl/pvVariableOutImpl.h"

#include <cstdint>
#include <vector>

namespace nds
{

/*
 * Default constructor
 *
 *********************/
template <typename T>
PVVariableOut<T>::PVVariableOut()
{

}


/*
 * Construct a PVVariableOut
 *
 ***************************/
template <typename T>
PVVariableOut<T>::PVVariableOut(const std::string& name):
    PVBaseOut(std::shared_ptr<PVBaseOutImpl>(new PVVariableOutImpl<T>(name)))
{}


/*
 * Return the value stored in the PV
 *
 ***********************************/
template <typename T>
T PVVariableOut<T>::getValue() const
{
    return std::static_pointer_cast<PVVariableOutImpl<T> >(m_pImplementation)->getValue();
}


/*
 * Return the value and timestamp stored in the PV
 *
 *************************************************/
template <typename T>
void PVVariableOut<T>::getValue(timespec* pTime, T* pValue) const
{
    std::static_pointer_cast<PVVariableOutImpl<T> >(m_pImplementation)->getValue(pTime, pValue);
}

// Instantiate all the needed data types
////////////////////////////////////////
template class PVVariableOut<std::int32_t>;
template class PVVariableOut<double>;
template class PVVariableOut<std::vector<std::int8_t> >;
template class PVVariableOut<std::vector<std::uint8_t> >;
template class PVVariableOut<std::vector<std::int32_t> >;
template class PVVariableOut<std::vector<double> >;
template class PVVariableOut<std::string>;


}


