#ifndef NDSDATAACQUISITIONIMPL_H
#define NDSDATAACQUISITIONIMPL_H

#include "nodeImpl.h"
#include "pvVariableImpl.h"
#include "../include/nds3/definitions.h"
#include <memory>
namespace nds
{

template<typename T>
class DataAcquisitionImpl: public NodeImpl
{
public:
    DataAcquisitionImpl(const std::string& name,
                    recordType_t recordType,
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

    void pushData(const timespec& timestamp, const T& data);

    double getFrequencyHz();
    double getDurationSeconds();
    size_t getMaxElements();
    size_t getDecimation();

    /**
     * @brief Returns the timestamp at the moment of the start of the acquisition.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when the acquisition started.
     */
    timespec getStartTimestamp();

    /**
     * @brief Called by the state machine. Store the current timestamp and then calls the
     *        delegated onStart function.
     */
    void onStart();


protected:
    // Delegate functions
    /////////////////////
    stateChange_t m_onStartDelegate;

    getTimestampPlugin_t m_startTimestampFunction;

    timespec m_startTime;

    // PVs
    std::shared_ptr<PVVariableImpl<T> > m_dataPV;
    std::shared_ptr<PVVariableImpl<double> > m_frequencyPV;
    std::shared_ptr<PVVariableImpl<double> > m_durationPV;
    std::shared_ptr<PVVariableImpl<std::int32_t> > m_maxElementsPV;
    std::shared_ptr<PVVariableImpl<std::int32_t> > m_decimationPV;
    std::shared_ptr<StateMachineImpl> m_stateMachine;



};

}
#endif // NDSDATAACQUISITIONIMPL_H

