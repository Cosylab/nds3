#include "../include/nds3/ndsdelegate.h"

namespace nds
{


Delegate::~Delegate()
{

}

void Delegate::readInt32(const std::string& /* name*/, timespec* /* pTimestamp */, std::int32_t* /* pValue */)
{
    throw;
}

void Delegate::writeInt32(const std::string& /* name*/, const timespec& /* pTimestamp */, const std::int32_t& /* value */)
{
    throw;
}

}
