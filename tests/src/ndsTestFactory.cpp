#include "../include/ndsTestFactory.h"
#include "../include/ndsTestInterface.h"
#include <sstream>

namespace nds
{

namespace tests
{

const std::string TestControlSystemFactoryImpl::getName() const
{
    return "test";
}

InterfaceBaseImpl* TestControlSystemFactoryImpl::getNewInterface(const std::string& fullName)
{
    return new TestControlSystemInterfaceImpl(fullName);
}

void TestControlSystemFactoryImpl::run(int argc,char *argv[])
{
    // run until terminated
    ///////////////////////

}

LogStreamGetterImpl* TestControlSystemFactoryImpl::getLogStreamGetter()
{
    return this;
}

void TestControlSystemFactoryImpl::registerCommand(const BaseImpl& node,
                                 const std::string& command,
                                 const std::string& usage,
                                 const size_t numParameters, command_t commandFunction)
{
}

void TestControlSystemFactoryImpl::deregisterCommand(const BaseImpl& node)
{
}


std::ostream* TestControlSystemFactoryImpl::getLogStream(const BaseImpl& node, const logLevel_t logLevel)
{
    return new std::ostringstream();

}

extern "C"
{
NDS3_API FactoryBaseImpl* allocateControlSystem()
{
    return new TestControlSystemFactoryImpl();
}

}

}

}
