#ifdef NDS3_TANGO

#ifndef NDSTANGOFACTORYIMPL_H
#define NDSTANGOFACTORYIMPL_H

#include <tango.h>
#include "factoryBaseImpl.h"

namespace nds
{

class InterfaceBaseImpl;

class NdsDevice;

/**
 * @brief Takes care of registering everything with Tango
 *
 */
class TangoFactoryImpl: public FactoryBaseImpl
{
public:
    static TangoFactoryImpl& getInstance();

    TangoFactoryImpl();

    void setLastCreatedDevice(NdsDevice* pDevice);


    virtual InterfaceBaseImpl* getNewInterface(const std::string& fullName);

    virtual void run(int argc,char *argv[]);

private:
    static void class_factory(Tango::DServer* pDServer);

    void registerDriversWithTango(Tango::DServer* pDServer);

    Tango::Util* m_pTangoUtil;

    NdsDevice* m_pLastCreatedDevice;
};

}

#endif // NDSTANGOFACTORYIMPL_H

#endif // NDS3_TANGO
