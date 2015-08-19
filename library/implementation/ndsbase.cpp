#include "../include/nds3/ndsbase.h"
#include "../include/nds3/ndsport.h"
#include "ndsbaseimpl.h"

namespace nds
{

Base::Base(std::shared_ptr<BaseImpl> impl): m_pImplementation(impl)
{
}

Base::Base()
{

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


}
