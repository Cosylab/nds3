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
    PVImpl(const std::string& name, PV* pInterface): PVBaseImpl(name, pInterface)
    {}

    virtual void read(timespec* pTimestamp, T* pValue)
    {
        *pValue = m_value;
    }

    virtual void write(const timespec timestamp, const T& value)
    {
        m_value =value;
    }

private:
    T m_value;
};
}
#endif // NDSPVIMPL_H
