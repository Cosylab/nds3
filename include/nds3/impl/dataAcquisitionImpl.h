/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSDATAACQUISITIONIMPL_H
#define NDSDATAACQUISITIONIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"

namespace nds
{

template <typename T> class PVVariableInImpl;
template <typename T> class PVVariableOutImpl;


template<typename T>
class DataAcquisitionImpl: public NodeImpl
{
public:
    DataAcquisitionImpl(const std::string& name,
                    size_t maxElements,
                    stateChange_t switchOnFunction,
                    stateChange_t switchOffFunction,
                    stateChange_t startFunction,
                    stateChange_t stopFunction,
                    stateChange_t recoverFunction,
                    allowChange_t allowStateChangeFunction);

    /**
     * @brief Specifies the function to call to get the acquisition start timestamp.
     *
     * The function is called only once at each start of the acquisition and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    void push(const timespec& timestamp, const T& data);

    double getFrequencyHz();
    double getDurationSeconds();
    double getAmplitude();
    double getOffset();
    size_t getMaxElements();
    size_t getDecimation();
    size_t getSamplingMode();
    size_t getGround();

    /**
     * @brief Returns the timestamp at the moment of the start of the acquisition.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when the acquisition started.
     */
    timespec getStartTimestamp() const;

    /**
     * @brief Called by the state machine. Store the current timestamp and then calls the
     *        delegated onStart function.
     */
    void onStart();


protected:
    /**
     * @brief In the state machine we set the start function to onStart(), so we
     *        remember here what to call from onStart().
     */
    stateChange_t m_onStartDelegate;

    /**
     * @brief Delegate function that retrieves the start time. Executed
     *        by onStart().
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_startTimestampFunction;

    /**
     * @brief Acquisition start time. Retrieved during onStart() via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_startTime;

    // PVs
    std::shared_ptr<PVVariableInImpl<T> > m_dataPV;
    std::shared_ptr<PVVariableOutImpl<double> > m_frequencyPV;
    std::shared_ptr<PVVariableOutImpl<double> > m_durationPV;
    std::shared_ptr<PVVariableOutImpl<double> > m_amplitudePV;
    std::shared_ptr<PVVariableOutImpl<double> > m_offsetPV;
    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_decimationPV;
    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_samplingmodePV;
    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_groundPV;
    std::shared_ptr<StateMachineImpl> m_stateMachine;



};

}
#endif // NDSDATAACQUISITIONIMPL_H

