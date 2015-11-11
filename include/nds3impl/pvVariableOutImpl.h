/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSPVVARIABLEOUTIMPL_H
#define NDSPVVARIABLEOUTIMPL_H

#include <mutex>
#include "pvBaseOutImpl.h"

namespace nds
{

/**
 * @brief Implementation of an output PV object that also stores the value so it does
 *         not need to delegate the read and write functions.
 *
 * @tparam T  the PV data type.
 *            The following data types are supported:
 *            - std::int32_t
 *            - std::double
 *            - std::vector<std::int8_t>
 *            - std::vector<std::uint8_t>
 *            - std::vector<std::int32_t>
 *            - std::vector<double>
 *            - std::string
 */
template <typename T>
class PVVariableOutImpl: public PVBaseOutImpl
{
public:
    /**
     * @brief Constructor.
     *
     * @param name the name of the PV
     */
    PVVariableOutImpl(const std::string& name, const outputPvType_t pvType = outputPvType_t::generic);

    /**
     * @brief Called when the control system wants to read a value from the PV.
     *
     * @param pTimestamp pointer to a variable that will be filled with the timestamp of
     *                   the stored value
     * @param pValue     pointer to a variable that will be filled with the stored value
     */
    virtual void read(timespec* pTimestamp, T* pValue) const;

    /**
     * @brief Called when the control system wants to write a value into the PV.
     *
     * @param timestamp the timestamp to store in the PV
     * @param value     the value to store in the PV
     */
    virtual void write(const timespec& timestamp, const T& value);

    /**
     * @brief Return the data type of the PV.
     * @return an enumeration representing the data type
     */
    virtual dataType_t getDataType() const;

    /**
     * @brief Call to retrieve the value stored in the PV
     *
     * @return the value stored in the PV
     */
    T getValue() const;

    /**
     * @brief Retrieve the value and the timestamp stored in the PV.
     *
     * @param pTime  the timestamp stored in the PV
     * @param pValue the value stored in the PV
     */
    void getValue(timespec* pTime, T* pValue) const;

private:
    T m_value;            ///< Value stored in the PV
    timespec m_timestamp; ///< Timestamp stored in the PV

    mutable std::mutex m_pvMutex; ///< Mutex used to synchronize the access to m_value and m_timestamp

};

}
#endif // NDSPVVARIABLEOUTIMPL_H

