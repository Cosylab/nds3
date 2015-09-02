#include "ndspvholddelegateimpl.h"
#include "../include/nds3/ndsdelegate.h"

namespace nds
{

PVHoldDelegateImpl::PVHoldDelegateImpl(const std::string &name, dataType_t dataType, std::shared_ptr<Delegate> pDelegate):
    PVBaseImpl(name), m_pDelegate(pDelegate), m_dataType(dataType)
{

}

void PVHoldDelegateImpl::read(timespec* pTimestamp, std::int32_t* pValue)
{
    m_pDelegate->readInt32(getFullNameFromPort(), pTimestamp, pValue);
}

void PVHoldDelegateImpl::write(const timespec& timestamp, const std::int32_t& value)
{
    m_pDelegate->writeInt32(getFullNameFromPort(), timestamp, value);
}

dataType_t PVHoldDelegateImpl::getDataType()
{
    return m_dataType;
}


}
