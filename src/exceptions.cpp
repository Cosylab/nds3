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

FactoryError::FactoryError(const std::string &what): NdsError(what)
{
}

DirectoryNotFoundError::DirectoryNotFoundError(const std::string &what): FactoryError(what)
{
}

DriverNotFound::DriverNotFound(const std::string &what): FactoryError(what)
{
}

DriverAlreadyRegistered::DriverAlreadyRegistered(const std::string &what): FactoryError(what)
{
}

DriverDoesNotExportRegistrationFunctions::DriverDoesNotExportRegistrationFunctions(const std::string &what): FactoryError(what)
{
}

ControlSystemNotFound::ControlSystemNotFound(const std::string &what): FactoryError(what)
{
}

DeviceNotAllocated::DeviceNotAllocated(const std::string &what): FactoryError(what)
{
}

DeviceAlreadyCreated::DeviceAlreadyCreated(const std::string &what): FactoryError(what)
{
}

PVAlreadyDeclared::PVAlreadyDeclared(const std::string& what): FactoryError(what)
{
}

MissingInputPV::MissingInputPV(const std::string& what): FactoryError(what)
{
}

MissingOutputPV::MissingOutputPV(const std::string& what): FactoryError(what)
{
}

MissingDestinationPV::MissingDestinationPV(const std::string &what): FactoryError(what)
{
}

INIParserError::INIParserError(const std::string& what): NdsError(what)
{
}

INIParserMissingSection::INIParserMissingSection(const std::string& what): INIParserError(what)
{
}

INIParserSyntaxError::INIParserSyntaxError(const std::string &what): INIParserError(what)
{
}


}
