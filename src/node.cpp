/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/node.h"
#include "nds3/pvBase.h"
#include "nds3/stateMachine.h"
#include "nds3/factory.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/factoryBaseImpl.h"

namespace nds
{

Node::Node()
{

}

Node::Node(std::shared_ptr<NodeImpl> nodeImpl): Base(std::static_pointer_cast<BaseImpl>(nodeImpl))
{
}

Node::Node(const std::string &name, const nodeType_t nodeType): Base(std::shared_ptr<BaseImpl>(new NodeImpl(name, nodeType)))
{
}

void Node::initialize(void* pDeviceObject, Factory& factory)
{
    std::shared_ptr<FactoryBaseImpl> pFactory = factory.m_pFactory;
    std::static_pointer_cast<NodeImpl>(m_pImplementation)->initializeRootNode(pDeviceObject, *(pFactory.get()));
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
