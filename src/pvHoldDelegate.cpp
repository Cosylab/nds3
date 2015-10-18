#include "../include/nds3/pvHoldDelegate.h"
#include "../include/nds3/delegate.h"

#include "../include/nds3impl/pvHoldDelegateImpl.h"

namespace nds
{

PVHoldDelegate::PVHoldDelegate(const std::string& name, dataType_t dataType, std::shared_ptr<Delegate> pDelegate, dataDirection_t dataDirection):
    PVBase(std::shared_ptr<PVBaseImpl>(new PVHoldDelegateImpl(name, dataType, pDelegate, dataDirection)))
{

}

}
