#ifndef NDSSTATEMACHINE_H
#define NDSSTATEMACHINE_H

#include <cstdint>

#include "node.h"
#include "definitions.h"

namespace nds
{

/**
 * @brief Represents a state machine to be attached to a node.
 *
 * Attach the state machine to a node by using Node::addChild().
 *
 * The state machine provides a <b>local state</b> and a <b>global state</b>:
 * - the local state represents the state of a single state machine
 * - the global state takes into account all the state machines attached to
 *   the children nodes (the state with the higher priority)
 *
 * In EPICS the state machine PVs will be available as "StateMachine-setState"
 *  (set the state) and "StateMachine-getState" (to retrieve the state).
 * The PV "StateMachine-globalState" represents the global state (state
 *  with higher priority among the current node and all its children).
 *
 * Transitions between the different states can happen on the same
 *  thread that requested the transition or on a different thread when
 *  the StateMachine is created with the bAsync flag set to true.
 */
class NDS3_API StateMachine: public Node
{

public:
    StateMachine();

    /**
     * @brief Construct the state machine.
     *
     * @param bAsync                   if true then the state transitions are executed
     *                                  in a dedicated thread, if false then the state
     *                                  transitions block
     * @param switchOnFunction         function to be called to switch the node on.
     *                                 The function is guaranteed to be called only when
     *                                  the current state is off.
     *                                 The state machine sets the state to initializing before
     *                                  calling the function and to on after the function returns.
     * @param switchOffFunction        function to be called to switch the node off.
     *                                 The function is guaranteed to be called only when
     *                                  the current state is on.
     *                                 The state machine sets the state to switchingOff before
     *                                  calling the function and to off after the function returns.
     * @param startFunction            function to be called to switch to the running state.
     *                                 The function is guaranteed to be called only when
     *                                  the current state is on.
     *                                 The state machine sets the state to starting before
     *                                  calling the function and to running after the function returns.
     * @param stopFunction
     * @param recoverFunction
     * @param allowStateChangeFunction
     */
    StateMachine(bool bAsync,
                 stateChange_t switchOnFunction,
                 stateChange_t switchOffFunction,
                 stateChange_t startFunction,
                 stateChange_t stopFunction,
                 stateChange_t recoverFunction,
                 allowChange_t allowStateChangeFunction);

    /**
     * @brief Instruct the state machine to change the local state to the requested one.
     *
     * The state machine will first check that the change from the current state to
     *  the requested state is legal: if the change is not legal then a StateMachineNoSuchTransition
     *  exception is thrown.
     *
     * Then the delegate function allowStateChangeFunction defined in the state machine constructor
     *  will be called: if the function returns false then a StateMachineTransitionDenied exception
     *  will be thrown.
     *
     * The state will be changed to an intermediate states while the state change delegate function is
     *  executed. If the function executing the state change fails then it can throw one of the following
     *  exceptions:
     * - StateMachineRollBack: the state change failed and the state should go back to the initial one
     *   (the state active before the transition was requested)
     * - any exception: the state switches to Fault.
     *
     * @param newState
     */
    void setState(const state_t newState);

    /**
     * @brief Returns the local state of the state machine.
     *
     * The local state does not reflect the state of the children nodes.
     *
     * @return the local state
     */
    state_t getLocalState();

    /**
     * @brief Returns the global state of the node to which this state machine is
     *        attached.
     *
     * The global state is the state with higher priority amongs the local state and the states
     *  of the children nodes of the node that owns this state machine.
     *
     * @return the global state
     */
    state_t getGlobalState();

    /**
     * @brief Check if the transition to the new state is legal and is not denied by
     *        the delegate function.
     *
     * @param newState the new requested state
     * @return true if the transition is legal and has not been denied, false otherwise.
     */
    bool canChange(const state_t newState);

};

}
#endif // NDSSTATEMACHINE_H
