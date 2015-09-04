#ifndef NDSBASE_H
#define NDSBASE_H

/**
 * @file ndsbase.h
 * @brief Defines the base class used for nodes, PVs and state machines.
 *
 * Include nds3.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */


#include "definitions.h"
#include <memory>
#include <string>

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
protected:
    Base();

public:
#ifndef SWIG
    Base(std::shared_ptr<BaseImpl> impl);
#endif

    /**
     * @brief Copy constructor for the base class. The copy will refer to the same
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
     * @brief Get the Node that holds an ASYN port. Query the parent nodes if necessary.
     *
     * @return a reference to the parent AsynNode, which can be used to communicate directly with
     *         the AsynDriver
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
     * @brief Return the node's name as seen by the parent AsynPort.
     *
     * For instance, for a record "TIME" contained in the channel "CH0" that is part of
     * an AsynPort named "DEVICE" the return value will be "CH0-TIME": node that the "DEVICE"
     * part is missing.
     *
     * @return the node's name relative to the AsynPort that contains it.
     */
    std::string getFullNameFromPort() const;

    /**
     * @brief Registers all the records with the AsynDriver. Call this from the root node
     *        which will take care of traversing its children and initialize them.
     */
    void initialize();

    /**
     * @brief Get the current time.
     *
     * - if a custom time function has been declared with setTimestampDelegate() then the
     *   delegate function is called, or...
     * - if this is not the root node, the parent node's getTimestamp() is called, or...
     * - if this is the parent node then the local machine's UTC time is returned.
     *
     * @return the current time
     */
    timespec getTimestamp() const;

    void setTimestampDelegate(getTimestampPlugin_t timestampDelegate);

#ifndef SWIG
protected:
    std::shared_ptr<BaseImpl> m_pImplementation;
#endif

};

}
#endif // NDSBASE_H
