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

void PVBaseImpl::initialize()
{
    getPort()->registerPV(std::static_pointer_cast<PVBaseImpl>(shared_from_this()));
}

}
