#include "baseImpl.h"
#include "nodeImpl.h"
#include "factoryBaseImpl.h"
#include "logStreamGetterImpl.h"

#include <stdexcept>

namespace nds
{

BaseImpl::BaseImpl(const std::string& name): m_name(name),
    m_timestampFunction(std::bind(&BaseImpl::getLocalTimestamp, this)),
    m_logLevel(logLevel_t::warning)
{
    for(size_t scanLevels(0); scanLevels != m_loggersKeys.size(); ++scanLevels)
    {
        pthread_key_create(&(m_loggersKeys[scanLevels]), &BaseImpl::deleteLogger);
    }
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
    m_logStreamGetter = controlSystem.getLogStreamGetter();
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


}


