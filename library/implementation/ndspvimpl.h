#ifndef NDSPVIMPL_H
#define NDSPVIMPL_H

#include "ndspvbaseimpl.h"

namespace nds
{

/**
 * @brief Template class for simple PVs (int, float)
 *
 * Stores the value locally.
 *
 */
template <typename T>
class PVImpl: public PVBaseImpl
{
public:
    PVImpl(const std::string& name, PV* pInterface): PVBaseImpl(name, pInterface), m_timestamp{0,0}
    {}

    virtual void read(timespec* pTimestamp, T* pValue)
    {
        *pValue = m_value;
        *pTimestamp = m_timestamp;
    }

    virtual void write(const timespec timestamp, const T& value)
    {
        m_value =value;
        m_timestamp = timestamp;
    }

private:
    T m_value;
    timespec m_timestamp;
};
}
#endif // NDSPVIMPL_H
