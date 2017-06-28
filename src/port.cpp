/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/port.h"
#include "nds3/pvBase.h"
#include "nds3/factory.h"
#include "nds3/impl/portImpl.h"

namespace nds
{

Port::Port(std::shared_ptr<PortImpl> portImpl): Node(std::static_pointer_cast<NodeImpl>(portImpl))
{
}

Port::Port(const std::string &name, const nodeType_t nodeType):
    Node(std::shared_ptr<NodeImpl>(new PortImpl(name, nodeType)))
{
}

}
