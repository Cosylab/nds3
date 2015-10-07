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
