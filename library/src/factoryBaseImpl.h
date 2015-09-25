#ifndef NDSFACTORYBASEIMPL_H
#define NDSFACTORYBASEIMPL_H

#include "../include/nds3/definitions.h"
#include <map>
#include <mutex>
#include <memory>

namespace nds
{

class InterfaceBaseImpl;
class BaseImpl;

class FactoryBaseImpl
{
    friend class InterfaceBaseImpl;
public:
    virtual ~FactoryBaseImpl();

    virtual void registerDriver(const std::string& driverName, allocateDriver_t allocateFunction, deallocateDriver_t deallocateFunction);

    virtual InterfaceBaseImpl* getNewInterface(const std::string& fullName) = 0;

    virtual void run(int argc,char *argv[]) = 0;

    void* createDriver(const std::string& name, const std::string& parameter);

    void destroyDriver(void* pDevice);

    void holdNode(void* pDeviceObject, std::shared_ptr<BaseImpl> pHoldNode);



protected:

    typedef std::map<std::string, std::pair<allocateDriver_t, deallocateDriver_t> > driverAllocDeallocMap_t;
    driverAllocDeallocMap_t m_driversAllocDealloc;

    typedef std::multimap<std::string, void*> allocatedDevices_t;
    allocatedDevices_t m_allocatedDevices;

    typedef std::list<std::shared_ptr<BaseImpl> > nodesList_t;
    typedef std::map<void*, nodesList_t> heldNodes_t;
    heldNodes_t m_heldNodes;

    std::recursive_mutex m_mutex;

};

}


#endif // NDSFACTORYBASEIMPL_H
