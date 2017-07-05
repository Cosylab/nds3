/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSPVACTIONIMPL_H
#define NDSPVACTIONIMPL_H

#include "nds3/impl/pvBaseOutImpl.h"
#include "nds3/impl/pvBaseInImpl.h"
#include "nds3/definitions.h"

namespace nds
{

/**
 * @brief Action PV. Delegates the read/write user defined functions
 * and set the value back to the control system.
 */
class NDS3_API PVActionImpl: public PVBaseOutImpl
{
public:
    /**
     * @brief Definition of the method used to read the initial value.
     */
    typedef std::function<void (timespec*, std::int32_t*)> initValue_t;

    /**
     * @brief Definition of the method used to write.
     */
    typedef std::function<void (const timespec&, const std::int32_t&)> write_t;

    /**
     * @brief Constructor. Specifies the methods used for read/write
     *
     * @param name          PV's name
     * @param readFunction  read method
     * @param writeFunction write method
     */
    PVActionImpl(const std::string& name, write_t writeFunction, initValue_t initValueFunction, const outputPvType_t pvType = outputPvType_t::generic);

    /**
     * @brief Constructor. Specifies the method used to write.
     *
     * The read method is not specified and does nothing if the control system tries to call it.
     *
     * @param name          the PV's name
     * @param writeFunction write method
     */
    PVActionImpl(const std::string& name, write_t writeFunction, const outputPvType_t pvType = outputPvType_t::generic);

    /**
     * @brief Called when the control system wants to read the value.
     *
     * Internally it calls the delegate read method.
     *
     * @param pTimestamp a variable that will be filled with the correct timestamp
     * @param pValue     a variable that will be filled with the correct value
     */
    virtual void read(timespec* pTimestamp, std::int32_t* pValue) const;

    /**
     * @brief Called when the control system wants to write a value.
     *
     * Internally it calls the delegate write method.
     *
     * @param timestamp timestamp related to the new value
     * @param value     new value for the PV
     */
    virtual void write(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Returns the PV's data type.
     *
     * @return the PV's data type
     */
    virtual dataType_t getDataType() const;

    /**
     * @brief Called when wants to set a value back to the control system.
     *
     * @param timestamp  timestamp related to the new value
     * @param value      value to set back to the control system
     */
    void setValueBack(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Set the related acknowledge pv.
     *
     * @param pv    the related acknowledge pv
     */
    void setAcknowledgePV(PVBaseInImpl* pv);


private:
    write_t m_writer;          ///< Method used to write the value
    initValue_t m_initializer; ///< Method used to read the initial value

    /**
     * @brief Delegate method used when the read method is not specified.
     *        It does nothing.
     */
    void dontInitialize(timespec*, std::int32_t*);

    PVBaseInImpl* m_pAcknowledgePV;

};

}
#endif // NDSPVACTIONIMPL_H
