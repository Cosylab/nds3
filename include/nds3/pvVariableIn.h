/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSPVVARIABLEIN_H
#define NDSPVVARIABLEIN_H

/**
 * @file pvVariableIn.h
 *
 * @brief Defines the nds::PVVariableIn class, an input PV that stores the values
 *        to be supplied to the control system.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "definitions.h"
#include "pvBaseIn.h"

namespace nds
{

/**
 * @brief An input PV object that also stores the value so it does not need to
 *        delegate the read function.
 *
 * The device support can use PVVariableIn::setValue() to modify the PV's value;
 * the read() method will be used to read the value on the control system side.
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
class NDS3_API PVVariableIn: public PVBaseIn
{
public:

    /**
     * @brief Initializes an empty PVVariableIn PV.
     *
     * You must assign a valid PVVariableIn PV before calling Node::initialize() on the root node.
     */
    PVVariableIn();

    /**
     * @brief Construct the PVVariableIn object.
     *
     * @param name          name of the PV
     */
    PVVariableIn(const std::string& name);

    /**
     * @ingroup datareadwrite
     * @brief Set the variable's value.
     *
     * The control system will retrieve the value when it choses to do so.
     *
     * If one or more output PVs have been suscribed to this PV via Factory::subscribe()
     * then the value will be pushed immediately to all the subscribed PVs via PVBaseOut::write().
     *
     * See also Factory::subscribe() and PVBaseOut::subscribeTo().
     *
     * @param value the value to write into the variable. The timestamp will
     *              be taken via the getTimestamp() method.
     */
    void setValue(const T& value);

    /**
     * @ingroup datareadwrite
     * @brief Set the variable's value.
     *
     * The control system will retrieve the value when it choses to do so.
     *
     * If one or more output PVs have been suscribed to this PV via Factory::subscribe()
     * then the value will be pushed immediately to all the subscribed PVs via PVBaseOut::write().
     *
     * See also Factory::subscribe() and PVBaseOut::subscribeTo().
     *
     * @param timestamp the timestamp to assign to the variable
     * @param value     the value to write into the variable
     */
    void setValue(const timespec& timestamp, const T& value);
};

}

#endif // NDSPVVARIABLEIN_H
