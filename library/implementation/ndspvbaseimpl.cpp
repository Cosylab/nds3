#include "ndspvbaseimpl.h"
#include "ndsportImpl.h"
#include "../include/nds3/ndsport.h"
#include "../include/nds3/ndspvbase.h"

namespace nds
{

PVBaseImpl::PVBaseImpl(const std::string& name): BaseImpl(name),
    m_type(notSpecified),
    m_scanType(passive),
    m_periodicScanSeconds(1),
    m_maxElements(1)
{

}

template<typename T>
void PVBaseImpl::read(timespec* /* pTimestamp */, T* /* pValue */)
{
    throw;
}

template<typename T>
void PVBaseImpl::write(const timespec& /* pTimestamp */, const T& /* value */)
{
    throw;
}

template void PVBaseImpl::read<std::int32_t>(timespec*, std::int32_t*);
template void PVBaseImpl::write<std::int32_t>(const timespec&, const std::int32_t&);

template void PVBaseImpl::read<std::vector<std::int32_t> >(timespec*, std::vector<std::int32_t>*);
template void PVBaseImpl::write<std::vector<std::int32_t> >(const timespec&, const std::vector<std::int32_t>&);

template void PVBaseImpl::read<double>(timespec*, double*);
template void PVBaseImpl::write<double>(const timespec&, const double&);


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


void PVBaseImpl::initialize()
{
    getPort()->registerPV(std::static_pointer_cast<PVBaseImpl>(shared_from_this()));
}


}
