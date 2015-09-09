#include "ndsstatemachineimpl.h"
#include "ndspvdelegateimpl.h"
#include "ndspvbaseimpl.h"
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

StateMachineImpl::StateMachineImpl(bool bAsync,
                                   stateChange_t switchOnFunction,
                                   stateChange_t switchOffFunction,
                                   stateChange_t startFunction,
                                   stateChange_t stopFunction,
                                   stateChange_t recoverFunction,
                                   allowChange_t allowStateChangeFunction): NodeImpl("StateMachine"),
            m_bAsync(bAsync),
            m_localState(state_t::off),
            m_switchOn(switchOnFunction), m_switchOff(switchOffFunction), m_start(startFunction), m_stop(stopFunction), m_recover(recoverFunction),
            m_allowChange(allowStateChangeFunction)
{
    // Add PVs for the state machine
    std::shared_ptr<PVDelegateImpl<std::int32_t> > pSetStatePV(
                new PVDelegateImpl<std::int32_t>("setState",
                                                 std::bind(&StateMachineImpl::readLocalState, this, std::placeholders::_1, std::placeholders::_2),
                                                 std::bind(&StateMachineImpl::writeLocalState, this, std::placeholders::_1, std::placeholders::_2)));
    pSetStatePV->setDescription("Set local state");
    pSetStatePV->setScanType(scanType_t::passive, 0);
    pSetStatePV->setType(recordType_t::longout);
    addChild(pSetStatePV);

    m_pGetStatePV.reset(
                new PVDelegateImpl<std::int32_t>("getState",
                                                 std::bind(&StateMachineImpl::readLocalState, this, std::placeholders::_1, std::placeholders::_2)));
    m_pGetStatePV->setDescription("Get local state");
    m_pGetStatePV->setScanType(scanType_t::interrupt, 0);
    m_pGetStatePV->setType(recordType_t::longin);
    addChild(m_pGetStatePV);

    std::shared_ptr<PVDelegateImpl<std::int32_t> > pGetGlobalStatePV(
                new PVDelegateImpl<std::int32_t>("getGlobalState",
                                                 std::bind(&StateMachineImpl::readGlobalState, this, std::placeholders::_1, std::placeholders::_2)));
    pGetGlobalStatePV->setScanType(scanType_t::passive, 0);
    pGetGlobalStatePV->setType(recordType_t::longin);
    addChild(pGetGlobalStatePV);
}

StateMachineImpl::~StateMachineImpl()
{
    if(m_transitionThread.joinable())
    {
        m_transitionThread.join();
    }
}

void StateMachineImpl::initialize()
{
    NodeImpl::initialize();

    std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
    m_localState = state_t::off;
    m_stateTimestamp = getTimestamp();
    m_pGetStatePV->push(m_stateTimestamp, (std::int32_t)m_localState);
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

        if(localState == state_t::off && newState == state_t::on)
        {
            transitionFunction = m_switchOn;
            transitionState = state_t::initializing;
        }
        else if(localState == state_t::on && newState == state_t::off)
        {
            transitionFunction = m_switchOff;
            transitionState = state_t::switchingOff;
        }
        else if(localState == state_t::on && newState == state_t::running)
        {
            transitionFunction = m_start;
            transitionState = state_t::starting;
        }
        else if(localState == state_t::running && newState == state_t::on)
        {
            transitionFunction = m_stop;
            transitionState = state_t::stopping;
        }
        else if(localState == state_t::fault && newState == state_t::off)
        {
            transitionFunction = m_recover;
            transitionState = state_t::switchingOff;
        }
        else
        {
            std::ostringstream buildErrorMessage;
            buildErrorMessage << "No transition from state " << (int)localState << " to state " << (int)newState;
            throw StateMachineNoSuchTransition(buildErrorMessage.str());
        }

        // Check if the transition is allowed, then set the intermediate state
        //////////////////////////////////////////////////////////////////////
        if(!m_allowChange(localState, globalState, newState))
        {
            std::ostringstream buildErrorMessage;
            buildErrorMessage << "The transition from state " << (int)localState << " to state " << (int)newState << " has been denied";
            throw StateMachineTransitionDenied(buildErrorMessage.str());
        }
        m_localState = transitionState;
        m_stateTimestamp = getTimestamp();
        m_pGetStatePV->push(m_stateTimestamp, (std::int32_t)m_localState);
    }
    if(m_bAsync)
    {
        if(m_transitionThread.joinable())
        {
            m_transitionThread.join();
        }
        m_transitionThread = std::thread(std::bind(&StateMachineImpl::executeTransition, this, localState, newState, transitionFunction));
    }
    else
    {
        executeTransition(localState, newState, transitionFunction);
    }
}

void StateMachineImpl::executeTransition(const state_t initialState, const state_t finalState, stateChange_t transitionFunction)
{
    try
    {
        transitionFunction();
        std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
        m_localState = finalState;
        m_stateTimestamp = getTimestamp();
        m_pGetStatePV->push(m_stateTimestamp, (std::int32_t)m_localState);
    }
    catch(StateMachineRollBack& e)
    {
        std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
        m_localState = initialState;
        m_stateTimestamp = getTimestamp();
        m_pGetStatePV->push(m_stateTimestamp, (std::int32_t)m_localState);
        throw;
    }
    catch(std::runtime_error& e)
    {
        std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
        m_localState = state_t::fault;
        m_stateTimestamp = getTimestamp();
        m_pGetStatePV->push(m_stateTimestamp, (std::int32_t)m_localState);
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

    if((int)returnState > (int)childrenState)
    {
        return returnState;
    }
    return childrenState;

}

bool StateMachineImpl::isAllowedTransition(const state_t currentState, const state_t newState) const
{
    return isIntermediateState(currentState) ||
                (currentState == state_t::off && newState == state_t::on) ||
                (currentState == state_t::on && newState == state_t::off) ||
                (currentState == state_t::on && newState == state_t::running) ||
                (currentState == state_t::running && newState == state_t::on) ||
                (currentState == state_t::fault && newState == state_t::on) ||
                newState == state_t::fault;

}

bool StateMachineImpl::isIntermediateState(const state_t state) const
{
    return state == state_t::switchingOff ||
            state == state_t::initializing ||
            state == state_t::starting ||
            state == state_t::stopping;
}

void StateMachineImpl::readLocalState(timespec* pTimestamp, std::int32_t* pValue)
{
    std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
    *pTimestamp = m_stateTimestamp;
    *pValue = (std::int32_t)m_localState;
}

void StateMachineImpl::writeLocalState(const timespec& /* pTimestamp */, const std::int32_t& value)
{
    setState((state_t)value);
}

void StateMachineImpl::readGlobalState(timespec* pTimestamp, std::int32_t* pValue)
{
    *pValue = (std::int32_t)getGlobalState();
    *pTimestamp = getTimestamp();
}

void StateMachineImpl::writeGlobalState(const timespec& pTimestamp, const std::int32_t& value)
{

}

}
