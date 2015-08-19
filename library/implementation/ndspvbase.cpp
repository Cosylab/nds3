#include "../include/nds3/ndspvbase.h"
#include "ndspvbaseimpl.h"

namespace nds
{

PVBase::PVBase(const std::string &name): Base(std::shared_ptr<BaseImpl>(new PVBaseImpl(name)))
{
}

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

}
