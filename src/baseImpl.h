#ifndef NDSBASEIMPL_H
#define NDSBASEIMPL_H

#include <string>
#include <map>
#include <memory>
#include <array>
#include <set>
#include <mutex>
#include <ostream>
#include "../include/nds3/definitions.h"


namespace nds
{


class NodeImpl;
class PortImpl;
class FactoryBaseImpl;
class LogStreamBufferImpl;
class LogStreamGetterImpl;

/**
 * @internal
 * @brief This is the base class for the implementation of nodes, devices, PVs.
 *
 */
class BaseImpl: public std::enable_shared_from_this<BaseImpl>
{
protected:
    BaseImpl(const std::string& name);

public:
    BaseImpl(const BaseImpl& right) = delete;

    virtual ~BaseImpl();

    /**
     * @brief Get the Node that holds an ASYN port. Query the parent nodes if necessary.
     *
     * @return a reference to the parent AsynNode, which can be used to communicate directly with
     *         the AsynDriver
     */
    virtual std::shared_ptr<PortImpl> getPort();

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

    void setParent(std::shared_ptr<NodeImpl> pParent);

    std::shared_ptr<NodeImpl> getParent() const;


    /**
     * @brief Return the node's name as seen by the parent AsynPort.
     *
     * For instance, for a record "TIME" contained in the channel "CH0" that is part of
     * an AsynPort named "DEVICE" the return value will be "CH0-TIME": node that the "DEVICE"
     * part is missing.
     *
     * @return the node's name relative to the AsynPort that contains it.
     */
    virtual std::string getFullNameFromPort() const;

    /**
     * @brief Registers all the records with the control system. Call this from the root node
     *        which will take care of traversing its children and initialize them.
     *
     * On EPICS it will create all the PVs and register them with the AsynDriver,
     * on Tango will create the dynamic attributes.
     */
    virtual void initialize(FactoryBaseImpl& controlSystem);

    /**
     * @brief Deregister all the records from the control system. Call this from the root node
     *        which will take care of traversing its children and deinitialize them.
     */
    virtual void deinitialize() = 0;

    /**
     * @brief Return the current time.
     *
     * If a delegate function has been defined with setTimestampDelegate() then call
     *  the delegated function, otherwise call the parent node's getTimestamp().
     * If the node has no parent then return the current time as reported by the
     *  operating system.
     *
     * @return the current time
     */
    timespec getTimestamp() const;

    void setTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    std::ostream& getLogger(const logLevel_t logLevel);

    /**
     * @brief Returns true if the logging for a particular severity level has been enabled.
     *
     * @param logLevel the severity level for which the status is required
     * @return true if the logging for the specified severity level is enabled
     */
    bool isLogLevelEnabled(const logLevel_t logLevel) const;

    /**
     * @brief Enable the logging for a particular severity level.
     *
     * @param logLevel the severity level for which the logging is enabled.
     */
    virtual void setLogLevel(const logLevel_t logLevel);


protected:
    timespec getLocalTimestamp() const;

    /**
     * @brief This method is registered with pthread_create_key(&m_removeLoggersKey, this)
     *        to remove the loggers that are specific to the running thread
     */
    static void deleteLogger(void* logger);

    std::string m_name;
    std::weak_ptr<NodeImpl> m_pParent;

    getTimestampPlugin_t m_timestampFunction;

    volatile logLevel_t m_logLevel;

    /**
     * @brief Point to a LogStreamGetterImpl that is used every time a new log stream
     *        is needed.
     */
    LogStreamGetterImpl* m_logStreamGetter;

    /**
     * @brief Used to gain access to the node's loggers (they are different for each thread)
     */
    std::array<pthread_key_t, (size_t)logLevel_t::none> m_loggersKeys;
};


}










#endif // NDSBASEIMPL_H
