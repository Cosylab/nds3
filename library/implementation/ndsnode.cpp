#include "../include/nds3/ndsnode.h"
#include "ndsnodeimpl.h"

namespace nds
{

Node::Node()
{

}

Node::Node(const std::string &name)
{
    m_pImplementation.reset(new NodeImpl(name, this));
}

void Node::addChildInternal(Base* pChild)
{
    static_cast<NodeImpl*>(m_pImplementation.get())->addChild(pChild);
}

}
