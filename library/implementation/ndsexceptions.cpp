#include "../include/nds3/ndsexceptions.h"

namespace nds
{

NdsError::NdsError(const std::string &what): std::runtime_error(what)
{
}

StateMachineError::StateMachineError(const std::string& what): NdsError(what)
{
}

StateMachineRollBack::StateMachineRollBack(const std::string& what): StateMachineError(what)
{
}

StateMachineTransitionDenied::StateMachineTransitionDenied(const std::string& what): StateMachineError(what)
{
}

StateMachineNoSuchTransition::StateMachineNoSuchTransition(const std::string& what): StateMachineError(what)
{
}

}
