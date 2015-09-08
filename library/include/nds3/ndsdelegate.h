#ifndef NDSDELEGATE_H
#define NDSDELEGATE_H

#include "definitions.h"
#include <time.h>
#include <memory>
#include <cstdint>

namespace nds
{

/**
 * @brief The Delegate class to be used with PVHoldDelegate.
 *
 * The ownership of this object is transferred to PVHoldDelegate, which deletes it
 *  when no longer needed.
 */
class NDS3_API Delegate
{
public:
    virtual ~Delegate();

    virtual void readInt32(const std::string& pvName, timespec* pTimestamp, std::int32_t* pValue);
    virtual void writeInt32(const std::string& pvName, const timespec& timestamp, const std::int32_t& value);
};

}
#endif // NDSDELEGATE_H
