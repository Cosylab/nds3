#ifndef NDSPVVARIABLEOUTIMPL_H
#define NDSPVVARIABLEOUTIMPL_H

#include <mutex>
#include "pvBaseOutImpl.h"

namespace nds
{

/**
 * @brief Template class for simple PVs (int, float)
 *
 * Stores the value locally.
 *
 */
template <typename T>
class PVVariableOutImpl: public PVBaseOutImpl
{
public:
    PVVariableOutImpl(const std::string& name);

    virtual void read(timespec* pTimestamp, T* pValue) const;

    virtual void write(const timespec& timestamp, const T& value);

    virtual dataType_t getDataType() const;

    T getValue() const;

    void getValue(timespec* pTime, T* pValue) const;

private:
    T m_value;
    timespec m_timestamp;

    mutable std::mutex m_pvMutex;

};

}
#endif // NDSPVVARIABLEOUTIMPL_H

