#include "../include/nds3/ndsbase.h"
#include "../include/nds3/ndsport.h"
#include "ndsbaseimpl.h"

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

Port Base::getPort()
{
    return Port(m_pImplementation->getPort());
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

timespec Base::getTimestamp() const
{
    return m_pImplementation->getTimestamp();
}

void Base::setTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_pImplementation->setTimestampDelegate(timestampDelegate);
}


}
