#include "../include/nds3/ndsport.h"
#include "../include/nds3/ndspvbase.h"
#include "ndsportImpl.h"

namespace nds
{

Port::Port(std::shared_ptr<PortImpl> portImpl): Node(std::static_pointer_cast<NodeImpl>(portImpl))
{
}

Port::Port(const std::string &name): Node(std::shared_ptr<NodeImpl>(new PortImpl(name)))
{
}

}
