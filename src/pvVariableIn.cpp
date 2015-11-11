/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "../include/nds3/pvVariableIn.h"
#include "../include/nds3impl/pvVariableInImpl.h"

#include <cstdint>
#include <vector>

namespace nds
{

/*
 * Default constructor
 *
 *********************/
template <typename T>
PVVariableIn<T>::PVVariableIn()
{

}


/*
 * Constructor
 *
 *************/
template <typename T>
PVVariableIn<T>::PVVariableIn(const std::string& name):
    PVBaseIn(std::shared_ptr<PVBaseInImpl>(new PVVariableInImpl<T>(name)))
{}


/*
 * Store a value in the PV
 *
 *************************/
template <typename T>
void PVVariableIn<T>::setValue(const T& value)
{
    return std::static_pointer_cast<PVVariableInImpl<T> >(m_pImplementation)->setValue(value);
}


/*
 * Store a value and its timestamp in the PV
 *
 *******************************************/
template <typename T>
void PVVariableIn<T>::setValue(const timespec& timestamp, const T& value)
{
    return std::static_pointer_cast<PVVariableInImpl<T> >(m_pImplementation)->setValue(timestamp, value);
}


// Instantiate all the needed data types
////////////////////////////////////////
template class PVVariableIn<std::int32_t>;
template class PVVariableIn<double>;
template class PVVariableIn<std::vector<std::int8_t> >;
template class PVVariableIn<std::vector<std::uint8_t> >;
template class PVVariableIn<std::vector<std::int32_t> >;
template class PVVariableIn<std::vector<double> >;
template class PVVariableIn<std::string>;


}



