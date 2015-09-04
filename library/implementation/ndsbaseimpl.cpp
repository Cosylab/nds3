#include "ndsbaseimpl.h"
#include "ndsnodeimpl.h"

namespace nds
{

BaseImpl::BaseImpl(const std::string& name): m_name(name), m_timestampFunction(std::bind(&BaseImpl::getLocalTimestamp, this))
{}


std::shared_ptr<PortImpl> BaseImpl::getPort()
{
    std::shared_ptr<NodeImpl> temporaryPointer = m_pParent.lock();
    if(temporaryPointer.get() == 0)
    {
        throw;
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
    clock_gettime(CLOCK_MONOTONIC, &timestamp);
    return timestamp;
}

}


