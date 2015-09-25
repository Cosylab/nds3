#include "../include/nds3/port.h"
#include "../include/nds3/pvBase.h"
#include "../include/nds3/factory.h"
#include "portImpl.h"

namespace nds
{

Port::Port(std::shared_ptr<PortImpl> portImpl): Node(std::static_pointer_cast<NodeImpl>(portImpl))
{
}

Port::Port(const std::string &name):
    Node(std::shared_ptr<NodeImpl>(new PortImpl(name)))
{
}

}
