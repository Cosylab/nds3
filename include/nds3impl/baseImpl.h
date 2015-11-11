/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSBASEIMPL_H
#define NDSBASEIMPL_H

#include <string>
#include <map>
#include <memory>
#include <array>
#include <set>
#include <mutex>
#include <ostream>
#include "../nds3/definitions.h"


namespace nds
{


class NodeImpl;
class PortImpl;
class FactoryBaseImpl;
class LogStreamBufferImpl;
class LogStreamGetterImpl;
class ThreadBaseImpl;

/**
 * @internal
 * @brief This is the base class for the implementation of nodes, devices, PVs.
 *
 */
class NDS3_API BaseImpl: public std::enable_shared_from_this<BaseImpl>
{
protected:
    BaseImpl(const std::string& name);

public:
    BaseImpl(const BaseImpl& right) = delete;

    virtual ~BaseImpl();

    /**
     * @brief Set the string to pass to the naming rules in order to build
     *        the full node name as seen by the control system.
     *
     * If this method is not called then the external name is the same as the
     * node name.
     *
     * @param externalName the name of the node to pass to the naming rules
     */
    void setExternalName(const std::string& externalName);

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
    const std::string& getComponentName() const;

    void setParent(std::shared_ptr<NodeImpl> pParent, const std::uint32_t parentLevel);

    std::shared_ptr<NodeImpl> getParent() const;


    /**
     * @brief Return the full node's name, prepending the parents' names if necessary
     *        (e.g. "ROOT-CHANNEL1-THISNODE")
     *
     * @return the fully qualified node's name
     */
    const std::string& getFullName() const;

    /**
     * @brief Return the node's name as seen by the parent Port.
     *
     * For instance, for a record "TIME" contained in the channel "CH0" that is part of
     * a Port named "DEVICE" the return value will be "CH0-TIME": node that the "DEVICE"
     * part is missing.
     *
     * @return the node's name relative to the Port that contains it.
     */
    const std::string& getFullNameFromPort() const;

    /**
     * @brief Return the full node name as seen by the control system clients.
     *
     * @return the full node name as seen by the control system clients.
     */
    const std::string& getFullExternalName() const;

    const std::string& getFullExternalNameFromPort() const;



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

    ThreadBaseImpl* runInThread(const std::string& name, threadFunction_t function);

    /**
     * @ingroup logging
     * @brief Retrieve a stream that can be used for logging.
     *
     * Logging streams are specific for the requested log level and the current thread.
     *
     * @param logLevel the severity level of the messages that will be logged througn the stream
     * @return a logging stream for the requested log level in the current thread
     */
    std::ostream& getLogger(const logLevel_t logLevel);

    /**
     * @ingroup logging
     * @brief Returns true if the logging for a particular severity level has been enabled.
     *
     * @param logLevel the severity level for which the status is required
     * @return true if the logging for the specified severity level is enabled
     */
    bool isLogLevelEnabled(const logLevel_t logLevel) const;

    /**
     * @ingroup logging
     * @brief Enable the logging for a particular severity level.
     *
     * @param logLevel the severity level for which the logging is enabled.
     */
    virtual void setLogLevel(const logLevel_t logLevel);

    /**
     * @ingroup commands
     * @brief Define a command bound to the node.
     *
     * @param command
     * @param usage
     * @param numParameters
     */
    void defineCommand(const std::string& command, const std::string& usage, const size_t numParameters, const command_t function);

    /**
     * @brief Registers all the records with the control system. Do not call this
     *        function directly: call NodeImpl::initializeRootNode() instead.
     *
     * On EPICS it will create all the PVs and register them with the AsynDriver,
     * on Tango will create the dynamic attributes.
     */
    virtual void initialize(FactoryBaseImpl& controlSystem);

    /**
     * @brief Deregister all the records from the control system. Do not call this
     *        function directly: call NodeImpl::deinitializeRootNode() instead.
     */
    virtual void deinitialize();


    virtual std::string buildFullName(const FactoryBaseImpl& controlSystem) const ;
    virtual std::string buildFullNameFromPort(const FactoryBaseImpl& controlSystem) const ;

    virtual std::string buildFullExternalName(const FactoryBaseImpl& controlSystem) const = 0;
    virtual std::string buildFullExternalNameFromPort(const FactoryBaseImpl& controlSystem) const = 0;

protected:
    timespec getLocalTimestamp() const;

    /**
     * @brief User command that set the log level
     * @param logLevel   the log level to set
     * @param parameters ignored
     * @return           empty list of parameters
     */
    virtual parameters_t commandSetLogLevel(const logLevel_t logLevel, const parameters_t& parameters);

    /**
     * @brief The node name
     */
    std::string m_name;

    /**
     * @brief The node's external name (used for the naming rules).
     */
    std::string m_externalName;

    /**
     * @brief The level in the tree structure: 0 = root node.
     */
    std::uint32_t m_nodeLevel;

    /**
     * @brief Pointer to the parent node.
     */
    std::weak_ptr<NodeImpl> m_pParent;

    FactoryBaseImpl* m_pFactory;

    getTimestampPlugin_t m_timestampFunction;

    volatile logLevel_t m_logLevel;

    /**
     * @brief Point to a LogStreamGetterImpl that is used every time a new log stream
     *        is needed.
     */
    LogStreamGetterImpl* m_logStreamGetter;

    struct commandDefinition_t
    {
        commandDefinition_t(const std::string& command, const std::string& usage, const size_t numParameters, command_t function):
            m_command(command), m_usage(usage), m_numParameters(numParameters), m_function(function)
        {
        }

        std::string m_command;
        std::string m_usage;
        size_t m_numParameters;
        command_t m_function;
    };

    typedef std::list<commandDefinition_t> commands_t;
    commands_t m_commands;

protected:
    std::string m_cachedFullName;
    std::string m_cachedFullNameFromPort;
    std::string m_cachedFullExternalName;
    std::string m_cahcedFullExternalNameFromPort;
};


}










#endif // NDSBASEIMPL_H
