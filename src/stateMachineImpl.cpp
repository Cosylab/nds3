#include "../include/nds3impl/stateMachineImpl.h"
#include "../include/nds3impl/pvDelegateOutImpl.h"
#include "../include/nds3impl/pvDelegateInImpl.h"
#include "../include/nds3impl/pvBaseImpl.h"
#include "../include/nds3/exceptions.h"
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
    // Prepare enumeration for states
    /////////////////////////////////
    enumerationStrings_t enumerationStrings;
    for(int state(0); state != (int)state_t::MAX_STATE_NUM; ++state)
    {
        enumerationStrings.push_back(getStateName((state_t)state));
    }

    // Add PVs for the state machine
    ////////////////////////////////
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > pSetStatePV(
                new PVDelegateOutImpl<std::int32_t>("setState",
                                                    std::bind(&StateMachineImpl::writeLocalState, this, std::placeholders::_1, std::placeholders::_2),
                                                    std::bind(&StateMachineImpl::readLocalState, this, std::placeholders::_1, std::placeholders::_2)));
    pSetStatePV->setDescription("Set local state");
    pSetStatePV->setScanType(scanType_t::passive, 0);
    pSetStatePV->setEnumeration(enumerationStrings);
    addChild(pSetStatePV);

    m_pGetStatePV.reset(
                new PVDelegateInImpl<std::int32_t>("getState",
                                                 std::bind(&StateMachineImpl::readLocalState, this, std::placeholders::_1, std::placeholders::_2)));
    m_pGetStatePV->setDescription("Get local state");
    m_pGetStatePV->setScanType(scanType_t::interrupt, 0);
    m_pGetStatePV->setEnumeration(enumerationStrings);
    addChild(m_pGetStatePV);

    std::shared_ptr<PVDelegateInImpl<std::int32_t> > pGetGlobalStatePV(
                new PVDelegateInImpl<std::int32_t>("getGlobalState",
                                                 std::bind(&StateMachineImpl::readGlobalState, this, std::placeholders::_1, std::placeholders::_2)));
    pGetGlobalStatePV->setScanType(scanType_t::passive, 0);
    pGetGlobalStatePV->setEnumeration(enumerationStrings);
    addChild(pGetGlobalStatePV);

    // Register state transition commands
    /////////////////////////////////////
    defineCommand("switchOn", "", 0, std::bind(&StateMachineImpl::commandSetState, this, state_t::on, std::placeholders::_1));
    defineCommand("switchOff", "", 0, std::bind(&StateMachineImpl::commandSetState, this, state_t::off, std::placeholders::_1));
    defineCommand("start", "", 0, std::bind(&StateMachineImpl::commandSetState, this, state_t::running, std::placeholders::_1));
    defineCommand("stop", "", 0, std::bind(&StateMachineImpl::commandSetState, this, state_t::on, std::placeholders::_1));
}

StateMachineImpl::~StateMachineImpl()
{
    std::lock_guard<std::mutex> lockThread(m_lockTransitionThread);
    if(m_transitionThread.joinable())
    {
        m_transitionThread.join();
    }
}

void StateMachineImpl::initialize(FactoryBaseImpl& controlSystem)
{
    NodeImpl::initialize(controlSystem);

    std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
    m_localState = state_t::off;
    m_stateTimestamp = getTimestamp();
    m_pGetStatePV->push(m_stateTimestamp, (std::int32_t)m_localState);
}

void StateMachineImpl::deinitialize()
{
    std::lock_guard<std::mutex> lockThread(m_lockTransitionThread);
    if(m_transitionThread.joinable())
    {
        m_transitionThread.join();
    }
    NodeImpl::deinitialize();

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

/*
 * Changes the state. First check if the change is allowed, then execute the
 *  state transition.
 * The state transition may be executed in a secondary thread
 *
 ***************************************************************************/
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

        // Find the transitional state that we have to set and
        //  the function to call to perform the transition
        //////////////////////////////////////////////////////
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
            buildErrorMessage << "No transition from state " << getStateName(localState) << " to state " << getStateName(newState);
            throw StateMachineNoSuchTransition(buildErrorMessage.str());
        }

        // Check if the transition is allowed, then set the intermediate state
        //////////////////////////////////////////////////////////////////////
        if(!m_allowChange(localState, globalState, newState))
        {
            std::ostringstream buildErrorMessage;
            buildErrorMessage << "The transition from state " << getStateName(localState) << " to state " << getStateName(newState) << " has been denied";
            throw StateMachineTransitionDenied(buildErrorMessage.str());
        }
        m_localState = transitionState;
        m_stateTimestamp = getTimestamp();
        m_pGetStatePV->push(m_stateTimestamp, (std::int32_t)m_localState);
    }

    // Execute the state transition. Launch a secondary thread if necessary
    ///////////////////////////////////////////////////////////////////////
    if(m_bAsync)
    {
        std::lock_guard<std::mutex> lockThread(m_lockTransitionThread);
        if(m_transitionThread.joinable())
        {
            m_transitionThread.join();
        }
        m_transitionThread = std::thread(std::bind(&StateMachineImpl::executeTransitionThread, this, localState, newState, transitionFunction));
    }
    else
    {
        executeTransition(localState, newState, transitionFunction);
    }
}

void StateMachineImpl::executeTransitionThread(const state_t initialState, const state_t finalState, stateChange_t transitionFunction)
{
    try
    {
        executeTransition(initialState, finalState, transitionFunction);
    }
    catch(const std::runtime_error& e)
    {
        ndsErrorStream(*this) << "Error while asyncronously changing the state: " << e.what() << std::endl;
    }
}

/*
 * Execute the transition from a state to another. It may get executed
 *  in a secondary thread
 *
 *********************************************************************/
void StateMachineImpl::executeTransition(const state_t initialState, const state_t finalState, stateChange_t transitionFunction)
{
    try
    {
        ndsInfoStream(*this) << "Switching state from " << getStateName(initialState) << " to " << getStateName(finalState) << std::endl;

        transitionFunction();

        ndsInfoStream(*this) << "State switching successful" << std::endl;

        std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
        m_localState = finalState;
        m_stateTimestamp = getTimestamp();
        m_pGetStatePV->push(m_stateTimestamp, (std::int32_t)m_localState);
    }
    catch(StateMachineRollBack& e)
    {
        ndsWarningStream(*this) << "Warning: " << e.what() << " - Rolling back to state " << getStateName(initialState) << std::endl;

        std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
        m_localState = initialState;
        m_stateTimestamp = getTimestamp();
        m_pGetStatePV->push(m_stateTimestamp, (std::int32_t)m_localState);
        throw;
    }
    catch(std::runtime_error& e)
    {
        ndsErrorStream(*this) << "Error: " << e.what() << " - Switching to state " << getStateName(state_t::fault) << std::endl;

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

void StateMachineImpl::writeGlobalState(const timespec& /* pTimestamp */, const std::int32_t& /* value */)
{

}

parameters_t StateMachineImpl::commandSetState(const state_t state, const parameters_t& /* parameters */)
{
    setState(state);
    return parameters_t();
}

std::string StateMachineImpl::getStateName(const state_t state)
{
    switch(state)
    {
    case state_t::unknown:
        return "UNKNOWN";
    case state_t::off:
        return "OFF";
    case state_t::switchingOff:
        return "SWITCHING_OFF";
    case state_t::initializing:
        return "INITIALIZING";
    case state_t::on:
        return "ON";
    case state_t::stopping:
        return "STOPPING";
    case state_t::starting:
        return "STARTING";
    case state_t::running:
        return "RUNNING";
    case state_t::fault:
        return "FAULT";
    case state_t::MAX_STATE_NUM:
        throw std::logic_error("The enumeration MAX_STATE_NUM is used only to know how many states are defined");
    }
    throw std::logic_error("Uknown state");
}

}
