#ifndef NDSINTERFACEIMPL_H
#define NDSINTERFACEIMPL_H

#include <asynPortDriver.h>
#include <string>
#include <vector>
#include "ndspvbaseimpl.h"

namespace nds
{

class PVBase;

/**
 * @internal
 * @brief The AsynInterface class. Allocated by AsynPort
 *        to communicate with the AsynDriver
 */
class InterfaceImpl: public asynPortDriver
{
public:
    InterfaceImpl(const std::string& portName);

    void registerPV(std::shared_ptr<PVBaseImpl> pv);

    virtual asynStatus readInt32(asynUser *pasynUser, epicsInt32 *value);
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);

    virtual asynStatus drvUserCreate(asynUser *pasynUser, const char *drvInfo,
                                     const char **pptypeName, size_t *psize);

    timespec convertEpicsTimeToUnixTime(const epicsTimeStamp& time);
    epicsTimeStamp convertUnixTimeToEpicsTime(const timespec& time);

private:
    std::vector<std::shared_ptr<PVBaseImpl> > m_pvs;
};

}

#endif // NDSINTERFACEIMPL_H
