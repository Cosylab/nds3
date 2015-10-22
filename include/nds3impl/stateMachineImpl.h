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
 * Provides 2 PVs that allow to read and set the local and global state.
 */
class StateMachineImpl: public NodeImpl
{
public:

    StateMachineImpl(bool bAsync,
                     stateChange_t switchOnFunction,
                     stateChange_t switchOffFunction,
                     stateChange_t startFunction,
                     stateChange_t stopFunction,
                     stateChange_t recoverFunction,
                     allowChange_t allowStateChangeFunction);

    ~StateMachineImpl();

    virtual void initialize(FactoryBaseImpl& controlSystem);

    virtual void deinitialize();

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

    virtual state_t getLocalState() const;
    virtual state_t getGlobalState() const;

    bool canChange(const state_t newState) const;

    bool isAllowedTransition(const state_t currentState, const state_t newState) const;

    bool isIntermediateState(const state_t newState) const;

    parameters_t commandSetState(const state_t state, const parameters_t& parameters);

protected:
    void executeTransition(const state_t initialState, const state_t finalState, stateChange_t transitionFunction);

    void readLocalState(timespec* pTimestamp, std::int32_t* pValue);
    void writeLocalState(const timespec& pTimestamp, const std::int32_t& value);

    void readGlobalState(timespec* pTimestamp, std::int32_t* pValue);
    void writeGlobalState(const timespec& pTimestamp, const std::int32_t& value);

    static std::string getStateName(const state_t state);

    bool m_bAsync;

    std::thread m_transitionThread;
    std::mutex m_lockTransitionThread;

    state_t m_localState;
    timespec m_stateTimestamp;

    stateChange_t m_switchOn;
    stateChange_t m_switchOff;
    stateChange_t m_start;
    stateChange_t m_stop;
    stateChange_t m_recover;
    allowChange_t m_allowChange;

    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_pGetStatePV;

};


}

#endif // NDSSTATEMACHINEIMPL_H
