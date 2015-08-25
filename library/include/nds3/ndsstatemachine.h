#ifndef NDSSTATEMACHINE_H
#define NDSSTATEMACHINE_H

#include <cstdint>

#include "ndsnode.h"
#include "definitions.h"

namespace nds
{

/**
 * @brief Provides a state machine to a Node.
 *
 * The state machine ha
 */
class StateMachine: public Node
{

public:
    /**
     * @brief Creates a state machine to be attached to a node.
     *
     * Attach the state machine to a node by using Node::addChild().
     * In EPICS the state machine PVs will be available as "StateMachine-state" and
     * "StateMachine-globalState"
     *
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
     * @param startFunction
     * @param stopFunction
     * @param recoverFunction
     * @param allowStateChangeFunction
     */
    StateMachine(stateChange_t switchOnFunction,
                 stateChange_t switchOffFunction,
                 stateChange_t startFunction,
                 stateChange_t stopFunction,
                 stateChange_t recoverFunction,
                 allowChange_t allowStateChangeFunction);

    void setState(const state_t newState);
    state_t getLocalState();
    state_t getGlobalState();
    bool canChange(const state_t newState);


};

}
#endif // NDSSTATEMACHINE_H
