#include "ndspvholddelegateimpl.h"
#include "../include/nds3/ndsdelegate.h"

namespace nds
{

PVHoldDelegateImpl::PVHoldDelegateImpl(const std::string &name,  Delegate* pDelegate):
    PVBaseImpl(name), m_pDelegate(pDelegate)
{

}

void PVHoldDelegateImpl::read(timespec* pTimestamp, std::int32_t* pValue)
{
    m_pDelegate->readInt32(pTimestamp, pValue);
}

void PVHoldDelegateImpl::write(const timespec& timestamp, const std::int32_t& value)
{
    m_pDelegate->writeInt32(timestamp, value);
}

}
