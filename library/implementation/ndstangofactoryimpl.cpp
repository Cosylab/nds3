#ifdef NDS3_TANGO

#include "ndstangofactoryimpl.h"
#include "ndstangointerfaceimpl.h"

namespace nds
{

TangoFactoryImpl& TangoFactoryImpl::getInstance()
{
    static TangoFactoryImpl factory;
    return factory;
}

TangoFactoryImpl::TangoFactoryImpl(): m_pLastCreatedDevice(0)
{
}


InterfaceBaseImpl* TangoFactoryImpl::getNewInterface(const std::string& fullName)
{
    return new TangoInterfaceImpl(fullName, m_pLastCreatedDevice);
}

void TangoFactoryImpl::setLastCreatedDevice(NdsDevice* pDevice)
{
    m_pLastCreatedDevice = pDevice;
}

void TangoFactoryImpl::run(int argc,char *argv[])
{
    m_pTangoUtil = Tango::Util::init(argc, argv);

    Tango::DServer::register_class_factory(class_factory);
    try
    {
        m_pTangoUtil->server_init(false);
        m_pTangoUtil->server_run();
    }
    catch(const Tango::DevFailed& exception)
    {
        Tango::DevErrorList errors = exception.errors;
        for(int scanErrors(0); scanErrors != errors.length(); ++scanErrors)
        {
            Tango::DevError error = errors[scanErrors];
            std::cout << errors[scanErrors].reason << std::endl;
        }
    }
}

void TangoFactoryImpl::class_factory(Tango::DServer* pDServer)
{
    TangoFactoryImpl& factory(getInstance());
    factory.registerDriversWithTango(pDServer);
}

void TangoFactoryImpl::registerDriversWithTango(Tango::DServer* pDServer)
{
    // Add Tango classes
    for(driverAllocDeallocMap_t::const_iterator scanDrivers(m_driversAllocDealloc.begin()), endScan(m_driversAllocDealloc.end()); scanDrivers != endScan; ++scanDrivers)
    {
        std::string driverName = scanDrivers->first;
        pDServer->_add_class(new NdsDeviceClass(driverName, scanDrivers->second.first, scanDrivers->second.second));
    }


}

}

#endif
