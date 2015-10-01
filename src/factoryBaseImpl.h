#ifndef NDSFACTORYBASEIMPL_H
#define NDSFACTORYBASEIMPL_H

#include "../include/nds3/definitions.h"
#include <map>
#include <mutex>
#include <memory>
#include <thread>

namespace nds
{

class InterfaceBaseImpl;
class BaseImpl;
class LogStreamGetterImpl;

/**
 * @brief This is the base class for objects that interact with specific control systems
 *        and need to be allocated only once (singletons).
 *
 * In order to create support for a control system you need to overwrite this class and
 *  InterfaceBaseImpl.
 *
 * The FactoryBaseImpl object will be created when NDS starts and will stay alive until NDS
 *  is shutdown.
 *
 * The overwritten classes should implement at least the following methods:
 * - getNewInterface()
 * - run()
 * - getLogStreamGetter()
 */

class FactoryBaseImpl
{
    friend class InterfaceBaseImpl;
public:
    virtual ~FactoryBaseImpl();

    virtual void registerDriver(const std::string& driverName, allocateDriver_t allocateFunction, deallocateDriver_t deallocateFunction);

    virtual std::thread createThread(const std::string& name, threadFunction_t function);

    void* createDevice(const std::string& name, const std::string& parameter);

    void destroyDriver(void* pDevice);

    void holdNode(void* pDeviceObject, std::shared_ptr<BaseImpl> pHoldNode);

    /**
     * @brief This method is called by a PortNodeImpl object in order to create a
     *         named connection with the control system.
     *
     * @param fullName the full name of the PortNodeImpl
     * @return a pointer to an InterfaceBaseImpl object. The lifetime of the InterfaceBaseImpl
     *         object is controlled by the PortNodeImpl object.
     */
    virtual InterfaceBaseImpl* getNewInterface(const std::string& fullName) = 0;

    virtual void run(int argc,char *argv[]) = 0;

    virtual LogStreamGetterImpl* getLogStreamGetter() = 0;

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
