#include "ndspvbaseimpl.h"
#include "ndsportImpl.h"
#include "../include/nds3/ndsport.h"
#include "../include/nds3/ndspvbase.h"

namespace nds
{

PVBaseImpl::PVBaseImpl(const std::string& name): BaseImpl(name)
{

}

void PVBaseImpl::read(timespec* /* pTimestamp */, std::int32_t* /* pValue */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const std::int32_t& /* value */)
{
    throw;
}

void PVBaseImpl::read(timespec* /* pTimestamp */, double* /* pValue */)
{
    throw;
}

void PVBaseImpl::write(const timespec& /* pTimestamp */, const double& /* value */)
{
    throw;
}

void PVBaseImpl::setType(const std::string& type)
{
    m_type = type;
}

void PVBaseImpl::setDescription(const std::string& description)
{
    m_description = description;
}

void PVBaseImpl::setInterfaceName(const std::string& interfaceName)
{
    m_interfaceName = interfaceName;
}

std::string PVBaseImpl::getType()
{
    return m_type;
}

std::string PVBaseImpl::getDescription()
{
    return m_description;
}

std::string PVBaseImpl::getInterfaceName()
{
    return m_interfaceName;
}


void PVBaseImpl::initialize()
{
    getPort()->registerPV(std::static_pointer_cast<PVBaseImpl>(shared_from_this()));
}


}
