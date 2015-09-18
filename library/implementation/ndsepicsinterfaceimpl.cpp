#ifdef NDS3_EPICS

#include "ndsepicsinterfaceimpl.h"
#include "ndspvbaseimpl.h"
#include "ndsportImpl.h"
#include "../include/nds3/ndspvbase.h"
#include "../include/nds3/ndsexceptions.h"
#include <cstdint>
#include <sstream>
#include <ostream>
#include <fstream>
#include <stdexcept>
#include <memory.h>

#include "iocsh.h"

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
 *
 * We use a switch and not a map for the conversion so we receive warnings
 *  if we forget a conversion.
 *
 *************************************************************************/
typedef std::pair<std::string, std::string> dataTypeAndFTVL_t;
dataTypeAndFTVL_t dataTypeToEpicsString(dataType_t dataType, bool bInput)
{
    switch(dataType)
    {
    case dataType_t::dataInt8:
        return dataTypeAndFTVL_t("asynOctet", "CHAR");
    case dataType_t::dataUint8:
        return dataTypeAndFTVL_t("asynOctet", "UCHAR");
    case dataType_t::dataInt32:
        return dataTypeAndFTVL_t("asynInt32", "");
    case dataType_t::dataUint32:
        return dataTypeAndFTVL_t("asynUInt32Digital", "");
    case dataType_t::dataFloat64:
        return dataTypeAndFTVL_t("asynFloat64", "");
    case dataType_t::dataInt8Array:
        if(bInput)
        {
            return dataTypeAndFTVL_t("asynOctetRead", "CHAR");
        }
        else
        {
            return dataTypeAndFTVL_t("asynOctetWrite", "CHAR");
        }
    case dataType_t::dataUint8Array:
        if(bInput)
        {
            return dataTypeAndFTVL_t("asynOctetRead", "UCHAR");
        }
        else
        {
            return dataTypeAndFTVL_t("asynOctetWrite", "UCHAR");
        }
    case dataType_t::dataInt32Array:
        if(bInput)
        {
            return dataTypeAndFTVL_t("asynInt32ArrayIn", "LONG");
        }
        else
        {
            return dataTypeAndFTVL_t("asynInt32ArrayOut", "LONG");
        }
    case dataType_t::dataFloat64Array:
        if(bInput)
        {
            return dataTypeAndFTVL_t("asynFloat64ArrayIn", "DOUBLE");
        }
        else
        {
            return dataTypeAndFTVL_t("asynFloat64ArrayOut", "DOUBLE");
        }
    }
    throw std::logic_error("Unknown data type");
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
    case recordType_t::notSpecified:
        throw std::logic_error("The type notSpecified should not be auto-inserted in the generated db file");
    case recordType_t::aai:
        return typeAndInput_t("aai", true);
    case recordType_t::aao:
        return typeAndInput_t("aao", false);
    case recordType_t::ai:
        return typeAndInput_t("ai", true);
    case recordType_t::ao:
        return typeAndInput_t("ao", false);
    case recordType_t::bi:
        return typeAndInput_t("bi", true);
    case recordType_t::bo:
        return typeAndInput_t("bo", false);
    case recordType_t::longin:
        return typeAndInput_t("longin", true);
    case recordType_t::longout:
        return typeAndInput_t("longout", false);
    case recordType_t::mbbi:
        return typeAndInput_t("mbbi", true);
    case recordType_t::mbbo:
        return typeAndInput_t("mbbo", false);
    case recordType_t::mbbiDirect:
        return typeAndInput_t("mbbiDirect", true);
    case recordType_t::mbboDirect:
        return typeAndInput_t("mbboDirect", false);
    case recordType_t::stringIn:
        return typeAndInput_t("stringin", true);
    case recordType_t::stringOut:
        return typeAndInput_t("stringout", false);
    case recordType_t::waveformIn:
        return typeAndInput_t("waveform", true);
    case recordType_t::waveformOut:
        return typeAndInput_t("waveform", false);
    }
    throw std::logic_error("Unknown record type");
}


/*
 * Register a PV and generated the db file
 *
 *****************************************/
void EpicsInterfaceImpl::registerPV(std::shared_ptr<PVBaseImpl> pv)
{
    // Save the PV in a list. The order in the is used as "reason".
    ///////////////////////////////////////////////////////////////
    m_pvs.push_back(pv);
    m_pvNameToReason[pv->getFullNameFromPort()] = m_pvs.size() - 1;

    // If the record type is specified then attempt to auto generate a db file
    //////////////////////////////////////////////////////////////////////////
    recordType_t type = pv->getType();
    if(type != recordType_t::notSpecified)
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
        case scanType_t::passive:
            scanType << "Passive";
            break;
        case scanType_t::periodic:
            scanType << pv->getScanPeriodSeconds() << " second";
            break;
        case scanType_t::interrupt:
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

        // Add enumerations
        static const char* epicsEnumNames[] = {"ZR", "ON", "TW", "TH", "FR", "FV", "SX", "SV", "EI", "NI", "TE", "EL", "TV", "TT", "FT", "FF"};
        const enumerationStrings_t& enumerations = pv->getEnumerations();
        size_t enumNumber(0);
        for(enumerationStrings_t::const_iterator scanStrings(enumerations.begin()), endScan(enumerations.end()); scanStrings != endScan; ++scanStrings)
        {
            dbEntry << "    field(" << epicsEnumNames[enumNumber] << "VL, " << enumNumber << ")" << std::endl;
            dbEntry << "    field(" << epicsEnumNames[enumNumber] << "ST, \"" << *scanStrings << "\")" << std::endl;
            enumNumber++;
        }


        dbEntry << "}" << std::endl << std::endl;
        dbEntry.flush();

        m_autogeneratedDB += dbEntry.str();
    }
}


/*
 * Called after the registration of the PVs has been performed
 *
 *************************************************************/
void EpicsInterfaceImpl::registrationTerminated()
{
    char tmpBuffer[L_tmpnam];

    std::string tmpFileName(tmpnam_r(tmpBuffer));

    std::string fileName(tmpFileName);
    std::ofstream outputStream(fileName.c_str());
    outputStream << m_autogeneratedDB;
    outputStream.flush();

    std::string command("dbLoadDatabase ");
    command += tmpFileName;
    iocshCmd(command.c_str());
}

void EpicsInterfaceImpl::push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::int32_t& value)
{
    pushOneValue<epicsInt32, asynInt32Interrupt>(pv, timestamp, (epicsInt32)value, asynStdInterfaces.int32InterruptPvt);
}

void EpicsInterfaceImpl::push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const double& value)
{
    pushOneValue<epicsFloat64, asynFloat64Interrupt>(pv, timestamp, (epicsFloat64)value, asynStdInterfaces.float64InterruptPvt);
}

void EpicsInterfaceImpl::push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::vector<std::int32_t> & value)
{
    pushArray<epicsInt32, asynInt32ArrayInterrupt>(pv, timestamp, (epicsInt32*)value.data(), value.size(), asynStdInterfaces.int32ArrayInterruptPvt);
}


/*
 * Push a scalar value to EPICS
 *
 ******************************/
template<typename T, typename interruptType>
void EpicsInterfaceImpl::pushOneValue(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const T& value, void* interruptPvt)
{
    int reason = m_pvNameToReason[pv->getFullNameFromPort()];

    ELLLIST       *pclientList;
    int            addr;

    pasynManager->interruptStart(interruptPvt, &pclientList);

    interruptNode* pnode = (interruptNode *)ellFirst(pclientList);
    while (pnode)
      {
        interruptType *pInterrupt = (interruptType *)pnode->drvPvt;
        pInterrupt->pasynUser->timestamp = convertUnixTimeToEpicsTime(timestamp);
        pasynManager->getAddr(pInterrupt->pasynUser, &addr);
        if ((pInterrupt->pasynUser->reason == reason) && (0 == addr))
          {
            pInterrupt->pasynUser->auxStatus = asynSuccess;
            pInterrupt->callback(pInterrupt->userPvt, pInterrupt->pasynUser, value);
          }
        pnode = (interruptNode *)ellNext(&pnode->node);
      }
    pasynManager->interruptEnd(interruptPvt);
}


/*
 * Push an array to EPICS
 *
 ************************/
template<typename T, typename interruptType>
void EpicsInterfaceImpl::pushArray(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const T* pValue, size_t numElements, void* interruptPvt)
{
    int reason = m_pvNameToReason[pv->getFullNameFromPort()];

    ELLLIST       *pclientList;
    int            addr;

    pasynManager->interruptStart(interruptPvt, &pclientList);

    interruptNode* pnode = (interruptNode *)ellFirst(pclientList);
    while (pnode)
      {
        interruptType *pInterrupt = (interruptType *)pnode->drvPvt; // TODO: remember the last used pointer
        pInterrupt->pasynUser->timestamp = convertUnixTimeToEpicsTime(timestamp);
        pasynManager->getAddr(pInterrupt->pasynUser, &addr);
        if ((pInterrupt->pasynUser->reason == reason) && (0 == addr))
          {
            pInterrupt->pasynUser->auxStatus = asynSuccess;
            pInterrupt->callback(pInterrupt->userPvt, pInterrupt->pasynUser, (T*)pValue, numElements);
          }
        pnode = (interruptNode *)ellNext(&pnode->node);
      }
    pasynManager->interruptEnd(interruptPvt);
}

/*
 * Called to write one scalar value into a PV
 *
 ********************************************/
template<typename T>
asynStatus EpicsInterfaceImpl::writeOneValue(asynUser* pasynUser, const T& value)
{
    timespec timestamp = convertEpicsTimeToUnixTime(pasynUser->timestamp);

    try
    {
        m_pvs[pasynUser->reason]->write(timestamp, value);
        pasynUser->auxStatus = asynSuccess;
    }
    catch(std::runtime_error& e)
    {
        pasynUser->auxStatus = asynError;
        errorAndSize_t errorAndSize = getErrorString(e.what());
        pasynUser->errorMessage = (char*)errorAndSize.first;
        pasynUser->errorMessageSize = errorAndSize.second;
    }

    return (asynStatus)pasynUser->auxStatus;
}


/*
 * Called to read a scalar value from a PV
 *
 *****************************************/
template<typename T>
asynStatus EpicsInterfaceImpl::readOneValue(asynUser* pasynUser, T* pValue)
{
    try
    {
        timespec timestamp = convertEpicsTimeToUnixTime(pasynUser->timestamp);
        m_pvs[pasynUser->reason]->read(&timestamp, pValue);
        pasynUser->timestamp = convertUnixTimeToEpicsTime(timestamp);
    }
    catch(std::runtime_error& e)
    {
        pasynUser->auxStatus = asynError;
        errorAndSize_t errorAndSize = getErrorString(e.what());
        pasynUser->errorMessage = (char*)errorAndSize.first;
        pasynUser->errorMessageSize = errorAndSize.second;
    }

    return (asynStatus)pasynUser->auxStatus;

}


/*
 * Called to read an array from a PV
 *
 ***********************************/
template<typename T>
asynStatus EpicsInterfaceImpl::readArray(asynUser *pasynUser, T* pValue, size_t nElements, size_t *nIn)
{
    try
    {
        timespec timestamp = convertEpicsTimeToUnixTime(pasynUser->timestamp);

        if(pasynUser->timestamp.secPastEpoch == 0 && pasynUser->timestamp.nsec == 0)
        {
            clock_gettime(CLOCK_REALTIME, &timestamp);
        }

        std::vector<T> vector(nElements);
        m_pvs[pasynUser->reason]->read(&timestamp, &vector);

        if(vector.size() > nElements)
        {
            vector.resize(nElements);
        }
        *nIn = vector.size();

        ::memcpy(pValue, vector.data(), vector.size() * sizeof(T));

        pasynUser->timestamp = convertUnixTimeToEpicsTime(timestamp);
        pasynUser->auxStatus = asynSuccess;

    }
    catch(std::runtime_error& e)
    {
        pasynUser->auxStatus = asynError;
        errorAndSize_t errorAndSize = getErrorString(e.what());
        pasynUser->errorMessage = (char*)errorAndSize.first;
        pasynUser->errorMessageSize = errorAndSize.second;
    }
    return (asynStatus)pasynUser->auxStatus;
}


/*
 * Called to write an array into a PV
 *
 ************************************/
template<typename T>
asynStatus EpicsInterfaceImpl::writeArray(asynUser *pasynUser, T* pValue, size_t nElements)
{
    timespec timestamp = convertEpicsTimeToUnixTime(pasynUser->timestamp);

    std::vector<T> vector(nElements);
    ::memcpy(vector.data(), pValue, vector.size() * sizeof(T));

    try
    {
        m_pvs[pasynUser->reason]->write(timestamp, vector);
        pasynUser->auxStatus = asynSuccess;
    }
    catch(std::runtime_error& e)
    {
        pasynUser->auxStatus = asynError;
        errorAndSize_t errorAndSize = getErrorString(e.what());
        pasynUser->errorMessage = (char*)errorAndSize.first;
        pasynUser->errorMessageSize = errorAndSize.second;
    }

    return (asynStatus)pasynUser->auxStatus;
}





asynStatus EpicsInterfaceImpl::readInt32(asynUser *pasynUser, epicsInt32 *pValue)
{
    return readOneValue(pasynUser, (std::int32_t*)pValue);
}

asynStatus EpicsInterfaceImpl::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    return writeOneValue(pasynUser, (std::int32_t)value);
}


asynStatus EpicsInterfaceImpl::readFloat64(asynUser *pasynUser, epicsFloat64 *pValue)
{
    return readOneValue(pasynUser, (double*)pValue);
}

asynStatus EpicsInterfaceImpl::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
    return writeOneValue(pasynUser, (double)value);
}

asynStatus EpicsInterfaceImpl::readInt32Array(asynUser *pasynUser, epicsInt32* pValue,
                                              size_t nElements, size_t *nIn)
{
    return readArray(pasynUser, (std::int32_t*)pValue, nElements, nIn);
}

asynStatus EpicsInterfaceImpl::writeInt32Array(asynUser *pasynUser, epicsInt32* pValue,
                                               size_t nElements)
{
    return writeArray(pasynUser, pValue, nElements);
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


/*
 * Constants used for the EPICS<-->UNIX time conversion
 *
 ******************************************************/
static const uint64_t nanosecondCoeff(1000000000L);
static const uint64_t conversionToEpics(uint64_t(POSIX_TIME_AT_EPICS_EPOCH) * nanosecondCoeff);

/*
 * Conversion from EPICS epoch to UNIX epoch
 *
 *******************************************/
timespec EpicsInterfaceImpl::convertEpicsTimeToUnixTime(const epicsTimeStamp& time)
{
    if(time.secPastEpoch == 0 && time.nsec == 0)
    {
        timespec unixTime{0, 0};
        return unixTime;
    }

    std::uint64_t timeNs((std::uint64_t)time.secPastEpoch * nanosecondCoeff + (std::uint64_t)time.nsec + conversionToEpics);

    timespec unixTime;
    unixTime.tv_sec = (std::uint32_t)(timeNs / nanosecondCoeff);
    unixTime.tv_nsec = (std::uint32_t)(timeNs % nanosecondCoeff);

    return unixTime;
}


/*
 * Conversion from UNIX epoch to EPICS epoch
 *
 *******************************************/
epicsTimeStamp EpicsInterfaceImpl::convertUnixTimeToEpicsTime(const timespec& time)
{
    if(time.tv_sec == 0 && time.tv_nsec == 0)
    {
        epicsTimeStamp epicsTime{0, 0};
        return epicsTime;
    }

    std::uint64_t timeNs((std::uint64_t)time.tv_sec * nanosecondCoeff + (std::uint64_t)time.tv_nsec);
    if(timeNs < conversionToEpics)
    {
        throw TimeConversionError("The Unix epoch is smaller than the Epics epoch 0");
    }

    std::uint64_t epicsTimeNs(timeNs - conversionToEpics);

    epicsTimeStamp epicsTime;
    epicsTime.secPastEpoch = epicsTimeNs / nanosecondCoeff;
    epicsTime.nsec = epicsTimeNs % nanosecondCoeff;

    return epicsTime;
}


/*
 * Allocate a static buffer for an error message
 *
 ***********************************************/
EpicsInterfaceImpl::errorAndSize_t EpicsInterfaceImpl::getErrorString(const std::string& error)
{
    std::pair<std::set<std::string>::const_iterator, bool> elementInsertion = m_errorMessages.insert(error);
    return errorAndSize_t((*elementInsertion.first).c_str(), (*elementInsertion.first).size());
}

}

#endif
