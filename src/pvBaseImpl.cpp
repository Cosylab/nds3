/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/port.h"
#include "nds3/pvBase.h"
#include "nds3/impl/pvBaseImpl.h"
#include "nds3/impl/portImpl.h"

namespace nds
{

/*
 * Constructor
 *
 *************/
PVBaseImpl::PVBaseImpl(const std::string& name): BaseImpl(name),
    m_scanType(scanType_t::passive),
    m_periodicScanSeconds(1),
    m_maxElements(1),
    m_bProcessAtInit(false)
{

}


/*
 * Read functions for all the supported data types
 *
 *************************************************/
void PVBaseImpl::read(timespec* /* pTimestamp */, std::int32_t* /* pValue */) const
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, double* /* pValue */) const
{
    throw;
}

void PVBaseImpl::read(timespec* pTimestamp, std::vector<std::int8_t>* pValue) const
{
    // TODO
    // Epics calls this also for unsigned-int.
    // If we arrive here maybe we really wanted to call the unsigned int function.
    // This is as ugly as it can get: consider modifying this
    read(pTimestamp, (std::vector<std::uint8_t>*) pValue);
}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::vector<std::uint8_t>* /* pValue */) const
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::vector<std::int32_t>* /* pValue */) const
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::vector<double>* /* pValue */) const
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::string* /* pValue */) const
{
    throw;
}


/*
 * Write functions for all the supported data types
 *
 **************************************************/
void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::int32_t& /* value */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const double& /* value */)
{
    throw;
}

void PVBaseImpl::write(const timespec& pTimestamp, const std::vector<std::int8_t>& value)
{
    // TODO
    // Epics calls this also for unsigned-int.
    // If we arrive here maybe we really wanted to call the unsigned int function.
    // This is as ugly as it can get: consider modifying this
    write(pTimestamp, (const std::vector<std::uint8_t>&) value);
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::vector<std::uint8_t>& /* value */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::vector<std::int32_t>& /* value */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::vector<double>& /* value */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::string& /* value */)
{
    throw;
}


/*
 * Set the description for the PV
 *
 ********************************/
void PVBaseImpl::setDescription(const std::string& description)
{
    m_description = description;
}


/*
 * Set the engineering units
 *
 ***************************/
void PVBaseImpl::setUnits(const std::string &units)
{
    m_units = units;
}


/*
 * Set the scan type
 *
 *******************/
void PVBaseImpl::setScanType(const scanType_t scanType, const double periodSeconds)
{
    m_scanType = scanType;
    m_periodicScanSeconds = periodSeconds;
}


/*
 * Set the maximum number of elements that can be written or read from the PV
 *
 ****************************************************************************/
void PVBaseImpl::setMaxElements(const size_t maxElements)
{
    m_maxElements = maxElements;
}


/*
 * Set the enumeration strings
 *
 *****************************/
void PVBaseImpl::setEnumeration(const enumerationStrings_t &enumerations)
{
    m_enumeration = enumerations;
}


/*
 * Specifies if the PV has to be processed during the device initialization
 *
 **************************************************************************/
void PVBaseImpl::processAtInit(const bool bProcessAtInit)
{
    m_bProcessAtInit = bProcessAtInit;
}


/*
 * Retrieve the description associated with the PV
 *
 *************************************************/
const std::string& PVBaseImpl::getDescription() const
{
    return m_description;
}


/*
 * Retrieve the engineering units
 *
 ********************************/
const std::string& PVBaseImpl::getUnits() const
{
    return m_units;
}


/*
 * Retrieve the PV's scan type
 *
 *****************************/
scanType_t PVBaseImpl::getScanType() const
{
    return m_scanType;
}


/*
 * Retrieve the polling period, in seconds
 *
 *****************************************/
double PVBaseImpl::getScanPeriodSeconds() const
{
    return m_periodicScanSeconds;
}


/*
 * Return the maximum number of elements that can be written or read in the PV
 *
 *****************************************************************************/
size_t PVBaseImpl::getMaxElements() const
{
    return m_maxElements;
}


/*
 * Return a reference to the enumeration strings
 *
 ***********************************************/
const enumerationStrings_t& PVBaseImpl::getEnumerations() const
{
    return m_enumeration;
}


/*
 * Return true if the PV has to be processed during the initialization of the device
 *
 ***********************************************************************************/
bool PVBaseImpl::getProcessAtInit() const
{
    return m_bProcessAtInit;
}


/*
 * Register the PV with the control system
 *
 *****************************************/
void PVBaseImpl::initialize(FactoryBaseImpl& controlSystem)
{
    BaseImpl::initialize(controlSystem);
    getPort()->registerPV(std::static_pointer_cast<PVBaseImpl>(shared_from_this()));
}


/*
 * Deregister the PV from the control system
 *
 *******************************************/
void PVBaseImpl::deinitialize()
{
    BaseImpl::deinitialize();
    getPort()->deregisterPV(std::static_pointer_cast<PVBaseImpl>(shared_from_this()));
}



}
