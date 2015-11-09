#include "../include/nds3/base.h"
#include "../include/nds3/port.h"
#include "../include/nds3/factory.h"
#include "../include/nds3impl/baseImpl.h"
#include "../include/nds3impl/threadBaseImpl.h"

namespace nds
{

Base::Base()
{
}

Base::Base(std::shared_ptr<BaseImpl> impl): m_pImplementation(impl)
{
}

Base::Base(const Base& right): m_pImplementation(right.m_pImplementation)
{
}

Base& Base::operator=(const Base& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

Base::~Base()
{
}

/*
 * Set the external name
 *
 ***********************/
void Base::setExternalName(const std::string& externalName)
{
    std::static_pointer_cast<BaseImpl>(m_pImplementation)->setExternalName(externalName);
}

Port Base::getPort()
{
    if(m_pImplementation == 0)
    {
        throw std::logic_error("The NDS object has not been allocated");
    }
    return Port(m_pImplementation->getPort());
}


const std::string& Base::getComponentName() const
{
    return m_pImplementation->getComponentName();
}


const std::string& Base::getFullName() const
{
    return m_pImplementation->getFullName();
}


const std::string& Base::getFullNameFromPort() const
{
    return m_pImplementation->getFullNameFromPort();
}

timespec Base::getTimestamp() const
{
    return m_pImplementation->getTimestamp();
}

void Base::setTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_pImplementation->setTimestampDelegate(timestampDelegate);
}

std::ostream& Base::getLogger(const logLevel_t logLevel)
{
    return m_pImplementation->getLogger(logLevel);
}

bool Base::isLogLevelEnabled(const logLevel_t logLevel) const
{
    return m_pImplementation->isLogLevelEnabled(logLevel);
}

void Base::setLogLevel(const logLevel_t logLevel)
{
    m_pImplementation->setLogLevel(logLevel);
}

void Base::defineCommand(const std::string& command, const std::string& usage, const size_t numParameters, const command_t function)
{
    m_pImplementation->defineCommand(command, usage, numParameters, function);
}

Thread Base::runInThread(const std::string &name, threadFunction_t function)
{
    return Thread(std::shared_ptr<ThreadBaseImpl>(m_pImplementation->runInThread(name, function)));
}

Thread Base::runInThread(threadFunction_t function)
{
    return Thread(std::shared_ptr<ThreadBaseImpl>(m_pImplementation->runInThread(getFullName(), function)));
}


}
