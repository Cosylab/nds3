#ifndef NDSEXCEPTIONS_H
#define NDSEXCEPTIONS_H

#include "definitions.h"
#include <stdexcept>

namespace nds
{

class NDS3_API NdsError: public std::runtime_error
{
public:
    NdsError(const std::string& what);
};

class NDS3_API StateMachineError: public NdsError
{
public:
    StateMachineError(const std::string& what);
};


/**
 * @brief Throw this exception in a state transition function to cause the
 *        state machine to roll back to the previous state.
 */
class NDS3_API StateMachineRollBack: public StateMachineError
{
public:
    StateMachineRollBack(const std::string& what);
};


/**
 * @brief This exception is thrown by the state machine when a transition
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


}

#endif // NDSEXCEPTIONS_H
