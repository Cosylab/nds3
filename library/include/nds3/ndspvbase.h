#ifndef NDSPVBASE_H
#define NDSPVBASE_H

#include "definitions.h"
#include "ndsbase.h"
#include <cstdint>
#include <time.h>

namespace nds
{

class PVBaseImpl;

class NDS3_API PVBase: public Base
{
protected:
    PVBase();

public:
#ifndef SWIG
    PVBase(std::shared_ptr<PVBaseImpl> pvImpl);
#endif

    // For now only int32. All the base versions throw.
    // Only the overwritten ones in the AsynPV or AsynDelegatePV
    // will function correctly.
    ////////////////////////////////////////////////////////////
    void read(timespec* pTimestamp, std::int32_t* pValue);
    void write(const timespec& timestamp, const std::int32_t& value);

    void setType(const std::string& type);
    void setDescription(const std::string& description);
    void setInterfaceName(const std::string& interfaceName);

};

}
#endif // NDSPVBASE_H
