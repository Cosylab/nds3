/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include <cstdint>
#include <vector>

#include "nds3/pvDelegateIn.h"
#include "nds3/impl/pvDelegateInImpl.h"

namespace nds
{

/*
 * Constructor
 *
 *************/
template <typename T>
PVDelegateIn<T>::PVDelegateIn(const std::string& name, read_t readFunction):
    PVBaseIn(std::shared_ptr<PVBaseInImpl>(new PVDelegateInImpl<T>(name, readFunction)))
{}


// Instantiate all the needed data types
////////////////////////////////////////
template class PVDelegateIn<std::int32_t>;
template class PVDelegateIn<double>;
template class PVDelegateIn<std::vector<std::int8_t> >;
template class PVDelegateIn<std::vector<std::uint8_t> >;
template class PVDelegateIn<std::vector<std::int32_t> >;
template class PVDelegateIn<std::vector<double> >;
template class PVDelegateIn<std::string>;


}

