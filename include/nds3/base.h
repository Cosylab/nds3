#ifndef NDSBASE_H
#define NDSBASE_H

/**
 * @file base.h
 * @brief Defines the base class used for nodes, PVs, state machines and acquisition
 *        devices.
 *
 * Include nds3.h instead of this file, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */


#include "definitions.h"
#include <memory>
#include <string>

/**
 * @namespace nds
 * @brief All the classes and methods defined by the NDS3 framework are in the namespace
 *        nds.
 */
namespace nds
{

class BaseImpl;
class Port;
class Node;

/**
 * @brief This is the base class for all the NDS3 public classes that
 *        declare a device structure.
 *
 */
class NDS3_API Base
{
    friend class Node;

public:
#ifndef SWIG
    Base(std::shared_ptr<BaseImpl> impl);
#endif

protected:
    /**
     * @brief Initializes an empty base class.
     */
    Base();

public:
    /**
     * @brief Copy constructor. The copy will refer to the same
     *        implementation object referenced in the copied object.
     *
     * @param right the object to copy
     */
    Base(const Base& right);

    /**
     * @brief Copy operator. The copy will refer to the same implementation object
     *        referenced in the copied object.
     *
     * @param right the object to copy
     * @return
     */
    Base& operator=(const Base& right);

    virtual ~Base();

    /**
     * @brief Get the Node that communicate with the device (ASYN port on EPICS, Device on Tango).
     *        Query the parent nodes if necessary.
     *
     * @return a reference to the parent Device node, which can be used to communicate directly with
     *         the device
     */
    virtual Port getPort();

    /**
     * @brief Return the node's name.
     *
     * @return the node's name.
     */
    std::string getComponentName() const;

    /**
     * @brief Return the full node's name, prepending the parents' names if necessary
     *        (e.g. "ROOT-CHANNEL1-THISNODE")
     *
     * @return the fully qualified node's name
     */
    std::string getFullName() const;

    /**
     * @brief Return the node's name as seen by the parent Port node (the node that communicates
     *        with the device).
     *
     * For instance, for a record "TIME" contained in the channel "CH0" that is part of
     * an Port named "DEVICE" the return value will be "CH0-TIME": node that the "DEVICE"
     * part is missing.
     *
     * @return the node's name relative to the Port that contains it.
     */
    std::string getFullNameFromPort() const;

    /**
     * @brief Registers all the records with the control system. Call this from the root node
     *        which will take care of traversing its children and initialize them.
     *
     * @param pDeviceObject   pointer to the device object
     * @param controlSystem_t control system with which the node must be registered
     */
    void initialize(void* pDeviceObject, const controlSystem_t controlSystemType = controlSystem_t::defaultSystem);

    /**
     * @brief Get the current time.
     *
     * - if a custom time function has been declared with setTimestampDelegate() then the
     *   delegate function is called, or...
     * - if this is not the root node then the parent node's getTimestamp() is called, or...
     * - if this is the parent node then the local machine's UTC time is returned.
     *
     * @return the current time
     */
    timespec getTimestamp() const;

    /**
     * @brief Specify the delegate function to call to get the timestamp.
     *
     * If this method is not called then an internal function is called, which traverses
     * all the parent nodes until the node with no parent returns the local time or
     * call its own delegate function if specified.
     *
     * @param timestampDelegate the delegate function to call to get the timestamp
     */
    void setTimestampDelegate(getTimestampPlugin_t timestampDelegate);

#ifndef SWIG
protected:
    std::shared_ptr<BaseImpl> m_pImplementation;
#endif

};

}
#endif // NDSBASE_H
