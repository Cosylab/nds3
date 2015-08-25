#include "ndsinterfaceimpl.h"
#include "ndspvbaseimpl.h"
#include "../include/nds3/ndspvbase.h"

#include <cstdint>

namespace nds
{

InterfaceImpl::InterfaceImpl(const std::string& portName): asynPortDriver(
                                                               portName.c_str(),
                                                               256,    /* maxAddr */
                                                               0,
                                                               //asynCommonMask |
                                                               asynDrvUserMask |
                                                               //asynOptionMask |
                                                               asynInt32Mask // |
                                                               //asynUInt32DigitalMask |
                                                               //asynFloat64Mask |
                                                               //asynOctetMask |
                                                               //asynInt8ArrayMask |
                                                               //asynInt16ArrayMask |
                                                               //asynInt32ArrayMask |
                                                               //asynFloat32ArrayMask |
                                                               //asynFloat64ArrayMask |
                                                               //asynGenericPointerMask,   /* Interface mask */
                                              ,
                                                               asynInt32Mask // |
                                                               //asynUInt32DigitalMask |
                                                               //asynFloat64Mask |
                                                               //asynOctetMask |
                                                               //asynInt8ArrayMask |
                                                               //asynInt16ArrayMask |
                                                               //asynInt32ArrayMask |
                                                               //asynFloat32ArrayMask |
                                                               //asynFloat64ArrayMask |
                                                               //asynGenericPointerMask,            /* Interface mask */
                                                               , ASYN_CANBLOCK | ASYN_MULTIDEVICE,  /* asynFlags. */
                                                               1,                                 /* Autoconnect */
                                                               0,                                 /* Default priority */
                                                               0)
{
}


void InterfaceImpl::registerPV(std::shared_ptr<PVBaseImpl> pv)
{
    m_pvs.push_back(pv);
}

asynStatus InterfaceImpl::readInt32(asynUser *pasynUser, epicsInt32 *value)
{
    timespec timestamp;
    if(pasynUser->timestamp.secPastEpoch == 0 && pasynUser->timestamp.nsec == 0)
    {
        clock_gettime(CLOCK_MONOTONIC, &timestamp);
    }
    m_pvs[pasynUser->reason]->read(&timestamp, (std::int32_t*)value);

    return asynSuccess;
}

asynStatus InterfaceImpl::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    timespec timestamp;

    m_pvs[pasynUser->reason]->write(timestamp,(std::int32_t) value);
    return asynSuccess;
}

asynStatus InterfaceImpl::drvUserCreate(asynUser *pasynUser, const char *drvInfo,
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

timespec InterfaceImpl::convertEpicsTimeToUnixTime(const epicsTimeStamp& time)
{
    timespec unixTime;

    return unixTime;

}

epicsTimeStamp InterfaceImpl::convertUnixTimeToEpicsTime(const timespec& time)
{
    epicsTimeStamp epicsTime;
    return epicsTime;

}

}
