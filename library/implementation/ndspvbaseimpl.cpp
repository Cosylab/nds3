#include "ndspvbaseimpl.h"
#include "ndsportImpl.h"
#include "../include/nds3/ndsport.h"
#include "../include/nds3/ndspvbase.h"

namespace nds
{

PVBaseImpl::PVBaseImpl(const std::string& name, PVBase* pInterface): BaseImpl(name, (Base*)pInterface)
{

}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::int32_t* /* pValue */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::int32_t& /* value */)
{
    throw;
}

void PVBaseImpl::initialize()
{
    getPort().registerPV(*(static_cast<PVBase*>(m_pInterfaceObject)));
}

}
