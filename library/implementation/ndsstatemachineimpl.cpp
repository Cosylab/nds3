#include "ndsstatemachineimpl.h"
#include "ndspvdelegateimpl.h"
#include "../include/nds3/ndsexceptions.h"
#include <cstdint>
#include <mutex>
#include <sstream>

namespace nds
{

// The state change mutex is the same across all the state machines.
// This allows to read consistent states across the hierarchical state
// machine
//////////////////////////////////////////////////////////////////////
std::recursive_mutex m_stateMutex;

StateMachineImpl::StateMachineImpl(stateChange_t switchOnFunction,
                                   stateChange_t switchOffFunction,
                                   stateChange_t startFunction,
                                   stateChange_t stopFunction,
                                   stateChange_t recoverFunction,
                                   allowChange_t allowStateChangeFunction): NodeImpl("StateMachine"),
            m_localState(off),
            m_switchOn(switchOnFunction), m_switchOff(switchOffFunction), m_start(startFunction), m_stop(stopFunction), m_recover(recoverFunction),
            m_allowChange(allowStateChangeFunction)
{
    // Add PVs for the state machine
    std::shared_ptr<PVDelegateImpl<std::int32_t> > pSetStatePV(
                new PVDelegateImpl<std::int32_t>("setState",
                                                 std::bind(&StateMachineImpl::readLocalState, this, std::placeholders::_1, std::placeholders::_2),
                                                 std::bind(&StateMachineImpl::writeLocalState, this, std::placeholders::_1, std::placeholders::_2)));
    pSetStatePV->setDescription("Set local state");
    pSetStatePV->setScanType(passive, 0);
    pSetStatePV->setType(longout);
    addChild(pSetStatePV);

    std::shared_ptr<PVDelegateImpl<std::int32_t> > pGetStatePV(
                new PVDelegateImpl<std::int32_t>("getState",
                                                 std::bind(&StateMachineImpl::readLocalState, this, std::placeholders::_1, std::placeholders::_2)));
    pGetStatePV->setDescription("Get local state");
    pGetStatePV->setScanType(passive, 0);
    pGetStatePV->setType(longin);
    addChild(pGetStatePV);

    std::shared_ptr<PVDelegateImpl<std::int32_t> > pGetGlobalStatePV(
                new PVDelegateImpl<std::int32_t>("getGlobalState",
                                                 std::bind(&StateMachineImpl::readGlobalState, this, std::placeholders::_1, std::placeholders::_2)));
    pGetGlobalStatePV->setScanType(passive, 0);
    pGetGlobalStatePV->setType(longin);
    addChild(pGetGlobalStatePV);
}

StateMachineImpl::~StateMachineImpl()
{

}

bool StateMachineImpl::canChange(state_t newState) const
{
    state_t localState, globalState;
    {
        std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
        globalState = getGlobalState();
        localState = getLocalState();
    }

    return isAllowedTransition(localState, newState) &&  m_allowChange(localState, globalState, newState);
}

void StateMachineImpl::setState(const state_t newState)
{
    state_t localState;
    state_t globalState;
    state_t transitionState;
    stateChange_t transitionFunction;
    {
        std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
        localState = getLocalState();
        globalState = getGlobalState();

        if(localState == off && newState == on)
        {
            transitionFunction = m_switchOn;
            transitionState = initializing;
        }
        else if(localState == on && newState == off)
        {
            transitionFunction = m_switchOff;
            transitionState = switchingOff;
        }
        else if(localState == on && newState == running)
        {
            transitionFunction = m_start;
            transitionState = starting;
        }
        else if(localState == running && newState == on)
        {
            transitionFunction = m_stop;
            transitionState = stopping;
        }
        else if(localState == fault && newState == off)
        {
            transitionFunction = m_recover;
            transitionState = switchingOff;
        }
        else
        {
            std::ostringstream buildErrorMessage;
            buildErrorMessage << "No transition from state " << localState << " to state " << newState;
            throw StateMachineNoSuchTransition(buildErrorMessage.str());
        }

        // Check if the transition is allowed, then set the intermediate state
        //////////////////////////////////////////////////////////////////////
        if(!m_allowChange(localState, globalState, newState))
        {
            std::ostringstream buildErrorMessage;
            buildErrorMessage << "The transition from state " << localState << " to state " << newState << " has been denied";
            throw StateMachineTransitionDenied(buildErrorMessage.str());
        }
        m_localState = transitionState;
        m_stateTimestamp = getTimestamp();
    }
    try
    {
        transitionFunction();
        std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
        m_localState = newState;
        m_stateTimestamp = getTimestamp();
    }
    catch(StateMachineRollBack& e)
    {
        std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
        m_localState = localState;
        m_stateTimestamp = getTimestamp();
    }
    catch(std::runtime_error& e)
    {
        std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
        m_localState = fault;
        m_stateTimestamp = getTimestamp();
        throw;
    }
}

state_t StateMachineImpl::getLocalState() const
{
    std::lock_guard<std::recursive_mutex> lock(m_stateMutex);

    return m_localState;
}


state_t StateMachineImpl::getGlobalState() const
{
    std::lock_guard<std::recursive_mutex> lock(m_stateMutex);

    state_t returnState = m_localState;

    std::shared_ptr<NodeImpl> pParentNode(getParent());

    state_t childrenState = pParentNode->getChildrenState();

    if(returnState > childrenState)
    {
        return returnState;
    }
    return childrenState;

}

bool StateMachineImpl::isAllowedTransition(const state_t currentState, const state_t newState) const
{
    return isIntermediateState(currentState) ||
                (currentState == off && newState == on) ||
                (currentState == on && newState == off) ||
                (currentState == on && newState == running) ||
                (currentState == running && newState == on) ||
                (currentState == fault && newState == on) ||
                newState == fault;

}

bool StateMachineImpl::isIntermediateState(const state_t state) const
{
    return state == switchingOff ||
            state == initializing ||
            state == starting ||
            state == stopping;
}



void StateMachineImpl::readLocalState(timespec* pTimestamp, std::int32_t* pValue)
{
    std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
    *pTimestamp = m_stateTimestamp;
    *pValue = m_localState;
}

void StateMachineImpl::writeLocalState(const timespec& /* pTimestamp */, const std::int32_t& value)
{
    setState((state_t)value);
}

void StateMachineImpl::readGlobalState(timespec* pTimestamp, std::int32_t* pValue)
{
    *pValue = getGlobalState();
    *pTimestamp = getTimestamp();
}

void StateMachineImpl::writeGlobalState(const timespec& pTimestamp, const std::int32_t& value)
{

}

}
