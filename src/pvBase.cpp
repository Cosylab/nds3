#include "../include/nds3/pvBase.h"
#include "pvBaseImpl.h"

namespace nds
{

PVBase::PVBase()
{

}

PVBase::PVBase(std::shared_ptr<PVBaseImpl> pvImpl): Base(std::static_pointer_cast<BaseImpl>(pvImpl))
{

}

void PVBase::setEnumeration(const enumerationStrings_t &enumerations)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->setEnumeration(enumerations);
}

template<typename T>
void PVBase::read(timespec* pTimestamp, T* pValue)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->read(pTimestamp, pValue);
}

template<typename T>
void PVBase::write(const timespec& timestamp, const T& value)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->write(timestamp, value);
}

template<typename T>
void PVBase::push(const timespec& timestamp, const T& value)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->push(timestamp, value);
}

template void PVBase::read<std::int8_t>(timespec*, std::int8_t*);
template void PVBase::write<std::int8_t>(const timespec&, const std::int8_t&);
template void PVBase::push<std::int8_t>(const timespec&, const std::int8_t&);

template void PVBase::read<std::uint8_t>(timespec*, std::uint8_t*);
template void PVBase::write<std::uint8_t>(const timespec&, const std::uint8_t&);
template void PVBase::push<std::uint8_t>(const timespec&, const std::uint8_t&);


template void PVBase::read<std::int32_t>(timespec*, std::int32_t*);
template void PVBase::write<std::int32_t>(const timespec&, const std::int32_t&);
template void PVBase::push<std::int32_t>(const timespec&, const std::int32_t&);

template void PVBase::read<std::uint32_t>(timespec*, std::uint32_t*);
template void PVBase::write<std::uint32_t>(const timespec&, const std::uint32_t&);
template void PVBase::push<std::uint32_t>(const timespec&, const std::uint32_t&);

template void PVBase::read<double>(timespec*, double*);
template void PVBase::write<double>(const timespec&, const double&);
template void PVBase::push<double>(const timespec&, const double&);

template void PVBase::read<std::vector<std::int8_t> >(timespec*, std::vector<std::int8_t>*);
template void PVBase::write<std::vector<std::int8_t> >(const timespec&, const std::vector<std::int8_t>&);
template void PVBase::push<std::vector<std::int8_t> >(const timespec&, const std::vector<std::int8_t>&);

template void PVBase::read<std::vector<std::uint8_t> >(timespec*, std::vector<std::uint8_t>*);
template void PVBase::write<std::vector<std::uint8_t> >(const timespec&, const std::vector<std::uint8_t>&);
template void PVBase::push<std::vector<std::uint8_t> >(const timespec&, const std::vector<std::uint8_t>&);

template void PVBase::read<std::vector<std::int32_t> >(timespec*, std::vector<std::int32_t>*);
template void PVBase::write<std::vector<std::int32_t> >(const timespec&, const std::vector<std::int32_t>&);
template void PVBase::push<std::vector<std::int32_t> >(const timespec&, const std::vector<std::int32_t>&);

template void PVBase::read<std::vector<double> >(timespec*, std::vector<double>*);
template void PVBase::write<std::vector<double> >(const timespec&, const std::vector<double>&);
template void PVBase::push<std::vector<double> >(const timespec&, const std::vector<double>&);

void PVBase::setType(const recordType_t type)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->setType(type);
}

void PVBase::setDescription(const std::string& description)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->setDescription(description);
}

void PVBase::setInterfaceName(const std::string& interfaceName)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->setInterfaceName(interfaceName);
}

void PVBase::setScanType(const scanType_t scanType, const double periodSeconds)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->setScanType(scanType, periodSeconds);
}

void PVBase::setMaxElements(const size_t maxElements)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->setMaxElements(maxElements);
}

}