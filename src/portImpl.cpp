#include "portImpl.h"
#include "pvBaseImpl.h"
#include "factoryBaseImpl.h"
#include "interfaceBaseImpl.h"

namespace nds
{


PortImpl::PortImpl(const std::string& name): NodeImpl(name)
{
}

PortImpl::~PortImpl()
{
}



std::shared_ptr<PortImpl> PortImpl::getPort()
{
    return std::static_pointer_cast<PortImpl>(shared_from_this());
}

std::string PortImpl::getFullNameFromPort() const
{
    return "";
}

void PortImpl::initialize(FactoryBaseImpl& controlSystem)
{
    if(m_pInterface.get() == 0)
    {
        m_pInterface.reset(controlSystem.getNewInterface(getFullName()));
    }
    NodeImpl::initialize(controlSystem);

    m_pInterface->registrationTerminated();
}

void PortImpl::deinitialize()
{
    if(m_pInterface.get() == 0)
    {
        throw std::logic_error("deinitialize called on non initialized port");
    }
    NodeImpl::deinitialize();
}

void PortImpl::registerPV(std::shared_ptr<PVBaseImpl> pv)
{
    m_pInterface->registerPV(pv);
}

void PortImpl::deregisterPV(std::shared_ptr<PVBaseImpl> pv)
{
    m_pInterface->deregisterPV(pv);
}

template<typename T>
void PortImpl::push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const T& value)
{
    m_pInterface->push(pv, timestamp, value);
}

template void PortImpl::push<std::int8_t>(std::shared_ptr<PVBaseImpl>, const timespec&, const std::int8_t&);
template void PortImpl::push<std::uint8_t>(std::shared_ptr<PVBaseImpl>, const timespec&, const std::uint8_t&);
template void PortImpl::push<std::int32_t>(std::shared_ptr<PVBaseImpl>, const timespec&, const std::int32_t&);
template void PortImpl::push<std::uint32_t>(std::shared_ptr<PVBaseImpl>, const timespec&, const std::uint32_t&);
template void PortImpl::push<double>(std::shared_ptr<PVBaseImpl>, const timespec&, const double&);
template void PortImpl::push<std::vector<std::int8_t> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<std::int8_t>&);
template void PortImpl::push<std::vector<std::uint8_t> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<std::uint8_t>&);
template void PortImpl::push<std::vector<std::int32_t> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<std::int32_t>&);
template void PortImpl::push<std::vector<double> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<double>&);
}
