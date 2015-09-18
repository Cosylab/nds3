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
        scanChildren->second->setParent(std::static_pointer_cast<NodeImpl>(shared_from_this()) );
    }
    for(tChildren::iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        scanChildren->second->initialize();
    }
}

state_t NodeImpl::getLocalState() const
{
    if(m_pStateMachine.get() == 0)
    {
        return state_t::unknown;
    }
    return m_pStateMachine->getLocalState();
}

state_t NodeImpl::getGlobalState() const
{
    if(m_pStateMachine.get() == 0)
    {
        return getChildrenState();
    }
    return m_pStateMachine->getGlobalState();
}

state_t NodeImpl::getChildrenState() const
{
    state_t returnState = state_t::unknown;
    for(tChildren::const_iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        if(scanChildren->second.get() != m_pStateMachine.get())
        {
            std::shared_ptr<NodeImpl> child = std::dynamic_pointer_cast<NodeImpl>(scanChildren->second);
            if(child.get() != 0)
            {
                state_t childState = child->getGlobalState();
                if((int)childState > (int)returnState)
                {
                    returnState = childState;
                }
            }
        }
    }
    return returnState;
}

}
