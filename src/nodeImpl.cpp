#include "../include/nds3impl/nodeImpl.h"
#include "../include/nds3impl/stateMachineImpl.h"
#include "../include/nds3/definitions.h"
#include <memory>
#include <mutex>
#include <sstream>

namespace nds
{

static std::recursive_mutex m_initializationMutex;

NodeImpl::NodeImpl(const std::string &name): BaseImpl(name)
{}


void NodeImpl::addChild(std::shared_ptr<BaseImpl> pChild)
{
    std::string name(pChild->getComponentName());

    if(m_children.find(name) != m_children.end())
    {
        std::ostringstream errorString;
        errorString << "A node with the name " << name << " has already been registered into " << getFullName() << std::endl;
        throw std::logic_error(errorString.str());
    }
    m_children[name] = pChild;

    std::shared_ptr<StateMachineImpl> stateMachine = std::dynamic_pointer_cast<StateMachineImpl>(pChild);
    if(stateMachine.get() != 0)
    {
        m_pStateMachine = stateMachine;

        // Add the state machine commands also to this node
        ///////////////////////////////////////////////////
        defineCommand("switchOn", "", 0, std::bind(&StateMachineImpl::setState, m_pStateMachine, state_t::on));
        defineCommand("switchOff", "", 0, std::bind(&StateMachineImpl::setState, m_pStateMachine, state_t::off));
        defineCommand("start", "", 0, std::bind(&StateMachineImpl::setState, m_pStateMachine, state_t::running));
        defineCommand("stop", "", 0, std::bind(&StateMachineImpl::setState, m_pStateMachine, state_t::on));

    }
}

void NodeImpl::initialize(FactoryBaseImpl& controlSystem)
{
    std::lock_guard<std::recursive_mutex> serializeInitialization(m_initializationMutex);

    BaseImpl::initialize(controlSystem);

    for(tChildren::iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        scanChildren->second->setParent(std::static_pointer_cast<NodeImpl>(shared_from_this()) );
    }
    for(tChildren::iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        scanChildren->second->initialize(controlSystem);
    }
}

void NodeImpl::deinitialize()
{
    std::lock_guard<std::recursive_mutex> serializeInitialization(m_initializationMutex);

    BaseImpl::deinitialize();
    for(tChildren::iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        scanChildren->second->deinitialize();
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

void NodeImpl::setLogLevel(const logLevel_t logLevel)
{
    BaseImpl::setLogLevel(logLevel);

    for(tChildren::const_iterator scanChildren(m_children.begin()), endScan(m_children.end()); scanChildren != endScan; ++scanChildren)
    {
        scanChildren->second->setLogLevel(logLevel);
    }

}

}
