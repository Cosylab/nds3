#include "../include/nds3/exceptions.h"

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

TimeConversionError::TimeConversionError(const std::string &what): NdsError(what)
{
}

NoPortDefinedError::NoPortDefinedError(const std::string &what): std::logic_error(what)
{
}

FactoryError::FactoryError(const std::string &what): std::runtime_error(what)
{
}

DirectoryNotFoundError::DirectoryNotFoundError(const std::string &what): FactoryError(what)
{
}

DriverNotFound::DriverNotFound(const std::string &what): FactoryError(what)
{
}

}
