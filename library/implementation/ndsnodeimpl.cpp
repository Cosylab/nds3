#include "ndsnodeimpl.h"
#include "ndsstatemachineimpl.h"
#include "../include/nds3/definitions.h"
#include <memory>

namespace nds
{

NodeImpl::NodeImpl(const std::string &name): BaseImpl(name)
{}


void NodeImpl::addChild(std::shared_ptr<BaseImpl> pChild)
{
    std::string name(pChild->getComponentName());

    if(m_children.find(name) != m_children.end())
    {
        throw;
    }
    pChild->setParent(std::static_pointer_cast<NodeImpl>(shared_from_this()) );
    m_children[name] = pChild;

    std::shared_ptr<StateMachineImpl> stateMachine = std::dynamic_pointer_cast<StateMachineImpl>(pChild);
    if(stateMachine.get() != 0)
    {
        m_pStateMachine = stateMachine;
    }
}

void NodeImpl::initialize()
{
    for(tChildren::iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        scanChildren->second->initialize();
    }
}

state_t NodeImpl::getLocalState()
{
    if(m_pStateMachine.get() == 0)
    {
        return unknownState;
    }
    return m_pStateMachine->getLocalState();
}

state_t NodeImpl::getGlobalState()
{
    if(m_pStateMachine.get() == 0)
    {
        return unknownState;
    }
    return m_pStateMachine->getGlobalState();
}

state_t NodeImpl::getChildrenState()
{
    state_t returnState = unknownState;
    for(tChildren::iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        if(scanChildren->second.get() != m_pStateMachine.get())
        {
            std::shared_ptr<NodeImpl> child = std::dynamic_pointer_cast<NodeImpl>(scanChildren->second);
            if(child.get() != 0)
            {
                state_t childState = child->getGlobalState();
                if(childState > returnState)
                {
                    returnState = childState;
                }
            }
        }
    }
    return returnState;
}



}
