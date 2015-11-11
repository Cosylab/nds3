/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSEXCEPTIONS_H
#define NDSEXCEPTIONS_H

/**
 * @file exceptions.h
 *
 * @brief Defines all the exceptions that may be thrown by the library or
 *        recognized and processed by the library.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "definitions.h"
#include <stdexcept>

namespace nds
{

/**
 * @brief The base class for all the exceptions thrown by the library.
 */
class NDS3_API NdsError: public std::runtime_error
{
public:
    /**
     * @brief Constructor.
     *
     * @param what Human readable string describing the cause of the exception.
     */
    NdsError(const std::string& what);
};

/**
 * @brief Base class for all the exceptions thrown by the StateMachine.
 */
class NDS3_API StateMachineError: public NdsError
{
public:
    /**
     * @brief Constructor.
     *
     * @param what Human readable string describing the cause of the exception.
     */
    StateMachineError(const std::string& what);
};


/**
 * @brief The user implemented code should throw this exception from a state transition
 *        function to cause the state machine to roll back to the previous state.
 *
 * See the StateMachine description for more details about the state transitions.
 */
class NDS3_API StateMachineRollBack: public StateMachineError
{
public:
    /**
     * @brief Constructor.
     *
     * @param what Human readable string describing the cause of the exception.
     */
    StateMachineRollBack(const std::string& what);
};


/**
 * @brief This exception is thrown by the StateMachine when a transition
 *        between 2 states cannot be executed because the transition does
 *        not exist.
 */
class NDS3_API StateMachineNoSuchTransition: public StateMachineError
{
public:
    StateMachineNoSuchTransition(const std::string& what);
};


/**
 * @brief This transition is thrown by the state machine when a delegated
 *        function reject the transition.
 */
class NDS3_API StateMachineTransitionDenied: public StateMachineError
{
public:
    StateMachineTransitionDenied(const std::string& what);
};


/**
 * @brief This exception is thrown when there is an error in the conversion
 *        between the UNIX epoch and the EPICS epoch.
 *
 * Usually happens when the Unix EPOCH indicates a time before 1st of January 1990
 * (the starting point for the EPICS epoch).
 */
class NDS3_API TimeConversionError: public NdsError
{
public:
    TimeConversionError(const std::string& what);
};


/**
 * @brief This exception is thrown when there isn't any Port defined in the
 *        device structure. Without a Port there cannot be any communication
 *        with the control system.
 */
class NDS3_API NoPortDefinedError: public std::logic_error
{
public:
    NoPortDefinedError(const std::string& what);
};


/**
 * @brief This is the base class for exceptions thrown by the NDS Factory.
 *        Usually it is thrown while allocating new control system structures.
 */
class NDS3_API FactoryError: public NdsError
{
public:
    FactoryError(const std::string& what);
};


/**
 * @brief This exception is thrown when the folder being scanned for control
 *        system modules does not exist.
 */
class NDS3_API DirectoryNotFoundError: public FactoryError
{
public:
    DirectoryNotFoundError(const std::string& what);
};

/**
 * @brief This exception is thrown while creating a device when the requested
 *        device driver cannot be located.
 */
class NDS3_API DriverNotFound: public FactoryError
{
public:
    DriverNotFound(const std::string& what);
};

/**
 * @brief This exception is thrown when a driver with the same name has already
 *        been registered.
 */
class NDS3_API DriverAlreadyRegistered: public FactoryError
{
public:
    DriverAlreadyRegistered(const std::string& what);
};

/**
 * @brief This exception is thrown when a loaded shared module containing a
 *        NDS device does not contain the export functions (see @ref NDS_DEFINE_DRIVER)
 */
class NDS3_API DriverDoesNotExportRegistrationFunctions: public FactoryError
{
public:
    DriverDoesNotExportRegistrationFunctions(const std::string& what);
};

class NDS3_API ControlSystemNotFound: public FactoryError
{
public:
    ControlSystemNotFound(const std::string& what);
};

class NDS3_API DeviceNotAllocated: public FactoryError
{
public:
    DeviceNotAllocated(const std::string& what);
};

class NDS3_API DeviceAlreadyCreated: public FactoryError
{
public:
    DeviceAlreadyCreated(const std::string& what);
};

class PVAlreadyDeclared: public FactoryError
{
public:
    PVAlreadyDeclared(const std::string& what);
};

class MissingInputPV: public FactoryError
{
public:
    MissingInputPV(const std::string& what);
};

class MissingOutputPV: public FactoryError
{
public:
    MissingOutputPV(const std::string& what);
};

class MissingDestinationPV: public FactoryError
{
public:
    MissingDestinationPV(const std::string& what);
};

class INIParserError: public NdsError
{
public:
    INIParserError(const std::string& what);
};

class INIParserMissingSection: public INIParserError
{
public:
    INIParserMissingSection(const std::string& what);
};

class INIParserSyntaxError: public INIParserError
{
public:
    INIParserSyntaxError(const std::string& what);
};

}

#endif // NDSEXCEPTIONS_H
