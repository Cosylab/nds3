/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "../include/nds3/pvDelegateOut.h"
#include "../include/nds3impl/pvDelegateOutImpl.h"

#include <cstdint>
#include <vector>

namespace nds
{

/*
 * Constructor (with initialization function)
 *
 ********************************************/
template <typename T>
PVDelegateOut<T>::PVDelegateOut(const std::string& name, write_t writeFunction, initValue_t initValueFunction):
    PVBaseOut(std::shared_ptr<PVBaseOutImpl>(new PVDelegateOutImpl<T>(name, writeFunction, initValueFunction)))
{}


/*
 * Constructor (without initialization function)
 *
 ***********************************************/
template <typename T>
PVDelegateOut<T>::PVDelegateOut(const std::string& name, write_t writeFunction):
    PVBaseOut(std::shared_ptr<PVBaseOutImpl>(new PVDelegateOutImpl<T>(name, writeFunction)))
{}


// Instantiate all the needed data types
////////////////////////////////////////
template class PVDelegateOut<std::int32_t>;
template class PVDelegateOut<double>;
template class PVDelegateOut<std::vector<std::int8_t> >;
template class PVDelegateOut<std::vector<std::uint8_t> >;
template class PVDelegateOut<std::vector<std::int32_t> >;
template class PVDelegateOut<std::vector<double> >;
template class PVDelegateOut<std::string>;


}

