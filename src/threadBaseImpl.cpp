/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/impl/threadBaseImpl.h"
#include "nds3/impl/factoryBaseImpl.h"

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
