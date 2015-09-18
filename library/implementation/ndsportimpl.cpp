#include "ndsportImpl.h"
#include "ndspvbaseimpl.h"
#include "ndsfactorybaseimpl.h"
#include "ndsinterfacebaseimpl.h"

namespace nds
{


PortImpl::PortImpl(const std::string& name, FactoryBaseImpl& controlSystem): NodeImpl(name), m_pFactory(&controlSystem)
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

void PortImpl::initialize()
{
    if(m_pInterface.get() == 0)
    {
        m_pInterface.reset(m_pFactory->getNewInterface(getFullName()));
    }
    NodeImpl::initialize();

    m_pInterface->registrationTerminated();
}

void PortImpl::registerPV(std::shared_ptr<PVBaseImpl> pv)
{
    m_pInterface->registerPV(pv);
}

template<typename T>
void PortImpl::push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const T& value)
{
    m_pInterface->push(pv, timestamp, value);
}

template void PortImpl::push<std::int32_t>(std::shared_ptr<PVBaseImpl>, const timespec&, const std::int32_t&);
template void PortImpl::push<std::vector<std::int32_t> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<std::int32_t>&);
template void PortImpl::push<double>(std::shared_ptr<PVBaseImpl>, const timespec&, const double&);
}
