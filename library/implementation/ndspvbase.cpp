#include "../include/nds3/ndspvbase.h"
#include "ndspvbaseimpl.h"

namespace nds
{

PVBase::PVBase(std::shared_ptr<PVBaseImpl> pvImpl): Base(std::static_pointer_cast<BaseImpl>(pvImpl))
{

}

void PVBase::read(timespec* pTimestamp, std::int32_t* pValue)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->read(pTimestamp, pValue);
}

void PVBase::write(const timespec& timestamp, const std::int32_t& value)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->write(timestamp, value);
}

void PVBase::setType(const std::string& type)
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

}
