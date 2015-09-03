#ifndef NDSDATAACQUISITION_H
#define NDSDATAACQUISITION_H

#include "definitions.h"
#include "ndsnode.h"

namespace nds
{

/**
 * This is a node that supplies a data acquisition PV and few control
 * PV that specifies how the acquisition should be performed.
 *
 * It also provides a state machine that allows to start/stop the acquisition.
 */
template <typename T>
class DataAcquisition: public Node
{
protected:
    DataAcquisition();

public:
    /**
     * @brief Constructs the data acquisition node.
     *
     * @param name        the node name
     * @param maxElements if the data type is an array, then indicated
     *                    the maximum size (in elements) of the acquired array
     */
    DataAcquisition(const std::string& name,
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
};

}
#endif // NDSDATAACQUISITION_H

