#include "ndsportImpl.h"
#include "ndsepicsinterfaceimpl.h"
#include "ndspvbaseimpl.h"

namespace nds
{

PortImpl::PortImpl(const std::string& name): NodeImpl(name)
{
}

PortImpl::~PortImpl()
{
}



std::shared_ptr<PortImpl> PortImpl::getPort()
{
    return std::static_pointer_cast<PortImpl>(shared_from_this());
}

std::string PortImpl::getFullNameFromPort() const
{
    return "";
}

void PortImpl::initialize()
{
    if(m_pInterface.get() == 0)
    {
        m_pInterface.reset(new EpicsInterfaceImpl(getFullName()));
    }
    NodeImpl::initialize();

    m_pInterface->registrationTerminated();
}

void PortImpl::registerPV(std::shared_ptr<PVBaseImpl> pv)
{
    m_pInterface->registerPV(pv);
}

}
