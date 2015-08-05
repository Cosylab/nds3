#ifndef NDSPVBASE_H
#define NDSPVBASE_H

#include "ndsbase.h"
#include <cstdint>

namespace nds
{


class PVBase: public Base
{
public:
    PVBase(const std::string& name);

    // For now only int32. All the base versions throw.
    // Only the overwritten ones in the AsynPV or AsynDelegatePV
    // will function correctly.
    ////////////////////////////////////////////////////////////
    virtual void read(timespec* pTimestamp, std::int32_t* pValue);
    virtual void write(const timespec& timestamp, const std::int32_t& value);

};

}
#endif // NDSPVBASE_H
