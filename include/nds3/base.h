/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSBASE_H
#define NDSBASE_H

/**
 * @file base.h
 * @brief Defines the base class used for nodes, PVs, state machines and acquisition
 *        devices.
 *
 * Include nds.h instead of this file, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "definitions.h"
#include "thread.h"
#include <memory>
#include <string>
#include <ostream>

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
 *        declare a device part (node, port, PV, etc).
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
     * @ingroup naming
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
     * @brief Get the Node that communicate with the device (ASYN port on EPICS, Device on Tango).
     *        Query the parent nodes if necessary.
     *
     * @return a reference to the parent Device node, which can be used to communicate directly with
     *         the device
     */
    virtual Port getPort();

    /**
     * @ingroup naming
     * @brief Return the node's name.
     *
     * @return the node's name.
     */
    const std::string& getComponentName() const;

    /**
     * @ingroup naming
     * @brief Return the full node's name, prepending the parents' names if necessary
     *        (e.g. "ROOT-CHANNEL1-THISNODE")
     *
     * @return the fully qualified node's name
     */
    const std::string& getFullName() const;

    /**
     * @ingroup naming
     * @brief Return the full node's name as seen by the control system clients.
     *
     * The external name is calculated from the node's full name (see getFullName)
     *  by applying the rules specified in the naming rules.
     *
     * @return the full node name as seen by the control system clients.
     */
    const std::string& getFullExternalName() const;

    /**
     * @ingroup naming
     * @brief Return the node's name as seen by the parent Port node (the node that communicates
     *        with the device).
     *
     * For instance, for a record "TIME" contained in the channel "CH0" that is part of
     * an Port named "DEVICE" the return value will be "CH0-TIME": node that the "DEVICE"
     * part is missing.
     *
     * @return the node's name relative to the Port that contains it.
     */
    const std::string& getFullNameFromPort() const;

    /**
     * @ingroup timestamp
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
     * @ingroup timestamp
     * @brief Specify the delegate function to call to get the timestamp.
     *
     * If this method is not called then an internal function is called, which traverses
     * all the parent nodes until the node with no parent returns the local time or
     * call its own delegate function if specified.
     *
     * @param timestampDelegate the delegate function to call to get the timestamp
     */
    void setTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @ingroup logging
     * @brief Retrieve a logging stream for the specified log level.
     *
     * You should use the NDS macros ndsDebugStream, ndsInfoStream, ndsWarningStream
     *  and ndsErrorStream in order to perform log operations: this increases the
     *  performances by avoiding non-necessary operations if the log has been disabled.
     *
     * The log information must be terminated by std::endl.
     *
     * Log streams are specific for each running thread in order to avoid multithreading
     *  related issues.
     *
     * Example of usage:
     * @code
     * node.getLogger(nds::logLevel_t::warning) << "This is a warning message" << std::endl;
     * @endcode
     *
     * or the preferred way:
     * @code
     * ndsWarningStream(node) << "This is the warning n." << 10 << std::endl;
     * @endcode
     *
     * @param logLevel the level of the messages that will be logged to the stream
     * @return a stream to which your application can send log messages.
     *         Log messages must be terminated by std::endl
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
     * The logging severity level will be set also on the node's children.
     *
     * @param logLevel the severity level for which the logging is enabled.
     */
    void setLogLevel(const logLevel_t logLevel);

    /**
     * @brief Define a command specific for the node.
     *
     * In EPICS the command will be available by typing:
     * @code
     * nds commandName nodeName [parameters]
     * @endcode
     *
     * For instance, for a command "switchOn" on a node named "Oscilloscope-ch0"
     *  you will have to type:
     * @code
     * nds switchOn Oscilloscope-ch0
     * @endcode
     *
     * @param command       the command name
     * @param usage         the command description
     * @param numParameters the number of parameters (only strings)
     * @param function      the delegate function to call to execute the command
     */
    void defineCommand(const std::string& command, const std::string& usage, const size_t numParameters, const command_t function);

    /**
     * @brief Create and run a thread using the control system facilities.
     *
     * If the control system does not provide any thread facility then a std::thread
     *  is created and executed.
     *
     * @param name     the name given to the thread
     * @param function the function to execute in the thread
     * @return         a Thread object referencing the new thread
     */
    Thread runInThread(const std::string& name, threadFunction_t function);

    /**
     * @brief Create and run a thread using the control system facilities. The created
     *        thread will have the node's name.
     *
     * If the control system does not provide any thread facility then a std::thread
     *  is created and executed.
     *
     * @param function the function to execute in the thread
     * @return         a Thread object referencing the new thread
     */
    Thread runInThread(threadFunction_t function);

#ifndef SWIG
protected:
    std::shared_ptr<BaseImpl> m_pImplementation;
#endif

};

}
#endif // NDSBASE_H
