#ifndef NDSPVVARIABLE_H
#define NDSPVVARIABLE_H

#include "definitions.h"
#include "ndspvbase.h"

namespace nds
{

/**
 * @brief A PV object that also stores the value so it does not need to
 *        delegate the read and write functions.
 *
 * The application can use PVVariable::write() and PVVariable::read() to
 * modify or read the PV's value; EPICS will use the same methods to write
 * or read the value on the control system side.
 *
 */
template <typename T>
class NDS3_API PVVariable: public PVBase
{
protected:
    PVVariable();

public:
    /**
     * @brief Construct the PVVariable object.
     *
     * @param name          name of the PV
     */
    PVVariable(const std::string& name);
};

}

#endif // NDSPVVARIABLE_H
