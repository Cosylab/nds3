/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "../include/nds3/pvBaseOut.h"
#include "../include/nds3impl/pvBaseOutImpl.h"

namespace nds
{

PVBaseOut::PVBaseOut()
{
}

PVBaseOut::PVBaseOut(std::shared_ptr<PVBaseOutImpl> pvImpl): PVBase(std::static_pointer_cast<PVBaseImpl>(pvImpl))
{
}

void PVBaseOut::subscribeTo(const std::string &inputPVName)
{
    std::static_pointer_cast<PVBaseOutImpl>(m_pImplementation)->subscribeTo(inputPVName);
}

template<typename T>
void PVBaseOut::read(timespec* pTimestamp, T* pValue) const
{
    std::static_pointer_cast<PVBaseOutImpl>(m_pImplementation)->read(pTimestamp, pValue);
}

template<typename T>
void PVBaseOut::write(const timespec& timestamp, const T& value)
{
    std::static_pointer_cast<PVBaseOutImpl>(m_pImplementation)->write(timestamp, value);
}

template void PVBaseOut::read<std::int32_t>(timespec*, std::int32_t*) const;
template void PVBaseOut::write<std::int32_t>(const timespec&, const std::int32_t&);

template void PVBaseOut::read<double>(timespec*, double*) const;
template void PVBaseOut::write<double>(const timespec&, const double&);

template void PVBaseOut::read<std::vector<std::int8_t> >(timespec*, std::vector<std::int8_t>*) const;
template void PVBaseOut::write<std::vector<std::int8_t> >(const timespec&, const std::vector<std::int8_t>&);

template void PVBaseOut::read<std::vector<std::uint8_t> >(timespec*, std::vector<std::uint8_t>*) const;
template void PVBaseOut::write<std::vector<std::uint8_t> >(const timespec&, const std::vector<std::uint8_t>&);

template void PVBaseOut::read<std::vector<std::int32_t> >(timespec*, std::vector<std::int32_t>*) const;
template void PVBaseOut::write<std::vector<std::int32_t> >(const timespec&, const std::vector<std::int32_t>&);

template void PVBaseOut::read<std::vector<double> >(timespec*, std::vector<double>*) const;
template void PVBaseOut::write<std::vector<double> >(const timespec&, const std::vector<double>&);

template void PVBaseOut::read<std::string >(timespec*, std::string*) const;
template void PVBaseOut::write<std::string >(const timespec&, const std::string&);

}

