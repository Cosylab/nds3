#ifndef NDSPVIMPL_H
#define NDSPVIMPL_H

#include <mutex>
#include "pvBaseImpl.h"

namespace nds
{

/**
 * @brief Template class for simple PVs (int, float)
 *
 * Stores the value locally.
 *
 */
template <typename T>
class PVVariableImpl: public PVBaseImpl
{
public:
    PVVariableImpl(const std::string& name);

    virtual void read(timespec* pTimestamp, T* pValue);

    virtual void write(const timespec& timestamp, const T& value);

    virtual dataType_t getDataType();

private:
    T m_value;
    timespec m_timestamp;

    std::mutex m_pvMutex;

};

}
#endif // NDSPVIMPL_H
