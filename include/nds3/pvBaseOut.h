/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSPVBASEOUT_H
#define NDSPVBASEOUT_H

/**
 * @file pvBaseOut.h
 *
 * @brief Defines the base class for the output PVs.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "pvBase.h"

namespace nds
{

class PVBaseOutImpl;

/**
 * @brief Represents an output PV.
 *
 * Output PVs are written by the control system and read by the device support.
 *
 */
class NDS3_API PVBaseOut: public PVBase
{
public:
    /**
     * @brief Construct an empty PVBaseOut.
     *
     * @warning Assign a PVDelegateOut or a PVVariableOut to this object before initializing
     *          the parent root node.
     */
    PVBaseOut();

#ifndef SWIG
    PVBaseOut(std::shared_ptr<PVBaseOutImpl> pvImpl);
#endif

    /**
     * @ingroup datareadwrite
     * @brief Call to read the value of the PV.
     *
     * If this object manages a delegated PV then the delegated function
     * will be called.
     *
     * @tparam T           the data type to read
     * @param pTimestamp   a pointer to a timestamp value that the called function will fill with
     *                     the proper timestamp
     * @param pValue       a pointer to a variable that the called function will fill with the
     *                     proper value
     */
    template<typename T>
    void read(timespec* pTimestamp, T* pValue) const;

    /**
     * @ingroup datareadwrite
     * @brief Call to write the value into the PV.
     *
     * If this object manages a delegated PV then the delegated function
     * will be called.
     *
     * @tparam T           the data type to write
     * @param timestamp    the timestamp to assign to the new value
     * @param value       the new value for the PV
     */
    template<typename T>
    void write(const timespec& timestamp, const T& value);

    /**
     * @brief Subscribe the PV to an input PV which may be located on any other
     *         device running in the same NDS process.
     *
     * When the input PV will be written by the device via a PVBaseIn::push() operation
     *  or via a PVVariableIn::setValue() then the input PV will forward the value
     *  to this PV by calling PVBaseOut::write().
     *
     * @param inputPVName the name of the input PV from which we want to
     *                     receive the values
     */
    void subscribeTo(const std::string& inputPVName);

};

}

#endif // NDSPVBASEOUT_H
