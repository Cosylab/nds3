#include "pvHoldDelegateImpl.h"
#include "../include/nds3/delegate.h"

namespace nds
{

PVHoldDelegateImpl::PVHoldDelegateImpl(const std::string &name, dataType_t dataType, std::shared_ptr<Delegate> pDelegate, dataDirection_t dataDirection):
    PVBaseImpl(name), m_pDelegate(pDelegate), m_dataType(dataType), m_dataDirection(dataDirection)
{

}

void PVHoldDelegateImpl::read(timespec* pTimestamp, std::int32_t* pValue) const
{
    m_pDelegate->readInt32(getFullNameFromPort(), pTimestamp, pValue);
}

void PVHoldDelegateImpl::write(const timespec& timestamp, const std::int32_t& value)
{
    m_pDelegate->writeInt32(getFullNameFromPort(), timestamp, value);
}

dataType_t PVHoldDelegateImpl::getDataType() const
{
    return m_dataType;
}

dataDirection_t PVHoldDelegateImpl::getDataDirection() const
{
    return m_dataDirection;
}

}
