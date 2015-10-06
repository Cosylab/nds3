#include "../include/nds3/dataAcquisition.h"
#include "dataAcquisitionImpl.h"

namespace nds
{

template <typename T>
DataAcquisition<T>::DataAcquisition(): Node()
{
}

/**
 * @brief Constructs the data acquisition node.
 *
 * @param name        the node name
 * @param maxElements if the data type is an array, then indicated
 *                    the maximum size (in elements) of the acquired array
 */
template <typename T>
DataAcquisition<T>::DataAcquisition(const std::string& name,
                recordType_t recordType,
                size_t maxElements,
                stateChange_t switchOnFunction,
                stateChange_t switchOffFunction,
                stateChange_t startFunction,
                stateChange_t stopFunction,
                stateChange_t recoverFunction,
                allowChange_t allowStateChangeFunction):
    Node(std::shared_ptr<DataAcquisitionImpl<T> >(new DataAcquisitionImpl<T>(name,
                                                                             recordType,
                                                                             maxElements,
                                                                             switchOnFunction,
                                                                             switchOffFunction,
                                                                             startFunction,
                                                                             stopFunction,
                                                                             recoverFunction,
                                                                             allowStateChangeFunction)))
{
}

template <typename T>
DataAcquisition<T>::DataAcquisition(const DataAcquisition<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
DataAcquisition<T>& DataAcquisition<T>::operator=(const DataAcquisition<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}


template <typename T>
void DataAcquisition<T>::pushData(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->pushData(timestamp, data);
}

template <typename T>
double DataAcquisition<T>::getFrequencyHz()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getFrequencyHz();
}

template <typename T>
double DataAcquisition<T>::getDurationSeconds()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getDurationSeconds();
}

template <typename T>
size_t DataAcquisition<T>::getMaxElements()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getMaxElements();
}

template <typename T>
size_t DataAcquisition<T>::getDecimation()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getDecimation();
}

template <typename T>
timespec DataAcquisition<T>::getStartTimestamp()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template class DataAcquisition<std::int32_t>;
template class DataAcquisition<double>;
template class DataAcquisition<std::vector<std::int8_t> >;
template class DataAcquisition<std::vector<std::uint8_t> >;
template class DataAcquisition<std::vector<std::int32_t> >;
template class DataAcquisition<std::vector<double> >;


}
