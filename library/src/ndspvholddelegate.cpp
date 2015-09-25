#include "../include/nds3/pvHoldDelegate.h"
#include "../include/nds3/delegate.h"

#include "pvHoldDelegateImpl.h"

namespace nds
{

PVHoldDelegate::PVHoldDelegate(const std::string& name, dataType_t dataType, std::shared_ptr<Delegate> pDelegate):
    PVBase(std::shared_ptr<PVBaseImpl>(new PVHoldDelegateImpl(name, dataType, pDelegate)))
{

}

}
