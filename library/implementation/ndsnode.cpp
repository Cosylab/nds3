#include "../include/nds3/ndsnode.h"
#include "../include/nds3/ndspvbase.h"
#include "../include/nds3/ndsstatemachine.h"

#include "ndsnodeimpl.h"

namespace nds
{

Node::Node()
{

}

Node::Node(std::shared_ptr<NodeImpl> nodeImpl): Base(std::static_pointer_cast<BaseImpl>(nodeImpl))
{
}

Node::Node(const std::string &name): Base(std::shared_ptr<BaseImpl>(new NodeImpl(name)))
{
}

Node Node::addNode(Node& node)
{
    addChildInternal(node);
    return node;
}

PVBase Node::addPV(PVBase& pvBase)
{
    addChildInternal(pvBase);
    return pvBase;
}

StateMachine Node::addStateMachine(StateMachine& stateMachine)
{
    addChildInternal(stateMachine);
    return stateMachine;
}

void Node::addChildInternal(Base& child)
{
    std::static_pointer_cast<NodeImpl>(m_pImplementation)->addChild(child.m_pImplementation);
}

}
