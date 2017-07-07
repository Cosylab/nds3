/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSPVACTION_H
#define NDSPVACTION_H

/**
 * @file pvAction.h
 *
 * @brief Defines the nds::PVAction class, an output PV that simulate an action.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/pvBaseOut.h"

#ifndef SWIG // PVAction will not be present in SWIG generated files

namespace nds
{

/**
 * @brief An output PV that delegates the read and write operations to two external
 *        functions.
 *
 * Calling PVAction::read() or PVDelegate::write() will result in a call
 *  to the delegated functions.
 *
 */
class NDS3_API PVAction: public PVBaseOut
{
public:
    /**
     * @ingroup datareadwrite
     * @brief Definition of the method used to read the initial PV value.
     *
     * The init function will receive two pointers to the timestamp and value
     *  that the function will have to fill with proper data.
     *
     */
    typedef std::function<void (timespec*, std::int32_t*)> initValue_t;

    /**
     * @ingroup datareadwrite
     * @brief Definition of the method used to write.
     */
    typedef std::function<void (const timespec&, const std::int32_t&)> write_t;

    /**
     * @brief Initializes an empty PVAction PV.
     *
     * You must assign a valid PVAction PV before calling Node::initialize() on the root node.
     */
    PVAction();

    /**
     * @brief Construct the PVAction object and specifies the external functions
     *        that must be called to write the data and to retrieve the initial value.
     *
     * The constructor also call processAtInit(true) in order to cause the call to
     *  the initValueFunction during the initialization.
     *
     * @param name           name of the PV
     * @param writeFunction  function to be used to write the value
     * @param initValueFunction function to be used for reading the initial value
     */
    PVAction(const std::string& name, write_t writeFunction, initValue_t initValueFunction);

    /**
     * @brief Construct the PVAction object and specifies the external function
     *        that must be called to write the data.
     *
     * @param name          name of the PV
     * @param writeFunction function to be used to write the value
     */
    PVAction(const std::string& name, write_t writeFunction);

    /**
     * @brief Function to set the value back to the control system.
     *
     * @param timestamp         timestamp related to the new value
     * @param value 			value to set back
     */
    void setValueBack(const timespec& timestamp, const std::int32_t& value);

#ifndef SWIG
private:
    initValue_t m_initializer;
    write_t m_writer;
#endif
};

}

#endif // SWIG
#endif // NDSPVACTION_H
