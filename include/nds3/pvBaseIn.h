#ifndef NDSPVBASEIN_H
#define NDSPVBASEIN_H

#include "pvBase.h"

namespace nds
{

class PVBaseInImpl;

/**
 * @brief Represents an input PV.
 *
 * Input PVs are read by the control system and written by the device support.
 *
 */
class NDS3_API PVBaseIn: public PVBase
{
public:
    PVBaseIn();

#ifndef SWIG
    PVBaseIn(std::shared_ptr<PVBaseInImpl> pvImpl);
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
     * @brief Pushes a value to the control system.
     *
     * The control system will receive the value immediately, regardless of
     * when it calls the read() function.
     *
     * @param timestamp    the new value's timestamp
     * @param value        the value to push to the control system
     */
    template<typename T>
    void push(const timespec& timestamp, const T& value);

};

}

#endif // NDSPVBASEIN_H
