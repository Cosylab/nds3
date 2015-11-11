/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSPVDELEGATEOUT_H
#define NDSPVDELEGATEOUT_H

/**
 * @file pvDelegateOut.h
 *
 * @brief Defines the nds::PVDelegateOut class, an output PV that delegates the write
 *        operation to an user defined function.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "definitions.h"
#include "pvBaseOut.h"

#ifndef SWIG // PVDelegate will not be present in SWIG generated files

namespace nds
{

/**
 * @brief An output PV that delegates the read and write operations to two external
 *        functions.
 *
 * Calling PVDelegateOut::read() or PVDelegate::write() will result in a call
 *  to the delegated functions.
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
 *
 */
template <typename T>
class NDS3_API PVDelegateOut: public PVBaseOut
{
protected:
    PVDelegateOut();

public:
    /**
     * @ingroup datareadwrite
     * @brief Definition of the method used to read the initial PV value.
     *
     * The init function will receive two pointers to the timestamp and value
     *  that the function will have to fill with proper data.
     *
     */
    typedef std::function<void (timespec*, T*)> initValue_t;

    /**
     * @ingroup datareadwrite
     * @brief Definition of the method used to write.
     */
    typedef std::function<void (const timespec&, const T&)> write_t;

    /**
     * @brief Construct the PVDelegateOut object and specifies the external functions
     *        that must be called to write the data and to retrieve the initial value.
     *
     * The constructor also call processAtInit(true) in order to cause the call to
     *  the initValueFunction during the initialization.
     *
     * @param name           name of the PV
     * @param writeFunction  function to be used to write the value
     * @param initValueFunction function to be used for reading the initial value
     */
    PVDelegateOut(const std::string& name, write_t writeFunction, initValue_t initValueFunction);

    /**
     * @brief Construct the PVDelegateOut object and specifies the external function
     *        that must be called to write the data.
     *
     * @param name          name of the PV
     * @param writeFunction function to be used to write the value
     */
    PVDelegateOut(const std::string& name, write_t writeFunction);

#ifndef SWIG
private:
    initValue_t m_initializer;
    write_t m_writer;
#endif
};

}

#endif // SWIG
#endif // NDSPVDELEGATEIN_H
