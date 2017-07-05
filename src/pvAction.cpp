/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/pvAction.h"
#include "nds3/impl/pvActionImpl.h"

#include <cstdint>
#include <vector>

namespace nds
{

/*
 * Default constructor
 *
 *********************/
PVAction::PVAction()
{

}

/*
 * Constructor (with initialization function)
 *
 ********************************************/
PVAction::PVAction(const std::string& name, write_t writeFunction, initValue_t initValueFunction):
    PVBaseOut(std::shared_ptr<PVBaseOutImpl>(new PVActionImpl(name, writeFunction, initValueFunction)))
{}


/*
 * Constructor (without initialization function)
 *
 ***********************************************/
PVAction::PVAction(const std::string& name, write_t writeFunction):
    PVBaseOut(std::shared_ptr<PVBaseOutImpl>(new PVActionImpl(name, writeFunction)))
{}

/*
 * Called to set value back to the control system
 *
 ***************************/
void PVAction::setValueBack(const timespec& timestamp, const std::int32_t& value)
{
    std::static_pointer_cast<PVActionImpl>(m_pImplementation)->setValueBack(timestamp, value);
}

}

