#include "../include/nds3/ndsnode.h"
#include "../include/nds3/ndspvbase.h"

#include "ndsnodeimpl.h"

namespace nds
{

Node::Node(std::shared_ptr<NodeImpl> nodeImpl): Base(std::static_pointer_cast<BaseImpl>(nodeImpl))
{
}

Node::Node(const std::string &name): Base(std::shared_ptr<BaseImpl>(new NodeImpl(name)))
{
}

void Node::addNode(Node& node)
{
    addChildInternal(node);
}

void Node::addPV(PVBase& pvBase)
{
    addChildInternal(pvBase);
}

void Node::addChildInternal(Base& child)
{
    std::static_pointer_cast<NodeImpl>(m_pImplementation)->addChild(child.m_pImplementation);
}

}
