#include "../include/nds3/ndsport.h"
#include "ndsportImpl.h"

namespace nds
{

Port::Port(const std::string &name)
{
    m_pImplementation.reset(new PortImpl(name, this));
}

void Port::registerPV(PVBase &pv)
{
    static_cast<PortImpl*>(m_pImplementation.get())->registerPV(pv);
}


}
