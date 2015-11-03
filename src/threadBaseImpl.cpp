#include "../include/nds3impl/threadBaseImpl.h"
#include "../include/nds3impl/factoryBaseImpl.h"

namespace nds
{

ThreadBaseImpl::ThreadBaseImpl(FactoryBaseImpl *pFactory, const std::string &name):
    m_pFactory(pFactory), m_name(name)
{
}

ThreadBaseImpl::~ThreadBaseImpl()
{

}

std::string ThreadBaseImpl::getName() const
{
    return m_name;
}

}
