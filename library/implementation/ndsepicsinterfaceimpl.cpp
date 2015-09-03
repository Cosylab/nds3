#include "ndsepicsinterfaceimpl.h"
#include "ndspvbaseimpl.h"
#include "ndsportImpl.h"
#include "../include/nds3/ndspvbase.h"

#include <cstdint>
#include <sstream>
#include <ostream>
#include <fstream>

#include <memory.h>

namespace nds
{

EpicsInterfaceImpl::EpicsInterfaceImpl(const std::string& portName): InterfaceBaseImpl(portName), asynPortDriver(
                                                               portName.c_str(),
                                                               256,    /* maxAddr */
                                                               0,
                                                               //asynCommonMask |
                                                               asynDrvUserMask |
                                                               //asynOptionMask |
                                                               asynInt32Mask |
                                                               //asynUInt32DigitalMask |
                                                               asynFloat64Mask  |
                                                               //asynOctetMask |
                                                               //asynInt8ArrayMask |
                                                               //asynInt16ArrayMask |
                                                               asynInt32ArrayMask //|
                                                               //asynFloat32ArrayMask |
                                                               //asynFloat64ArrayMask |
                                                               //asynGenericPointerMask,   /* Interface mask */
                                              ,
                                                               asynInt32Mask |
                                                               //asynUInt32DigitalMask |
                                                               asynFloat64Mask |
                                                               //asynOctetMask |
                                                               //asynInt8ArrayMask |
                                                               //asynInt16ArrayMask |
                                                               asynInt32ArrayMask //|
                                                               //asynFloat32ArrayMask |
                                                               //asynFloat64ArrayMask |
                                                               //asynGenericPointerMask,            /* Interrupt mask */
                                                               , ASYN_CANBLOCK | ASYN_MULTIDEVICE,  /* asynFlags. */
                                                               1,                                 /* Autoconnect */
                                                               0,                                 /* Default priority */
                                                               0)
{
}


/*
 * Convert a data type from enum to string.
 * We use a switch and not a map for the conversion so we receive warnings
 *  if we forget a conversion.
 *
 *************************************************************************/
typedef std::pair<std::string, std::string> dataTypeAndFTVL_t;
dataTypeAndFTVL_t dataTypeToEpicsString(dataType_t dataType, bool bInput)
{
    switch(dataType)
    {
    case dataInt32:
        return dataTypeAndFTVL_t("asynInt32", "");
    case dataFloat64:
        return dataTypeAndFTVL_t("asynFloat64", "");
    case dataInt32Array:
        if(bInput)
        {
            return dataTypeAndFTVL_t("asynInt32ArrayIn", "LONG");
        }
        else
        {
            return dataTypeAndFTVL_t("asynInt32ArrayOut", "LONG");
        }
    }
}


/*
 * Convert a PV type from enum to string.
 *
 * Here we use a switch statement instead of a table so we receive a
 * warning when we forget to check for a value.
 *
 *******************************************************************/
typedef std::pair<std::string, bool> typeAndInput_t;
typeAndInput_t recordTypeToEpicsString(recordType_t recordType)
{
    switch(recordType)
    {
    case aai:
        return typeAndInput_t("aai", true);
    case aao:
        return typeAndInput_t("aao", false);
    case ai:
        return typeAndInput_t("ai", true);
    case  ao:
        return typeAndInput_t("ao", false);
    case bi:
        return typeAndInput_t("bi", true);
    case bo:
        return typeAndInput_t("bo", false);
    case longin:
        return typeAndInput_t("longin", true);
    case longout:
        return typeAndInput_t("longout", false);
    case mbbi:
        return typeAndInput_t("mbbi", true);
    case mbbo:
        return typeAndInput_t("mbbo", false);
    case mbbiDirect:
        return typeAndInput_t("mbbiDirect", true);
    case mbboDirect:
        return typeAndInput_t("mbboDirect", false);
    case stringIn:
        return typeAndInput_t("stringin", true);
    case stringOut:
        return typeAndInput_t("stringout", false);
    case waveformIn:
        return typeAndInput_t("waveform", true);
    case waveformOut:
        return typeAndInput_t("waveform", false);
    }
}


/*
 * Register a PV
 *
 ***************/
void EpicsInterfaceImpl::registerPV(std::shared_ptr<PVBaseImpl> pv)
{
    // Save the PV in a list. The order in the is used as "reason".
    ///////////////////////////////////////////////////////////////
    m_pvs.push_back(pv);
    m_pvNameToReason[pv->getFullNameFromPort()] = m_pvs.size() - 1;

    // If the record type is specified then attempt to auto generate a db file
    //////////////////////////////////////////////////////////////////////////
    recordType_t type = pv->getType();
    if(type != notSpecified)
    {
        typeAndInput_t recordTypeAndInput = recordTypeToEpicsString(type);

        int portAddress(0);
        std::ostringstream dbEntry;

        std::string externalName(pv->getFullName());
        if(!pv->getInterfaceName().empty())
        {
            externalName = pv->getPort()->getFullName() + "-" + pv->getInterfaceName();
        }

        std::ostringstream scanType;

        switch(pv->getScanType())
        {
        case passive:
            scanType << "Passive";
            break;
        case periodic:
            scanType << pv->getScanPeriodSeconds() << " second";
            break;
        case interrupt:
            scanType << "I/O Intr";
            break;
        }

        dataTypeAndFTVL_t dataTypeAndFTVL = dataTypeToEpicsString(pv->getDataType(), recordTypeAndInput.second);
        size_t maxElements(pv->getMaxElements());
        dbEntry << "record(" << recordTypeAndInput.first << ", \"" << externalName << "\") {" << std::endl;
        dbEntry << "    field(DESC, \"" << pv->getDescription() << "\")" << std::endl;
        dbEntry << "    field(DTYP, \"" << dataTypeAndFTVL.first << "\")" << std::endl;

        if(!dataTypeAndFTVL.second.empty())
        {
            dbEntry << "    field(FTVL, \"" << dataTypeAndFTVL.second << "\")" << std::endl;
        }
        if(maxElements > 1)
        {
            dbEntry << "    field(NELM, " << maxElements << ")" << std::endl;
        }
        dbEntry << "    field(SCAN, \"" << scanType.str() << "\")" << std::endl;

        // Add INP/OUT fields
        if(recordTypeAndInput.second)
        {
            dbEntry << "    field(INP, \"@asyn(" << pv->getPort()->getFullName() << ", " << portAddress<< ")" << pv->getFullNameFromPort() << "\")" << std::endl;
        }
        else
        {
            dbEntry << "    field(OUT, \"@asyn(" << pv->getPort()->getFullName() << ", " << portAddress<< ")" << pv->getFullNameFromPort() << "\")" << std::endl;
        }

        dbEntry << "}" << std::endl << std::endl;
        dbEntry.flush();

        m_autogeneratedDB += dbEntry.str();
    }
}

void EpicsInterfaceImpl::registrationTerminated()
{
    // Save the records in a temporary file
    ///////////////////////////////////////
    std::string fileName("auto_generated_" + m_portName + ".db");
    std::ofstream outputStream(fileName.c_str());
    outputStream << m_autogeneratedDB;
    outputStream.flush();
}

void EpicsInterfaceImpl::push(const timespec& timestamp, std::shared_ptr<PVBaseImpl> pv)
{
    // Locate reason


    switch(pv->getDataType())
    {
    case
    }
}

void EpicsInterfaceImpl::push(const timespec& timestamp, std::shared_ptr<PVBaseImpl> pv, const std::int32_t& value)
{

}

void push(const timespec& timestamp, std::shared_ptr<PVBaseImpl> pv, const double& value);
void push(const timespec& timestamp, std::shared_ptr<PVBaseImpl> pv, const std::vector<std::int32_t> & value);




asynStatus EpicsInterfaceImpl::readInt32(asynUser *pasynUser, epicsInt32 *value)
{
    timespec timestamp;
    if(pasynUser->timestamp.secPastEpoch == 0 && pasynUser->timestamp.nsec == 0)
    {
        clock_gettime(CLOCK_MONOTONIC, &timestamp);
    }
    m_pvs[pasynUser->reason]->read(&timestamp, (std::int32_t*)value);

    return asynSuccess;
}

asynStatus EpicsInterfaceImpl::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    timespec timestamp;

    m_pvs[pasynUser->reason]->write(timestamp,(std::int32_t) value);
    return asynSuccess;
}

asynStatus EpicsInterfaceImpl::readFloat64(asynUser *pasynUser, epicsFloat64 *value)
{
    timespec timestamp;
    if(pasynUser->timestamp.secPastEpoch == 0 && pasynUser->timestamp.nsec == 0)
    {
        clock_gettime(CLOCK_MONOTONIC, &timestamp);
    }
    m_pvs[pasynUser->reason]->read(&timestamp, (double*)value);

    return asynSuccess;
}

asynStatus EpicsInterfaceImpl::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
    timespec timestamp;

    m_pvs[pasynUser->reason]->write(timestamp,(double) value);
    return asynSuccess;
}

asynStatus EpicsInterfaceImpl::readInt32Array(asynUser *pasynUser, epicsInt32 *value,
                                              size_t nElements, size_t *nIn)
{
    timespec timestamp;
    if(pasynUser->timestamp.secPastEpoch == 0 && pasynUser->timestamp.nsec == 0)
    {
        clock_gettime(CLOCK_MONOTONIC, &timestamp);
    }
    std::vector<std::int32_t> vector(nElements);
    m_pvs[pasynUser->reason]->read(&timestamp, &vector);

    if(vector.size() > nElements)
    {
        vector.resize(nElements);
    }
    *nIn = vector.size();
    ::memcpy(value, vector.data(), vector.size() * sizeof(std::int32_t));

    return asynSuccess;
}

asynStatus EpicsInterfaceImpl::writeInt32Array(asynUser *pasynUser, epicsInt32 *value,
                                               size_t nElements)
{
    timespec timestamp;

    std::vector<std::int32_t> vector(nElements);
    ::memcpy(vector.data(), value, vector.size() * sizeof(std::int32_t));

    m_pvs[pasynUser->reason]->write(timestamp, vector);
    return asynSuccess;
}





asynStatus EpicsInterfaceImpl::drvUserCreate(asynUser *pasynUser, const char *drvInfo,
                                 const char **pptypeName, size_t *psize)
{
    for(size_t scanReasons(0), endReasons(m_pvs.size()); scanReasons != endReasons; ++scanReasons)
    {
        if(m_pvs[scanReasons]->getFullNameFromPort() == drvInfo)
        {
            pasynUser->reason = scanReasons;
            pasynUser->userData = m_pvs[scanReasons].get();
            return asynSuccess;
        }
    }
    return asynError;
}

timespec EpicsInterfaceImpl::convertEpicsTimeToUnixTime(const epicsTimeStamp& time)
{
    timespec unixTime;

    return unixTime;

}

epicsTimeStamp EpicsInterfaceImpl::convertUnixTimeToEpicsTime(const timespec& time)
{
    epicsTimeStamp epicsTime;
    return epicsTime;

}

}
