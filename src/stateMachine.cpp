#include "../include/nds3/stateMachine.h"
#include "../include/nds3impl/stateMachineImpl.h"

namespace nds
{

/*
 * Default Constructor
 *
 *********************/
StateMachine::StateMachine()
{
}


/*
 * Constructor. Setup the delegates
 *
 **********************************/
StateMachine::StateMachine(bool bAsync,
                           stateChange_t switchOnFunction,
                           stateChange_t switchOffFunction,
                           stateChange_t startFunction,
                           stateChange_t stopFunction,
                           stateChange_t recoverFunction,
                           allowChange_t allowStateChangeFunction):
    Node(std::shared_ptr<NodeImpl>(new StateMachineImpl(bAsync,
                                                        switchOnFunction,
                                                        switchOffFunction,
                                                        startFunction,
                                                        stopFunction,
                                                        recoverFunction,
                                                        allowStateChangeFunction)))
{
}


/*
 * Change the local state
 *
 ************************/
void StateMachine::setState(state_t newState)
{
    std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->setState(newState);
}


/*
 * Get the local state
 *
 *********************/
state_t StateMachine::getLocalState()
{
    return std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->getLocalState();

}


/*
 * Get the global state
 *
 **********************/
state_t StateMachine::getGlobalState()
{
    state_t state;
    timespec unused;
    std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->getGlobalState(&unused, &state);
    return state;
}


/*
 * Check if a state transition is allowed
 *
 ****************************************/
bool StateMachine::canChange(const state_t newState)
{
    return std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->canChange(newState);

}


}
