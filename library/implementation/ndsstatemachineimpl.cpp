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
    m_pStatePV.reset(
                new PVDelegateImpl<std::int32_t>("state",
                                                 std::bind(&StateMachineImpl::readLocalState, this, std::placeholders::_1, std::placeholders::_2),
                                                 std::bind(&StateMachineImpl::writeLocalState, this, std::placeholders::_1, std::placeholders::_2)));
    m_pGlobalStatePV.reset(
                new PVDelegateImpl<std::int32_t>("globalState",
                                                 std::bind(&StateMachineImpl::readGlobalState, this, std::placeholders::_1, std::placeholders::_2),
                                                 std::bind(&StateMachineImpl::writeGlobalState, this, std::placeholders::_1, std::placeholders::_2)));

    addChild(m_pStatePV);
    addChild(m_pGlobalStatePV);
}

StateMachineImpl::~StateMachineImpl()
{

}

bool StateMachineImpl::canChange(state_t newState) const
{
    state_t currentState(getGlobalState());

    return isAllowedTransition(currentState, newState) &&  m_allowChange(currentState, newState);
}

void StateMachineImpl::setState(const state_t newState)
{
    state_t currentState;
    state_t transitionState;
    stateChange_t transitionFunction;
    {
        std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
        currentState = getGlobalState();

        if(currentState == off && newState == on)
        {
            transitionFunction = m_switchOn;
            transitionState = initializing;
        }
        else if(currentState == on && newState == off)
        {
            transitionFunction = m_switchOff;
            transitionState = switchingOff;
        }
        else if(currentState == on && newState == running)
        {
            transitionFunction = m_start;
            transitionState = starting;
        }
        else if(currentState == running && newState == on)
        {
            transitionFunction = m_stop;
            transitionState = stopping;
        }
        else if(currentState == fault && newState == off)
        {
            transitionFunction = m_recover;
            transitionState = switchingOff;
        }
        else
        {
            std::ostringstream buildErrorMessage;
            buildErrorMessage << "No transition from state " << currentState << " to state " << newState;
            throw StateMachineNoSuchTransition(buildErrorMessage.str());
        }

        // Check if the transition is allowed, then set the intermediate state
        //////////////////////////////////////////////////////////////////////
        if(m_allowChange(currentState, newState))
        {
            std::ostringstream buildErrorMessage;
            buildErrorMessage << "The transition from state " << currentState << " to state " << newState << " has been denied";
            throw StateMachineTransitionDenied(buildErrorMessage.str());
        }
        m_localState = transitionState;
    }
    try
    {
        transitionFunction();
        std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
        m_localState = newState;
    }
    catch(StateMachineRollBack& e)
    {
        std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
        m_localState = currentState;
    }
    catch(std::runtime_error& e)
    {
        std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
        m_localState = fault;
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
    *pValue = getLocalState();
}

void StateMachineImpl::writeLocalState(const timespec& pTimestamp, const std::int32_t& value)
{
    setState((state_t)value);
}

void StateMachineImpl::readGlobalState(timespec* pTimestamp, std::int32_t* pValue)
{
    *pValue = getGlobalState();
}

void StateMachineImpl::writeGlobalState(const timespec& pTimestamp, const std::int32_t& value)
{

}


}
