#include "../include/nds3/ndsfactory.h"
#include "ndsfactoryimpl.h"

namespace nds
{

Factory::Factory(): m_pFactory(&(FactoryImpl::getInstance()))
{

}

void Factory::registrationCommand(const std::string &registrationCommandName)
{
    m_pFactory->registrationCommand(registrationCommandName);

}

void Factory::run()
{
    m_pFactory->run();
}

}
