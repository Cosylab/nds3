/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSFACTORY_H
#define NDSFACTORY_H

/**
 * @file factory.h
 *
 * @brief Define the class that interfaces with the NDS factory and
 *        the chosen control system.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */


#include <string>
#include <memory>
#include <thread>
#include "definitions.h"

namespace nds
{

class FactoryBaseImpl;
class Thread;

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
    friend class Node;
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
     * The devices are loaded automatically from the folders declared in the
     *  enviroment variables NDS_DEVICES and LD_LIBRARY_PATH. However you can use
     *  this method to declare additional devices that have been statically linked
     *  to your application (e.g.: the NDS test units do this).
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

    /**
     * @brief Subscribe an output PV (derived from PVBaseOut) to an input PV
     *        (derived from PVBaseIn).
     *
     * Each time the specified input PV pushes new values or is written by the
     *  device support that owns it then the output PV receives the pushed or
     * written value immediately and in the same thread used to push/write the
     *  input PV.
     *
     * This method works also across control systems running in the same NDS process.
     *
     * @param pushFrom the full name of the input PV from which the data must be pushed
     * @param pushTo   the full name of the output PV to which the data must be pushed
     */
    void subscribe(const std::string& pushFrom, const std::string& pushTo);

    /**
     * @brief Unsubscribe an output PV from the input PV.
     * @param pushTo the name of the receiving PV that must be unsubscribed
     */
    void unsubscribe(const std::string& pushTo);

    /**
     * @brief Replicate an input PV (derived from PVBaseIn) to another input PV.
     *
     * Each time data is pushed to the source input PV then the same data is also pushed
     *  to the destination PV immediately and in the same thread used by the original
     *  push/write operation.
     *
     * This method works also across control systems running in the same NDS process.
     *
     * @param replicateSource      the full name of the input PV from which the data is replicated
     * @param replicateDestination the full name of the input PV to which the data must be copied
     */
    void replicate(const std::string& replicateSource, const std::string& replicateDestination);

    /**
     * @brief Stop the replication to the specified PV.
     *
     * @param destination the full name of the input PV that should stop receiving copies of the
     *                    data pushed to the source PV
     */
    void stopReplicationTo(const std::string& destination);

    /**
     * @brief Creates a new thread and executes the specified function in it.
     *
     * @param name     the thread name
     * @param function the function to execute in the new thread
     * @return         a Thread object that references the new thread
     */
    Thread runInThread(const std::string& name, threadFunction_t function);

    void loadNamingRules(std::istream& rules);
    void setNamingRules(const std::string& rulesName);


#ifndef SWIG
private:
    std::shared_ptr<FactoryBaseImpl> m_pFactory;
#endif
};

}
#endif // NDSFACTORY_H
