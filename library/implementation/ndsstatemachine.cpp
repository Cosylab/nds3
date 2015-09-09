#include "../include/nds3/ndsstatemachine.h"
#include "ndsstatemachineimpl.h"

namespace nds
{

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

void StateMachine::setState(state_t newState)
{
    std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->setState(newState);
}

state_t StateMachine::getLocalState()
{
    return std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->getLocalState();

}

state_t StateMachine::getGlobalState()
{
    return std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->getGlobalState();

}

bool StateMachine::canChange(const state_t newState)
{
    return std::static_pointer_cast<StateMachineImpl>(m_pImplementation)->canChange(newState);

}


}
