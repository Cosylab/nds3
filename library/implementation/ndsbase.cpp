#include "../include/nds3/ndsbase.h"
#include "ndsbaseimpl.h"

namespace nds
{

Base::Base()
{

}

Base::~Base()
{
}

Port& Base::getPort()
{
    return m_pImplementation->getPort();
}


std::string Base::getComponentName() const
{
    return m_pImplementation->getComponentName();
}


std::string Base::getFullName() const
{
    return m_pImplementation->getFullName();
}


std::string Base::getFullNameFromPort() const
{
    return m_pImplementation->getFullNameFromPort();
}

void Base::initialize()
{
    m_pImplementation->initialize();
}

void Base::setParent(Node *pParent)
{
    m_pImplementation->setParent(pParent);
}



}
