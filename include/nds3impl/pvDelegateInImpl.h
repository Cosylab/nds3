/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSDELEGATEPVINIMPL_H
#define NDSDELEGATEPVINIMPL_H

#include "pvBaseInImpl.h"
#include "../nds3/definitions.h"

namespace nds
{

/**
 * @brief Delegating Input PV. Delegates the read operation to an user defined function.
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
class PVDelegateInImpl: public PVBaseInImpl
{
public:
    /**
     * @brief Definition of the method used to read the value.
     *
     * The read function will receive a pointer to a timespec and a pointer to
     *  a value: the function will have to fill both the timespec and value
     *  with proper data.
     */
    typedef std::function<void (timespec*, T*)> read_t;

    /**
     * @brief Constructor for read-only PVs. Specifies the method used for reading the value.
     *
     * @param name          PV's name
     * @param readFunction  read method
     */
    PVDelegateInImpl(const std::string& name, read_t readFunction, const inputPvType_t pvType = inputPvType_t::generic);

    /**
     * @brief Called when the control system wants to read a value.
     *
     * Internally it calls the read method specified in the constructor.
     *
     * @param pTimestamp a pointer to a variable that will be filled with the timestamp
     * @param pValue     a pointer to a value that will be filled with the value
     */
    virtual void read(timespec* pTimestamp, T* pValue) const;

    /**
     * @brief Return the PV's data type.
     *
     * @return the PV's data type
     */
    virtual dataType_t getDataType() const;


private:
    read_t m_reader; ///< The method used to read the value.

};

}
#endif // NDSDELEGATEPVINIMPL_H

