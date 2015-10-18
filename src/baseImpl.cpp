#include "../include/nds3impl/baseImpl.h"
#include "../include/nds3impl/nodeImpl.h"
#include "../include/nds3impl/factoryBaseImpl.h"
#include "../include/nds3impl/logStreamGetterImpl.h"

#include <stdexcept>

namespace nds
{

BaseImpl::BaseImpl(const std::string& name): m_name(name), m_pFactory(0),
    m_timestampFunction(std::bind(&BaseImpl::getLocalTimestamp, this)),
    m_logLevel(logLevel_t::warning)
{
    for(size_t scanLevels(0); scanLevels != m_loggersKeys.size(); ++scanLevels)
    {
        pthread_key_create(&(m_loggersKeys[scanLevels]), &BaseImpl::deleteLogger);
    }

    // Register the commands for the log level
    //////////////////////////////////////////
    defineCommand("setLogLevelDebug", "", 0, std::bind(&BaseImpl::setLogLevel, this, logLevel_t::debug));
    defineCommand("setLogLevelInfo", "", 0, std::bind(&BaseImpl::setLogLevel, this, logLevel_t::info));
    defineCommand("setLogLevelWarning", "", 0, std::bind(&BaseImpl::setLogLevel, this, logLevel_t::warning));
    defineCommand("setLogLevelError", "", 0, std::bind(&BaseImpl::setLogLevel, this, logLevel_t::error));
}

BaseImpl::~BaseImpl()
{
    for(size_t scanLevels(0); scanLevels != m_loggersKeys.size(); ++scanLevels)
    {
        std::ostream* pStream = (std::ostream*)pthread_getspecific(m_loggersKeys[scanLevels]);
        if(pStream != 0)
        {
            pthread_setspecific(m_loggersKeys[scanLevels], 0);
            delete pStream;
        }
        pthread_key_delete(m_loggersKeys[scanLevels]);
    }

}

std::shared_ptr<PortImpl> BaseImpl::getPort()
{
    std::shared_ptr<NodeImpl> temporaryPointer = m_pParent.lock();
    if(temporaryPointer.get() == 0)
    {
        throw ;
    }
    return temporaryPointer->getPort();
}

std::string BaseImpl::getComponentName() const
{
    return m_name;
}

std::shared_ptr<NodeImpl> BaseImpl::getParent() const
{
    return m_pParent.lock();
}

std::string BaseImpl::getFullName() const
{
    std::shared_ptr<NodeImpl> temporaryPointer = m_pParent.lock();
    if(temporaryPointer == 0)
    {
        return getComponentName();
    }

    return temporaryPointer->getFullName() + "-" + getComponentName();
}

void BaseImpl::setParent(std::shared_ptr<NodeImpl> pParent)
{
    {
        std::shared_ptr<NodeImpl> temporaryPointer = m_pParent.lock();
        if(temporaryPointer)
        {
            throw std::logic_error("The object already has a parent");
        }
    }
    m_pParent = pParent;
}

std::string BaseImpl::getFullNameFromPort() const
{
    std::string parentName;

    std::shared_ptr<NodeImpl> temporaryPointer = m_pParent.lock();
    if(temporaryPointer != 0)
    {
        parentName = temporaryPointer->getFullNameFromPort();
    }
    if(parentName.empty())
    {
        return getComponentName();
    }
    return parentName + "-" + getComponentName();
}

void BaseImpl::initialize(FactoryBaseImpl &controlSystem)
{
    m_pFactory = &controlSystem;

    // Remember where we can go get our logging streams
    ///////////////////////////////////////////////////
    m_logStreamGetter = controlSystem.getLogStreamGetter();

    // Register all the commands
    ////////////////////////////
    for(commands_t::const_iterator scanCommands(m_commands.begin()), endCommands(m_commands.end()); scanCommands != endCommands; ++scanCommands)
    {
        controlSystem.registerCommand(getFullName(), scanCommands->m_command, scanCommands->m_usage, scanCommands->m_numParameters, scanCommands->m_function);
    }
}

void BaseImpl::deinitialize()
{
    // Deregister all the commands
    //////////////////////////////
    for(commands_t::const_iterator scanCommands(m_commands.begin()), endCommands(m_commands.end()); scanCommands != endCommands; ++scanCommands)
    {
        m_pFactory->deregisterCommand(getFullName());
    }

}

timespec BaseImpl::getTimestamp() const
{
    return m_timestampFunction();
}

void BaseImpl::setTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_timestampFunction = timestampDelegate;
}

timespec BaseImpl::getLocalTimestamp() const
{
    std::shared_ptr<NodeImpl> temporaryPointer = m_pParent.lock();
    if(temporaryPointer != 0)
    {
        return temporaryPointer->getTimestamp();
    }
    timespec timestamp;
    clock_gettime(CLOCK_REALTIME, &timestamp);
    return timestamp;
}

std::ostream& BaseImpl::getLogger(const logLevel_t logLevel)
{
    if(logLevel == logLevel_t::none)
    {
        throw std::logic_error("Should not ask for a log stream for severity \"none\"");
    }
    std::ostream* pStream = (std::ostream*)pthread_getspecific(m_loggersKeys[(size_t)logLevel]);
    if(pStream == 0)
    {
        pStream = m_logStreamGetter->getLogStream(*this, logLevel);
        pthread_setspecific(m_loggersKeys[(size_t)logLevel], pStream);
    }

    return *pStream;
}

void BaseImpl::deleteLogger(void* pLogger)
{
    delete (std::ostream*)pLogger;
}

bool BaseImpl::isLogLevelEnabled(const logLevel_t logLevel) const
{
    return (std::uint8_t)logLevel >= (std::uint8_t)m_logLevel;
}

void BaseImpl::setLogLevel(const logLevel_t logLevel)
{
    m_logLevel = logLevel;
}

void BaseImpl::defineCommand(const std::string& command, const std::string& usage, const size_t numParameters, const command_t function)
{
    m_commands.emplace_back(command, usage, numParameters, function);
}

}


