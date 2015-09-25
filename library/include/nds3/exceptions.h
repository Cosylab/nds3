#ifndef NDSEXCEPTIONS_H
#define NDSEXCEPTIONS_H

/**
 * @file ndsexceptions.h
 *
 * @brief Defines all the exceptions that may be thrown by the library or
 *        recognized and processed by the library.
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

}

#endif // NDSEXCEPTIONS_H
