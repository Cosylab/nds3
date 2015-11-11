/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSPVVARIABLEINIMPL_H
#define NDSPVVARIABLEINIMPL_H

#include <mutex>
#include "pvBaseInImpl.h"

namespace nds
{

/**
 * @brief Implementation of an input PV object that also stores the value so
 *        it does not need to delegate the read function.
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
class PVVariableInImpl: public PVBaseInImpl
{
public:
    /**
     * @brief Constructor.
     *
     * @param name the PV name
     */
    PVVariableInImpl(const std::string& name, const inputPvType_t pvType = inputPvType_t::generic);

    /**
     * @brief Called by the control system to read the value stored in the PV
     *
     * @param pTimestamp pointer to a variable that will be filled with the stored timestamp
     * @param pValue     pointer to a variable that will be filled with the stored value
     */
    virtual void read(timespec* pTimestamp, T* pValue) const;

    /**
     * @brief Return the PV data type
     *
     * @return the data type of the stored value
     */
    virtual dataType_t getDataType() const;

    /**
     * @brief Store a value in the PV. The timestamp is set to the current time.
     *
     * If output PVs are subscribers of this PV then the new value will be pushed
     *  to the subscribers in the same thread that call this function.
     *
     * @param value     value to store into the PV
     */
    void setValue(const T& value);

    /**
     * @brief Store a value and its timestamp into the PV.
     *
     * If output PVs are subscribers of this PV then the new value will be pushed
     *  to the subscribers in the same thread that call this function.
     *
     * @param timestamp timestamp related to the value
     * @param value     value to store in the PV
     */
    void setValue(const timespec& timestamp, const T& value);

private:
    T m_value;
    timespec m_timestamp;

    mutable std::mutex m_pvMutex;

};

}
#endif // NDSPVVARIABLEINIMPL_H
