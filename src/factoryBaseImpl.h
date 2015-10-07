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
 *        and has to be allocated only once (singletons).
 *
 * In order to create support for a control system you need to overwrite this class and
 *  InterfaceBaseImpl.
 *
 * The FactoryBaseImpl derived object will be created when NDS starts and will stay alive until NDS
 *  is shutdown.
 *
 * The overwritten classes must implement the following methods:
 * - getNewInterface()
 * - run()
 * - getLogStreamGetter()
 * - registerCommand()
 * - deregisterCommand()
 *
 * Optionally, the method createThread() can also be overwritten.
 */

class FactoryBaseImpl
{
    friend class InterfaceBaseImpl;
public:
    virtual ~FactoryBaseImpl();

    /**
     * @brief Called to register the functions that allocate and deallocate a device.
     *
     * @param driverName         the name of the device driver
     * @param allocateFunction   the allocation function.
     * @param deallocateFunction the deallocation function
     */
    void registerDriver(const std::string& driverName, allocateDriver_t allocateFunction, deallocateDriver_t deallocateFunction);

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

    /**
     * @brief Called to register a command tied to a specific node.
     *
     * The NDS framework guarantees that there are no multi-threading concurrency
     *  issues when calling this function.
     *
     * @param node            the node to which the command has to be tied
     * @param command         the command
     * @param usage           command's description
     * @param numParameters   the number of accepted parameters
     * @param commandFunction the delegate function to call to execute the command
     */
    virtual void registerCommand(const BaseImpl& node,
                                 const std::string& command,
                                 const std::string& usage,
                                 const size_t numParameters, command_t commandFunction) = 0;

    /**
     * @brief Called to deregister a command tied to a node.
     *
     * The NDS framework guarantees that there are no multi-threading concurrency
     *  issues when calling this function.
     *
     * @param node          to node from which the command has to be removed
     * @param command       the command to remove
     */
    virtual void deregisterCommand(const BaseImpl& node,
                                   const std::string& command) = 0;

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
