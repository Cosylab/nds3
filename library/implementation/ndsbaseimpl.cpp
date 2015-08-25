#include "ndsbaseimpl.h"
#include "ndsnodeimpl.h"

namespace nds
{


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

}


