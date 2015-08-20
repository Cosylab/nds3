#ifndef NDSBASE_H
#define NDSBASE_H

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

#ifndef SWIG
protected:
    std::shared_ptr<BaseImpl> m_pImplementation;
#endif

};

}
#endif // NDSBASE_H
