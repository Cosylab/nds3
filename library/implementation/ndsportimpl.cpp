#include "ndsportImpl.h"
#include "ndsinterfaceimpl.h"
#include "ndspvbaseimpl.h"
#include "../include/nds3/ndsbase.h"
#include "../include/nds3/ndsport.h"

namespace nds
{

PortImpl::PortImpl(const std::string& name, Port* pInterface): NodeImpl(name, (Node*)pInterface)
{
}

Port& PortImpl::getPort()
{
    return *(dynamic_cast<Port*>(m_pInterfaceObject));
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

void PortImpl::registerPV(PVBase& pv)
{
    m_pAsynInterface->registerPV(pv);
}

}
