#ifndef NDSFACTORY_H
#define NDSFACTORY_H

#include <string>
#include <memory>
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
    friend class Port;
public:
    /**
     * @brief Allocate a factory object that communicates with the control system
     *
     * @param controlSystemType the type of control system to communicate with
     */
    Factory(const controlSystem_t controlSystemType);

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
    void registerDriver(const std::string& driverName, allocateDriver_t allocateFunction, deallocateDriver_t deallocateFunction);

    /**
     * @brief Launches a loop that communicate with the control system.
     *
     * The loop terminates only when the application must exit.
     */
    virtual void run();

#ifndef SWIG
private:
    FactoryBaseImpl* m_pFactory;
#endif
};

}
#endif // NDSFACTORY_H
