#ifndef NDSPVHOLDDELEGATEIMPL_H
#define NDSPVHOLDDELEGATEIMPL_H

#include <memory>
#include "pvBaseImpl.h"
#include "../include/nds3/definitions.h"

namespace nds
{

class Delegate;

class PVHoldDelegateImpl: public PVBaseImpl
{
public:
    PVHoldDelegateImpl(const std::string& name, dataType_t type, std::shared_ptr<Delegate> pDelegate, dataDirection_t dataDirection);

    virtual void read(timespec* pTimestamp, std::int32_t* pValue) const;
    virtual void write(const timespec& pTimestamp, const std::int32_t& value);

    virtual dataType_t getDataType() const;

    virtual dataDirection_t getDataDirection() const;

private:
    std::shared_ptr<Delegate> m_pDelegate;

    dataType_t m_dataType;

    dataDirection_t m_dataDirection;
};

}
#endif // NDSPVHOLDDELEGATEIMPL_H
