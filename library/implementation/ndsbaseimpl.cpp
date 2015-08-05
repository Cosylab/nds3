#include "ndsbaseimpl.h"
#include "../include/nds3/ndsbase.h"
#include "../include/nds3/ndsnode.h"

namespace nds
{


Port& BaseImpl::getPort()
{
    if(m_pParent == 0)
    {
        throw;
    }
    return m_pParent->getPort();
}

std::string BaseImpl::getComponentName() const
{
    return m_name;
}

std::string BaseImpl::getFullName() const
{
    if(m_pParent == 0)
    {
        return getComponentName();
    }

    return m_pParent->getFullName() + "-" + getComponentName();
}

void BaseImpl::setParent(Node *pParent)
{
    m_pParent = pParent;
}

std::string BaseImpl::getFullNameFromPort() const
{
    std::string parentName;
    if(m_pParent != 0)
    {
        parentName = m_pParent->getFullNameFromPort();
    }
    if(parentName.empty())
    {
        return getComponentName();
    }
    return parentName + "-" + getComponentName();
}

}


