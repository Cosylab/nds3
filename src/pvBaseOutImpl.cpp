#include "../include/nds3impl/pvBaseOutImpl.h"
#include "../include/nds3impl/ndsFactoryImpl.h"

namespace nds
{

PVBaseOutImpl::PVBaseOutImpl(const std::string& name): PVBaseImpl(name)
{

}

void PVBaseOutImpl::initialize(FactoryBaseImpl &controlSystem)
{
    PVBaseImpl::initialize(controlSystem);
    NdsFactoryImpl::getInstance().registerOutputPV(this);
}

void PVBaseOutImpl::deinitialize()
{
    NdsFactoryImpl::getInstance().deregisterOutputPV(this);
    PVBaseImpl::deinitialize();
}

void PVBaseOutImpl::subscribeTo(const std::string &inputPVName)
{
    NdsFactoryImpl::getInstance().subscribe(inputPVName, this);
}


void PVBaseOutImpl::read(timespec* /* pTimestamp */, std::int32_t* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, double* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* pTimestamp, std::vector<std::int8_t>* pValue) const
{
    // TODO
    // Epics calls this also for unsigned-int.
    // If we arrive here maybe we really wanted to call the unsigned int function.
    // This is as ugly as it can get: consider modifying this
    read(pTimestamp, (std::vector<std::uint8_t>*) pValue);
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, std::vector<std::uint8_t>* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, std::vector<std::int32_t>* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, std::vector<double>* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::int32_t& /* value */)
{
    throw;
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const double& /* value */)
{
    throw;
}

void PVBaseOutImpl::write(const timespec& pTimestamp, const std::vector<std::int8_t>& value)
{
    // TODO
    // Epics calls this also for unsigned-int.
    // If we arrive here maybe we really wanted to call the unsigned int function.
    // This is as ugly as it can get: consider modifying this
    write(pTimestamp, (const std::vector<std::uint8_t>&) value);
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::vector<std::uint8_t>& /* value */)
{
    throw;
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::vector<std::int32_t>& /* value */)
{
    throw;
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::vector<double>& /* value */)
{
    throw;
}

dataDirection_t PVBaseOutImpl::getDataDirection() const
{
    return dataDirection_t::output;
}

}
