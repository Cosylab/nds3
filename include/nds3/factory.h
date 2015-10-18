#ifndef NDSFACTORY_H
#define NDSFACTORY_H

#include <string>
#include <memory>
#include <thread>
#include "definitions.h"

namespace nds
{

class FactoryBaseImpl;

/**
 * @brief Communicates with a static instance of the control system.
 *
 * It is used to register new devices and to launch the loop that
 *  communicates with the actual control system and runs until the
 *  application is terminated.
 *
 */
class NDS3_API Factory
{
    friend class Base;
public:
#ifndef SWIG
    Factory(std::shared_ptr<FactoryBaseImpl> impl);
#endif

    /**
     * @brief Allocate a factory object that communicates with the control system
     *
     * @param controlSystemName the type of control system to communicate with (empty = default)
     */
    Factory(const std::string& controlSystemName);

    /**
     * @brief Register a device in the control system.
     *
     * Your device lifecycle will be managed by NDS: an allocation function will be
     *  called when the device is needed and a deallocation function will be called
     *  when the device can be deleted.
     *
     * The allocation function should take care of creating the device structure using
     *  the Node, Port and PVBase derived classes and call initialize() on the root
     *  node. It should also allocate the classes that contain the delegate functions
     *  used by the nodes and PVs.
     *
     * The deallocation function should take care of removing the classes that contain
     *  the delegate functions.
     *
     * @param driverName         the device driver name
     * @param allocateFunction   the allocation function
     * @param deallocateFunction the deallocation function
     */
    static void registerDriver(const std::string& driverName, allocateDriver_t allocateFunction, deallocateDriver_t deallocateFunction);

    /**
     * @brief Register a new control system.
     *
     * Control system interfaces are loaded from dynamic modules during startup, but
     *  in some cases they may also be registered separately via this method.
     *
     * The test units use this method to register test control systems that have access
     *  to the NDS internals.
     *
     * @param factory the control system to be registered
     */
    static void registerControlSystem(Factory& factory);

    /**
     * @brief Launches a loop that communicate with the control system.
     *
     * The loop terminates only when the application must exit.
     *
     * @param argc  the argc parameter received by the main() function
     * @param argv  the argv parameter received by the main() function
     */
    void run(int argc,char *argv[]);

    /**
     * @brief Create a device registered with registerDriver or loaded
     *        during the startup sequence.
     *
     * @param driverName the name of the driver that implements the device
     * @param deviceName a parameter passed to the device usually used to
     *                    set the name of the root node
     * @param parameters map of named parameters passed to the device
     */
    void createDevice(const std::string& driverName, const std::string& deviceName, const namedParameters_t& parameters);

    /**
     * @brief Destroy a device created with createDevice().
     *
     * @param deviceName the name given to the device in createDevice()
     */
    void destroyDevice(const std::string& deviceName);

    void subscribe(const std::string& pushFrom, const std::string& pushTo);

    void unsubscribe(const std::string& pushTo);

    std::thread createThread(const std::string& name, threadFunction_t function);

#ifndef SWIG
private:
    std::shared_ptr<FactoryBaseImpl> m_pFactory;
#endif
};

}
#endif // NDSFACTORY_H
