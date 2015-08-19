#ifndef NDSPVBASEIMPL_H
#define NDSPVBASEIMPL_H

#include "ndsbaseimpl.h"

namespace nds
{

class PVBase;

/**
 * @brief Base class for all the PVs.
 */
class PVBaseImpl: public BaseImpl
{
public:
    PVBaseImpl(const std::string& name);

    // For now only int32. All the base versions throw.
    // Only the overwritten ones in the AsynPV or AsynDelegatePV
    // will function correctly.
    ////////////////////////////////////////////////////////////
    virtual void read(timespec* pTimestamp, std::int32_t* pValue);
    virtual void write(const timespec& pTimestamp, const std::int32_t& value);

    virtual void initialize();

};
}
#endif // NDSPVBASEIMPL_H
