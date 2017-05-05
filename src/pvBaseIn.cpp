/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/pvBaseIn.h"
#include "nds3/impl/pvBaseInImpl.h"

namespace nds
{

PVBaseIn::PVBaseIn()
{

}

PVBaseIn::PVBaseIn(std::shared_ptr<PVBaseInImpl> pvImpl): PVBase(std::static_pointer_cast<PVBaseImpl>(pvImpl))
{
}

template<typename T>
void PVBaseIn::read(timespec* pTimestamp, T* pValue) const
{
    std::static_pointer_cast<PVBaseInImpl>(m_pImplementation)->read(pTimestamp, pValue);
}

template<typename T>
void PVBaseIn::push(const timespec& timestamp, const T& value)
{
    std::static_pointer_cast<PVBaseInImpl>(m_pImplementation)->push(timestamp, value);
}

void PVBaseIn::setDecimation(const std::uint32_t decimation)
{
    std::static_pointer_cast<PVBaseInImpl>(m_pImplementation)->setDecimation(decimation);
}

void PVBaseIn::replicateFrom(const std::string &sourceInputPVName)
{
    std::static_pointer_cast<PVBaseInImpl>(m_pImplementation)->replicateFrom(sourceInputPVName);
}

template void PVBaseIn::read<std::int32_t>(timespec*, std::int32_t*) const;
template void PVBaseIn::push<std::int32_t>(const timespec&, const std::int32_t&);

template void PVBaseIn::read<double>(timespec*, double*) const;
template void PVBaseIn::push<double>(const timespec&, const double&);

template void PVBaseIn::read<std::vector<std::int8_t> >(timespec*, std::vector<std::int8_t>*) const;
template void PVBaseIn::push<std::vector<std::int8_t> >(const timespec&, const std::vector<std::int8_t>&);

template void PVBaseIn::read<std::vector<std::uint8_t> >(timespec*, std::vector<std::uint8_t>*) const;
template void PVBaseIn::push<std::vector<std::uint8_t> >(const timespec&, const std::vector<std::uint8_t>&);

template void PVBaseIn::read<std::vector<std::int32_t> >(timespec*, std::vector<std::int32_t>*) const;
template void PVBaseIn::push<std::vector<std::int32_t> >(const timespec&, const std::vector<std::int32_t>&);

template void PVBaseIn::read<std::vector<double> >(timespec*, std::vector<double>*) const;
template void PVBaseIn::push<std::vector<double> >(const timespec&, const std::vector<double>&);

template void PVBaseIn::read<std::string >(timespec*, std::string*) const;
template void PVBaseIn::push<std::string >(const timespec&, const std::string&);

}

