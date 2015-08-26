#ifndef NDSPVHOLDDELEGATE_H
#define NDSPVHOLDDELEGATE_H

#include "ndspvbase.h"

namespace nds
{

class Delegate;

class NDS3_API PVHoldDelegate: public PVBase
{
public:
    PVHoldDelegate(const std::string& name, dataType_t dataType, Delegate* pDelegate);

};

}
#endif // NDSPVHOLDDELEGATE_H
