/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSPVVARIABLEOUT_H
#define NDSPVVARIABLEOUT_H

/**
 * @file pvVariableOut.h
 *
 * @brief Defines the nds::PVVariableOut class, an outputput PV that stores the values
 *        received from the control system.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "definitions.h"
#include "pvBaseOut.h"

namespace nds
{

/**
 * @brief An output PV object that also stores the value so it does not need to
 *        delegate the read and write functions.
 *
 * The application can use PVVariableOut::getValue()to retrieve the PV's value;
 * the control system will use the methods read() and write() to read and set the value.
 *
 * @warning for all the vector data types and for std::string remember to call
 *          setMaxElements() to specify the maximum size of the vector or string.
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
class NDS3_API PVVariableOut: public PVBaseOut
{
public:


    /**
     * @brief Initializes an empty PVVariableOut PV.
     *
     * You must assign a valid PVVariableOut PV before calling Node::initialize() on the root node.
     */
    PVVariableOut();

    /**
     * @brief Construct the PVVariableOut object.
     *
     * @param name          name of the PV
     */
    PVVariableOut(const std::string& name);

    /**
     * @ingroup datareadwrite
     * @brief Retrieve the value stored in the PV.
     *
     * @return the value stored in the PV.
     */
    T getValue() const;

    /**
     * @ingroup datareadwrite
     * @brief Retrieve the value and the timestamp stored in the PV.
     *
     * @param pTime  pointer to a timespec structure taht will be filled with the PV's timestamp
     * @param pValue pointer to a variable that will be filled with the PV's value
     */
    void getValue(timespec* pTime, T* pValue) const;
};

}

#endif // NDSPVVARIABLEOUT_H
