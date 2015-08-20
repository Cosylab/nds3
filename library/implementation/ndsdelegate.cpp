#include "../include/nds3/ndsdelegate.h"

namespace nds
{


Delegate::~Delegate()
{

}

void Delegate::readInt32(timespec* /* pTimestamp */, std::int32_t* /* pValue */)
{
    throw;
}

void Delegate::writeInt32(const timespec& /* pTimestamp */, const std::int32_t& /* value */)
{
    throw;
}

}
