#ifndef NDSPVVARIABLEINIMPL_H
#define NDSPVVARIABLEINIMPL_H

#include <mutex>
#include "pvBaseInImpl.h"

namespace nds
{

/**
 * @brief Template class for simple PVs (int, float)
 *
 * Stores the value locally.
 *
 */
template <typename T>
class PVVariableInImpl: public PVBaseInImpl
{
public:
    PVVariableInImpl(const std::string& name);

    virtual void read(timespec* pTimestamp, T* pValue) const;

    virtual dataType_t getDataType() const;

    void setValue(const T& value);

    void setValue(const timespec& timestamp, const T& value);

private:
    T m_value;
    timespec m_timestamp;

    mutable std::mutex m_pvMutex;

};

}
#endif // NDSPVVARIABLEINIMPL_H
