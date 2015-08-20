#ifndef NDSPVHOLDDELEGATEIMPL_H
#define NDSPVHOLDDELEGATEIMPL_H

#include "ndspvbaseimpl.h"

namespace nds
{

class Delegate;

class PVHoldDelegateImpl: public PVBaseImpl
{
public:
    PVHoldDelegateImpl(const std::string& name, Delegate* pDelegate);

    virtual void read(timespec* pTimestamp, std::int32_t* pValue);
    virtual void write(const timespec& pTimestamp, const std::int32_t& value);

private:
    std::unique_ptr<Delegate> m_pDelegate;
};

}
#endif // NDSPVHOLDDELEGATEIMPL_H
