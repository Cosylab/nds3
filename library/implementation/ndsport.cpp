#include "../include/nds3/ndsport.h"
#include "../include/nds3/ndspvbase.h"
#include "../include/nds3/ndsfactory.h"
#include "ndsportImpl.h"

namespace nds
{

Port::Port(std::shared_ptr<PortImpl> portImpl): Node(std::static_pointer_cast<NodeImpl>(portImpl))
{
}

Port::Port(const std::string &name, const controlSystem_t controlSystem /*  = controlSystem_t::default */):
    Node(std::shared_ptr<NodeImpl>(new PortImpl(name, *(Factory::Factory(controlSystem).m_pFactory))))
{
}

}
