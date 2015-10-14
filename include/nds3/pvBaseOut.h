#ifndef NDSPVBASEOUT_H
#define NDSPVBASEOUT_H

#include "pvBase.h"

namespace nds
{

class PVBaseOutImpl;

/**
 * @brief Represents an output PV.
 *
 * Output PVs are written by the control system and read by the device support.
 *
 */
class NDS3_API PVBaseOut: public PVBase
{
public:
    PVBaseOut();

#ifndef SWIG
    PVBaseOut(std::shared_ptr<PVBaseOutImpl> pvImpl);
#endif

    /**
     * @ingroup datareadwrite
     * @brief Call to read the value of the PV.
     *
     * If this object manages a delegated PV then the delegated function
     * will be called.
     *
     * @tparam T           the data type to read
     * @param pTimestamp   a pointer to a timestamp value that the called function will fill with
     *                     the proper timestamp
     * @param pValue       a pointer to a variable that the called function will fill with the
     *                     proper value
     */
    template<typename T>
    void read(timespec* pTimestamp, T* pValue) const;

    /**
     * @ingroup datareadwrite
     * @brief Call to write the value into the PV.
     *
     * If this object manages a delegated PV then the delegated function
     * will be called.
     *
     * @tparam T           the data type to write
     * @param timestamp    the timestamp to assign to the new value
     * @param pValue       the new value for the PV
     */
    template<typename T>
    void write(const timespec& timestamp, const T& value);

};

}

#endif // NDSPVBASEOUT_H
