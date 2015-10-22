#include "../include/nds3impl/pvBaseImpl.h"
#include "../include/nds3impl/portImpl.h"
#include "../include/nds3/port.h"
#include "../include/nds3/pvBase.h"

namespace nds
{

PVBaseImpl::PVBaseImpl(const std::string& name): BaseImpl(name),
    m_scanType(scanType_t::passive),
    m_periodicScanSeconds(1),
    m_maxElements(1)
{

}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::int32_t* /* pValue */) const
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, double* /* pValue */) const
{
    throw;
}

void PVBaseImpl::read(timespec* pTimestamp, std::vector<std::int8_t>* pValue) const
{
    // TODO
    // Epics calls this also for unsigned-int.
    // If we arrive here maybe we really wanted to call the unsigned int function.
    // This is as ugly as it can get: consider modifying this
    read(pTimestamp, (std::vector<std::uint8_t>*) pValue);
}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::vector<std::uint8_t>* /* pValue */) const
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::vector<std::int32_t>* /* pValue */) const
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::vector<double>* /* pValue */) const
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::string* /* pValue */) const
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::int32_t& /* value */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const double& /* value */)
{
    throw;
}

void PVBaseImpl::write(const timespec& pTimestamp, const std::vector<std::int8_t>& value)
{
    // TODO
    // Epics calls this also for unsigned-int.
    // If we arrive here maybe we really wanted to call the unsigned int function.
    // This is as ugly as it can get: consider modifying this
    write(pTimestamp, (const std::vector<std::uint8_t>&) value);
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

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::string& /* value */)
{
    throw;
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

const enumerationStrings_t& PVBaseImpl::getEnumerations() const
{
    return m_enumeration;
}

void PVBaseImpl::initialize(FactoryBaseImpl& controlSystem)
{
    BaseImpl::initialize(controlSystem);
    getPort()->registerPV(std::static_pointer_cast<PVBaseImpl>(shared_from_this()));
}

void PVBaseImpl::deinitialize()
{
    getPort()->deregisterPV(std::static_pointer_cast<PVBaseImpl>(shared_from_this()));
}



}
