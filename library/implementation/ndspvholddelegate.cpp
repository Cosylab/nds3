#include "../include/nds3/ndspvholddelegate.h"
#include "../include/nds3/ndsdelegate.h"

#include "ndspvholddelegateimpl.h"

namespace nds
{

PVHoldDelegate::PVHoldDelegate(const std::string& name, Delegate* pDelegate):
    PVBase(std::shared_ptr<PVBaseImpl>(new PVHoldDelegateImpl(name, pDelegate)))
{

}

}
