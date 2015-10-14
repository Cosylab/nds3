#ifndef NDSPVVARIABLEOUT_H
#define NDSPVVARIABLEOUT_H

#include "definitions.h"
#include "pvBaseOut.h"

namespace nds
{

/**
 * @brief An output PV object that also stores the value so it does not need to
 *        delegate the read and write functions.
 *
 * The application can use PVVariable::getValue()to retrieve the PV's value;
 * the control system will use the methods read() and write() to read and set the value.
 *
 */
template <typename T>
class NDS3_API PVVariableOut: public PVBaseOut
{
public:

    PVVariableOut();

    /**
     * @brief Construct the PVVariable object.
     *
     * @param name          name of the PV
     */
    PVVariableOut(const std::string& name);

    /**
     * @ingroup datareadwrite
     * @brief Retrieve the value stored in the PV.
     *
     * @return the value stored in the PV.
     */
    T getValue() const;

    /**
     * @ingroup datareadwrite
     * @brief Retrieve the value and the timestamp stored in the PV.
     *
     * @param pTime  pointer to a timespec structure taht will be filled with the PV's timestamp
     * @param pValue pointer to a variable that will be filled with the PV's value
     */
    void getValue(timespec* pTime, T* pValue) const;
};

}

#endif // NDSPVVARIABLEOUT_H
