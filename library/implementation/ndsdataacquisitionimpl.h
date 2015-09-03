#ifndef NDSDATAACQUISITIONIMPL_H
#define NDSDATAACQUISITIONIMPL_H

#include "ndsnodeimpl.h"

namespace nds
{

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

    pushData(const timespec& timestamp, const T& data);

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
    timespec getLocalTimestamp();

    stateChange_t m_onStartDelegate;

    allowChange_t m_allowChangeDelegate;

    getTimestampPlugin_t m_getTimestampFunction;

    timespec m_startTime;


};

}
#endif // NDSDATAACQUISITIONIMPL_H

