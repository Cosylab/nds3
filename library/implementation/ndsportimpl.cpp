#include "ndsportImpl.h"
#include "ndsinterfaceimpl.h"
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
    m_pAsynInterface.reset(new InterfaceImpl(getFullName()));
    NodeImpl::initialize();
}

void PortImpl::registerPV(std::shared_ptr<PVBaseImpl> pv)
{
    m_pAsynInterface->registerPV(pv);
}

}
