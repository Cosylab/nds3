#include "pvBaseImpl.h"
#include "portImpl.h"
#include "../include/nds3/port.h"
#include "../include/nds3/pvBase.h"

namespace nds
{

PVBaseImpl::PVBaseImpl(const std::string& name): BaseImpl(name),
    m_type(recordType_t::notSpecified),
    m_scanType(scanType_t::passive),
    m_periodicScanSeconds(1),
    m_maxElements(1)
{

}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::int8_t* /* pValue */)
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::uint8_t* /* pValue */)
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::int32_t* /* pValue */)
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::uint32_t* /* pValue */)
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, double* /* pValue */)
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::vector<std::int8_t>* /* pValue */)
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::vector<std::uint8_t>* /* pValue */)
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::vector<std::int32_t>* /* pValue */)
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::vector<double>* /* pValue */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::int8_t& /* value */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::uint8_t& /* value */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::int32_t& /* value */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::uint32_t& /* value */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const double& /* value */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::vector<std::int8_t>& /* value */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::vector<std::uint8_t>& /* value */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::vector<std::int32_t>& /* value */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::vector<double>& /* value */)
{
    throw;
}


template<typename T>
void PVBaseImpl::push(const timespec& timestamp, const T& value)
{
    // Find the port then push the value
    ////////////////////////////////////
    std::shared_ptr<PortImpl> pPort(getPort());
    pPort->push(std::static_pointer_cast<PVBaseImpl>(shared_from_this()), timestamp, value);
}

template void PVBaseImpl::push<std::int8_t>(const timespec&, const std::int8_t&);
template void PVBaseImpl::push<std::uint8_t>(const timespec&, const std::uint8_t&);
template void PVBaseImpl::push<std::int32_t>(const timespec&, const std::int32_t&);
template void PVBaseImpl::push<std::uint32_t>(const timespec&, const std::uint32_t&);
template void PVBaseImpl::push<double>(const timespec&, const double&);
template void PVBaseImpl::push<std::vector<std::int8_t> >(const timespec&, const std::vector<std::int8_t>&);
template void PVBaseImpl::push<std::vector<std::uint8_t> >(const timespec&, const std::vector<std::uint8_t>&);
template void PVBaseImpl::push<std::vector<std::int32_t> >(const timespec&, const std::vector<std::int32_t>&);
template void PVBaseImpl::push<std::vector<double> >(const timespec&, const std::vector<double>&);

void PVBaseImpl::setType(const recordType_t type)
{
    m_type = type;
}

void PVBaseImpl::setDescription(const std::string& description)
{
    m_description = description;
}

void PVBaseImpl::setInterfaceName(const std::string& interfaceName)
{
    m_interfaceName = interfaceName;
}

void PVBaseImpl::setScanType(const scanType_t scanType, const double periodSeconds)
{
    m_scanType = scanType;
    m_periodicScanSeconds = periodSeconds;
}

void PVBaseImpl::setMaxElements(const size_t maxElements)
{
    m_maxElements = maxElements;
}

void PVBaseImpl::setEnumeration(const enumerationStrings_t &enumerations)
{
    m_enumeration = enumerations;
}

recordType_t PVBaseImpl::getType() const
{
    return m_type;
}

std::string PVBaseImpl::getDescription() const
{
    return m_description;
}

std::string PVBaseImpl::getInterfaceName() const
{
    return m_interfaceName;
}

scanType_t PVBaseImpl::getScanType() const
{
    return m_scanType;
}

double PVBaseImpl::getScanPeriodSeconds() const
{
    return m_periodicScanSeconds;
}

size_t PVBaseImpl::getMaxElements() const
{
    return m_maxElements;
}

const enumerationStrings_t& PVBaseImpl::getEnumerations()
{
    return m_enumeration;
}

void PVBaseImpl::initialize(FactoryBaseImpl& /* controlSystem */)
{
    getPort()->registerPV(std::static_pointer_cast<PVBaseImpl>(shared_from_this()));
}

void PVBaseImpl::deinitialize()
{
    getPort()->deregisterPV(std::static_pointer_cast<PVBaseImpl>(shared_from_this()));
}



}
