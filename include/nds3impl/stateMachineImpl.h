/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSSTATEMACHINEIMPL_H
#define NDSSTATEMACHINEIMPL_H

#include <thread>
#include "nodeImpl.h"
#include "../nds3/definitions.h"

namespace nds
{

template <typename T> class PVDelegateInImpl;

/**
 * @brief Implements the state machine.
 *
 * Provides several PVs that allow to read and set the local and global state.
 */
class StateMachineImpl: public NodeImpl
{
public:

    /**
     * @brief Construct the StateMachineImpl class.
     *
     * @param bAsync                   if true then the state transitions are executed
     *                                  in a dedicated thread, if false then the state
     *                                  transitions block
     * @param switchOnFunction         function to be called to switch the node state_t::on.
     *                                 The function is guaranteed to be called only when
     *                                  the current state is state_t::off.
     *                                 The state machine sets the state to state_t::initializing before
     *                                  calling the function and to state_t::on after the function returns.
     * @param switchOffFunction        function to be called to switch the node state_t::off.
     *                                 The function is guaranteed to be called only when
     *                                  the current state is state_t::on.
     *                                 The state machine sets the state to state_t::switchingOff before
     *                                  calling the function and to state_t::off after the function returns.
     * @param startFunction            function to be called to switch to the state_t::running state.
     *                                 The function is guaranteed to be called only when
     *                                  the current state is state_t::on.
     *                                 The state machine sets the state to state_t::starting before
     *                                  calling the function and to state_t::running after the function returns.
     * @param stopFunction             function to be called to switch to the state_t::off state.
     *                                 The function is guaranteed to be called only when the current
     *                                  state is state_t::running.
     *                                 The state machine sets the state to state_t::switchingOff before
     *                                  calling the function and to state_t::off after the function returns
     * @param recoverFunction          function to be called to switch the state fron state_t::fault to
     *                                  state_t::off.
     *                                 The function is guaranteed to be called only when the current
     *                                  state is state_t::fault.
     *                                 The state machine sets the state to state_t::switchingOff before
     *                                  calling the function and to state_t::off after the function returns
     * @param allowStateChangeFunction function to be called before every state switching to receive
     *                                  a confirmation that the state switch is allowed.
     *                                 The function is called only after other internal checks clear
     *                                  the state switch
     */
    StateMachineImpl(bool bAsync,
                     stateChange_t switchOnFunction,
                     stateChange_t switchOffFunction,
                     stateChange_t startFunction,
                     stateChange_t stopFunction,
                     stateChange_t recoverFunction,
                     allowChange_t allowStateChangeFunction);

    ~StateMachineImpl();

    /**
     * @brief Instructs the state machine to start the transition to another state.
     *
     * If the machine cannot start the transition to the desidered state then one
     *  of the following exceptions is thrown:
     * - StateMachineForbidden: one of the intermediate states has been specified in
     *                          the parameter. Only the state machine itself can
     *                          change the state to switchingOff, initializing, stopping
     *                          and starting.
     *
     * @param newState the new requested state. Only the following states are
     *                 allowed:
     *                 - state_t::off
     *                 - state_t::on
     *                 - state_t::running
     *                 - state_t::fault
     */
    void setState(const state_t newState);

    /**
     * @brief Return the local state. The local state does not reflect the state of the children.
     *
     * @return the current local state
     */
    virtual state_t getLocalState() const;

    /**
     * @brief Return the global state. The global state takes into consideration the states
     *        of the children of the node to which the state machine is attached.
     *
     * @param pTimestamp pointer to a variable that will be filled with the timestamp
     * @param pState     pointer to a variable that will be filled with the current global state
     */
    virtual void getGlobalState(timespec* pTimestamp, state_t* pState) const;

    /**
     * @brief Check if the transition to the new state is legal and is not denied by
     *        the delegate function.
     *
     * @param newState the new requested state
     * @return true if the transition is legal and has not been denied, false otherwise.
     */
    bool canChange(const state_t newState) const;

    /**
     * @brief Returns true if the transition between two states is legal (does not
     *        check the delegate function allowChange_t declared in the constructor).
     *
     * @param currentState the initial state
     * @param newState     the final state
     * @return             true if the state change is legal, false otherwise
     */
    bool isAllowedTransition(const state_t currentState, const state_t newState) const;

    /**
     * @brief Returns true if the specified state is an intermediate state (not
     *        settable directly with setState()).
     *
     * @param newState the state for which the information is required
     * @return true if the specified state is intermediate
     */
    bool isIntermediateState(const state_t newState) const;

    /**
     * @brief Delegate function for the commands that set the node's state
     *
     * @param state      the state to set
     * @param parameters parameters passed to the delegate (ignored)
     * @return           parameters returned by the command delegates (empty)
     */
    parameters_t commandSetState(const state_t state, const parameters_t& parameters);

    /**
     * @brief Register the PVs and set the local state to state_t::off.
     *
     * @param controlSystem the control system on which the node must be registered
     */
    virtual void initialize(FactoryBaseImpl& controlSystem);

    /**
     * @brief Wait for pending transitions to terminate and deregister the PVs.
     */
    virtual void deinitialize();


protected:

    /**
     * @brief Execute the state transition. May be called from a separate thread.
     *
     * @param initialState       the initial state
     * @param finalState         the final state
     * @param transitionFunction the delegate function that execute the transition
     */
    void executeTransition(const state_t initialState, const state_t finalState, stateChange_t transitionFunction);

    /**
     * @brief Thread function executed for asynchronous state machines. Calls executeTransition()
     *        and catches its exceptions.
     *
     * @param initialState       the initial state
     * @param finalState         the final state
     * @param transitionFunction the delegate function that executes the transition
     */
    void executeTransitionThread(const state_t initialState, const state_t finalState, stateChange_t transitionFunction);

    /**
     * @brief Delegate function called to read the local state.
     *
     * @param pTimestamp pointer to a value that will be filled with the timestamp of the last state change
     * @param pValue     pointer to a value that will be written with the current local state
     */
    void readLocalState(timespec* pTimestamp, std::int32_t* pValue);

    /**
     * @brief Delegate function called to set the local state.
     *
     * @param timestamp ignored. The timestamp is taken from the node
     * @param value     requested state
     */
    void writeLocalState(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Delegate function called to read the current global state
     *
     * @param pTimestamp pointer tp
     * @param pValue
     */
    void readGlobalState(timespec* pTimestamp, std::int32_t* pValue);

    /**
     * @brief Returns the human readable name for the requested state.
     *
     * @param state the state for which the name is required
     * @return the human readable name of the state
     */
    static std::string getStateName(const state_t state);

    bool m_bAsync;                     ///< If true then the state transitions happen in another thread (held by m_transitionThread)

    std::thread m_transitionThread;    ///< Thread that is used to execute the state transition when bAsync is true in the constructor
    std::mutex m_lockTransitionThread; ///< Lock the access to m_transitionThread

    state_t m_localState;              ///< The local state
    timespec m_stateTimestamp;         ///< The timestamp of the last local state change

    stateChange_t m_switchOn;          ///< Delegate function for the switch-on transition
    stateChange_t m_switchOff;         ///< Delegate function for the switch-off transition
    stateChange_t m_start;             ///< Delegate function for the start transition
    stateChange_t m_stop;              ///< Delegate function for the stop transition
    stateChange_t m_recover;           ///< Delegate function for the recover transition
    allowChange_t m_allowChange;       ///< Delegate function for the Allow-Change function

    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_pGetStatePV; ///< Delegate PV to which the local state change is pushed

};


}

#endif // NDSSTATEMACHINEIMPL_H
