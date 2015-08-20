#ifndef NDSDELEGATE_H
#define NDSDELEGATE_H

#include "definitions.h"
#include <time.h>
#include <cstdint>

namespace nds
{

/**
 * @brief The Delegate class
 */
class NDS3_API Delegate
{
public:
    virtual ~Delegate();

    virtual void readInt32(timespec* pTimestamp, std::int32_t* pValue);
    virtual void writeInt32(const timespec& timestamp, const std::int32_t& value);

};

}
#endif // NDSDELEGATE_H
