/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include <stdexcept>

#include "nds3/impl/baseImpl.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/factoryBaseImpl.h"
#include "nds3/impl/logStreamGetterImpl.h"
#include "nds3/impl/threadBaseImpl.h"

namespace nds
{

BaseImpl::BaseImpl(const std::string& name): m_name(name), m_externalName(name), m_nodeLevel(0), m_pFactory(0),
    m_timestampFunction(std::bind(&BaseImpl::getLocalTimestamp, this)),
    m_logLevel(logLevel_t::warning), m_cachedFullName(name), m_cachedFullNameFromPort()
{
    // Register the commands for the log level
    //////////////////////////////////////////
    defineCommand("setLogLevelDebug", "", 0, std::bind(&BaseImpl::commandSetLogLevel, this, logLevel_t::debug, std::placeholders::_1));
    defineCommand("setLogLevelInfo", "", 0, std::bind(&BaseImpl::commandSetLogLevel, this, logLevel_t::info, std::placeholders::_1));
    defineCommand("setLogLevelWarning", "", 0, std::bind(&BaseImpl::commandSetLogLevel, this, logLevel_t::warning, std::placeholders::_1));
    defineCommand("setLogLevelError", "", 0, std::bind(&BaseImpl::commandSetLogLevel, this, logLevel_t::error, std::placeholders::_1));
}

BaseImpl::~BaseImpl()
{

}

/*
 * Set the external name
 *
 ***********************/
void BaseImpl::setExternalName(const std::string& externalName)
{
    m_externalName = externalName;
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

const std::string& BaseImpl::getComponentName() const
{
    return m_name;
}

std::shared_ptr<NodeImpl> BaseImpl::getParent() const
{
    return m_pParent.lock();
}

std::string BaseImpl::buildFullName(const FactoryBaseImpl& controlSystem) const
{
    std::shared_ptr<NodeImpl> temporaryPointer = m_pParent.lock();
    if(temporaryPointer == 0)
    {
        return getComponentName();
    }

    return temporaryPointer->buildFullName(controlSystem) + "-" + getComponentName();
}

void BaseImpl::setParent(std::shared_ptr<NodeImpl> pParent, const std::uint32_t parentLevel)
{
    {
        std::shared_ptr<NodeImpl> temporaryPointer = m_pParent.lock();
        if(temporaryPointer)
        {
            throw std::logic_error("The object already has a parent");
        }
    }
    m_nodeLevel = parentLevel + 1;
    m_pParent = pParent;

}

const std::string& BaseImpl::getFullName() const
{
    return m_cachedFullName;
}

const std::string& BaseImpl::getFullNameFromPort() const
{
    return m_cachedFullNameFromPort;
}

const std::string& BaseImpl::getFullExternalName() const
{
    return m_cachedFullExternalName;
}

const std::string& BaseImpl::getFullExternalNameFromPort() const
{
    return m_cahcedFullExternalNameFromPort;
}


std::string BaseImpl::buildFullNameFromPort(const FactoryBaseImpl& controlSystem) const
{
    std::string parentName;

    std::shared_ptr<NodeImpl> temporaryPointer = m_pParent.lock();
    if(temporaryPointer != 0)
    {
        parentName = temporaryPointer->buildFullNameFromPort(controlSystem);
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

    m_cachedFullName = buildFullName(controlSystem);
    m_cachedFullNameFromPort = buildFullNameFromPort(controlSystem);

    m_cachedFullExternalName = buildFullExternalName(controlSystem);
    m_cahcedFullExternalNameFromPort = buildFullExternalNameFromPort(controlSystem);

    // Remember where we can go get our logging streams
    ///////////////////////////////////////////////////
    m_logStreamGetter = controlSystem.getLogStreamGetter();

    // Register all the commands
    ////////////////////////////
    for(commands_t::const_iterator scanCommands(m_commands.begin()), endCommands(m_commands.end()); scanCommands != endCommands; ++scanCommands)
    {
        controlSystem.registerCommand(*this, scanCommands->m_command, scanCommands->m_usage, scanCommands->m_numParameters, scanCommands->m_function);
    }
}

void BaseImpl::deinitialize()
{
    // Deregister all the commands
    //////////////////////////////
    for(commands_t::const_iterator scanCommands(m_commands.begin()), endCommands(m_commands.end()); scanCommands != endCommands; ++scanCommands)
    {
        m_pFactory->deregisterCommand(*this);
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

ThreadBaseImpl* BaseImpl::runInThread(const std::string &name, threadFunction_t function)
{
    return m_pFactory->runInThread(name, function);
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
    return *(m_logStreamGetter->getLogStream(logLevel));
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

parameters_t BaseImpl::commandSetLogLevel(const logLevel_t logLevel, const parameters_t &)
{
    setLogLevel(logLevel);
    return parameters_t();
}

}


