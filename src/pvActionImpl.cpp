/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include <type_traits>
#include "nds3/impl/pvActionImpl.h"
namespace nds
{

/*
 * Constructor (with initialization function)
 *
 ********************************************/
PVActionImpl::PVActionImpl(const std::string& name, write_t writeFunction, initValue_t initValueFunction, outputPvType_t pvType): PVBaseOutImpl(name, pvType),
    m_writer(writeFunction),
    m_initializer(initValueFunction)
{
    // Since we have a read method that is used to read the initial value then we
    //  set the "ProcessAtInit" flag
    /////////////////////////////////////////////////////////////////////////////
    processAtInit(true);
}


/*
 * Constructor (without initialization function)
 *
 ***********************************************/
PVActionImpl::PVActionImpl(const std::string& name, write_t writeFunction, const outputPvType_t pvType): PVBaseOutImpl(name, pvType),
    m_writer(writeFunction),
    m_initializer(std::bind(&PVActionImpl::dontInitialize, this, std::placeholders::_1, std::placeholders::_2))
{
    processAtInit(false);
}


/*
 * Called to read the initial value
 *
 **********************************/
void PVActionImpl::read(timespec* pTimestamp, std::int32_t* pValue) const
{
    m_initializer(pTimestamp, pValue);
}


/*
 * Called to write a value in the PV
 *
 ***********************************/
void PVActionImpl::write(const timespec& timestamp, const std::int32_t& value)
{
    m_writer(timestamp, value);
}


/*
 * Return the PV's data type
 *
 ***************************/
dataType_t PVActionImpl::getDataType() const
{
    return getDataTypeForCPPType<std::int32_t>();
}

/*
 * Called to set value back to the control system
 *
 ***************************/
void PVActionImpl::setValueBack(const timespec& timestamp, const std::int32_t& value)
{
    if(m_pAcknowledgePV)
    {
        m_pAcknowledgePV->push(timestamp, value);
    }
}

/*
 * Set the related acknowledge pv.
 *
 ***************************/
void PVActionImpl::setAcknowledgePV(PVBaseInImpl* pv)
{
    m_pAcknowledgePV = pv;
}


/*
 * Called to read a value when the reading function has not been declared
 *
 ************************************************************************/
void PVActionImpl::dontInitialize(timespec*, std::int32_t*)
{

}

}
