#include "../include/ndsTestFactory.h"
#include "../include/ndsTestInterface.h"
#include <sstream>

namespace nds
{

namespace tests
{

static TestControlSystemFactoryImpl* m_factoryInstance(0);
TestControlSystemFactoryImpl::TestControlSystemFactoryImpl()
{
    m_factoryInstance = this;

}

TestControlSystemFactoryImpl* TestControlSystemFactoryImpl::getInstance()
{
    return m_factoryInstance;
}

const std::string TestControlSystemFactoryImpl::getName() const
{
    return "test";
}

InterfaceBaseImpl* TestControlSystemFactoryImpl::getNewInterface(const std::string& fullName)
{
    return new TestControlSystemInterfaceImpl(fullName);
}

void TestControlSystemFactoryImpl::run(int /* argc */,char * /* argv */[])
{
    // run until terminated
    ///////////////////////

}

LogStreamGetterImpl* TestControlSystemFactoryImpl::getLogStreamGetter()
{
    return this;
}

void TestControlSystemFactoryImpl::registerCommand(const BaseImpl& /* node */,
                                 const std::string& /* command */,
                                 const std::string& /* usage */,
                                 const size_t /* numParameters */,
                                 command_t /* commandFunction */)
{
}

void TestControlSystemFactoryImpl::deregisterCommand(const BaseImpl& /* node */)
{
}

const std::string& TestControlSystemFactoryImpl::getDefaultSeparator(const uint32_t nodeLevel) const
{
    static const std::string separator0("/");
    static const std::string separator1("-");
    static const std::string separator2(".");

    if(nodeLevel == 0)
    {
        return separator0;
    }
    else if(nodeLevel == 1)
    {
        return separator1;
    }
    return separator2;
}

void TestControlSystemFactoryImpl::log(const std::string& logString, const logLevel_t /* logLevel */)
{
    std::lock_guard<std::mutex> lock(m_logMutex);
    m_logs.insert(logString);
}

size_t TestControlSystemFactoryImpl::countStringInLog(const std::string &string)
{
    return m_logs.count(string);
}

std::ostream* TestControlSystemFactoryImpl::createLogStream(const logLevel_t logLevel)
{
    return new TestLogStream(logLevel, this);
}


TestLogStreamBufferImpl::TestLogStreamBufferImpl(const logLevel_t logLevel, TestControlSystemFactoryImpl* pFactory):
    m_logLevel(logLevel), m_pFactory(pFactory)
{
}

int TestLogStreamBufferImpl::sync()
{
    std::string string(std::string(pbase(), pptr() - pbase()));
    m_pFactory->log(string, m_logLevel);
    seekpos(0);
    return 0;
}

TestLogStream::TestLogStream(const logLevel_t logLevel, TestControlSystemFactoryImpl* pFactory):
    std::ostream(&m_buffer), m_buffer(logLevel, pFactory)
{

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
