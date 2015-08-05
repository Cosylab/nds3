#include "../include/nds3/ndspvbase.h"
#include "ndspvbaseimpl.h"

namespace nds
{

PVBase::PVBase(const std::string &name)
{
    m_pImplementation.reset(new PVBaseImpl(name, this));
}

void PVBase::read(timespec* pTimestamp, std::int32_t* pValue)
{
    static_cast<PVBaseImpl*>(m_pImplementation.get())->read(pTimestamp, pValue);
}

void PVBase::write(const timespec& timestamp, const std::int32_t& value)
{
    static_cast<PVBaseImpl*>(m_pImplementation.get())->write(timestamp, value);
}

}
