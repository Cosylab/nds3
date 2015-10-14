#ifndef NDSPVHOLDDELEGATE_H
#define NDSPVHOLDDELEGATE_H

#include "pvBase.h"
#include <memory>

namespace nds
{

class Delegate;

class NDS3_API PVHoldDelegate: public PVBase
{
public:
    PVHoldDelegate(const std::string& name, dataType_t dataType, std::shared_ptr<Delegate> pDelegate, dataDirection_t dataDirection);

};

}
#endif // NDSPVHOLDDELEGATE_H
