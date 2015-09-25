#include "dataAcquisitionImpl.h"
#include "stateMachineImpl.h"
#include "../include/nds3/definitions.h"
namespace nds
{

template<typename T>
DataAcquisitionImpl<T>::DataAcquisitionImpl(
        const std::string& name,
        recordType_t recordType,
        size_t maxElements,
        stateChange_t switchOnFunction,
        stateChange_t switchOffFunction,
        stateChange_t startFunction,
        stateChange_t stopFunction,
        stateChange_t recoverFunction,
        allowChange_t allowStateChangeFunction):
    NodeImpl(name),
    m_onStartDelegate(startFunction),
    m_startTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
    // Add the children PVs
    m_dataPV.reset(new PVVariableImpl<T>("Data"));
    m_dataPV->setMaxElements(maxElements);
    m_dataPV->setDescription("Acquired data");
    m_dataPV->setScanType(scanType_t::interrupt, 0);
    m_dataPV->setType(recordType);
    addChild(m_dataPV);

    m_frequencyPV.reset(new PVVariableImpl<double>("Frequency"));
    m_frequencyPV->setDescription("Acquisition frequency");
    m_frequencyPV->setScanType(scanType_t::passive, 0);
    m_frequencyPV->setType(recordType_t::ai);
    m_frequencyPV->write(getTimestamp(), (double)1);
    addChild(m_frequencyPV);

    m_durationPV.reset(new PVVariableImpl<double>("Duration"));
    m_durationPV->setDescription("Acquisition duration");
    m_durationPV->setScanType(scanType_t::passive, 0);
    m_durationPV->setType(recordType_t::ai);
    addChild(m_durationPV);

    m_maxElementsPV.reset(new PVVariableImpl<std::int32_t>("maxLength"));
    m_maxElementsPV->setDescription("Number of element in acquisition array");
    m_maxElementsPV->setScanType(scanType_t::passive, 0);
    m_maxElementsPV->setType(recordType_t::longin);
    m_maxElementsPV->write(getTimestamp(), (std::int32_t)maxElements);
    addChild(m_maxElementsPV);

    m_decimationPV.reset(new PVVariableImpl<std::int32_t>("decimation"));
    m_decimationPV->setDescription("Decimation");
    m_decimationPV->setScanType(scanType_t::passive, 0);
    m_decimationPV->setType(recordType_t::longin);
    m_decimationPV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_decimationPV);

    // Add state machine
    m_stateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&DataAcquisitionImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_stateMachine);
}

template<typename T>
double DataAcquisitionImpl<T>::getFrequencyHz()
{
    double frequency;
    timespec timestamp;
    m_frequencyPV->read(&timestamp, &frequency);
    return frequency;
 }

template<typename T>
double DataAcquisitionImpl<T>::getDurationSeconds()
{
    double duration;
    timespec timestamp;
    m_durationPV->read(&timestamp, &duration);
    return duration;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getMaxElements()
{
    std::int32_t maxElements;
    timespec timestamp;
    m_maxElementsPV->read(&timestamp, &maxElements);
    return (size_t)maxElements;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getDecimation()
{
    std::int32_t decimation;
    timespec timestamp;
    m_decimationPV->read(&timestamp, &decimation);
    return (size_t)decimation;
}

template<typename T>
timespec DataAcquisitionImpl<T>::getStartTimestamp()
{
    return m_startTime;
}

template<typename T>
void DataAcquisitionImpl<T>::pushData(const timespec& timestamp, const T& data)
{
    m_dataPV->push(timestamp, data);
}

template<typename T>
void DataAcquisitionImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    m_onStartDelegate();
}


template class DataAcquisitionImpl<std::int32_t>;
template class DataAcquisitionImpl<double>;
template class DataAcquisitionImpl<std::vector<std::int32_t> >;


}
