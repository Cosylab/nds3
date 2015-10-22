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
     * If one or more output PVs have been suscribed to this PV via Factory::subscribe()
     * then the value will be pushed immediately to all the subscribed PVs via PVBaseOut::write().
     *
     * See also Factory::subscribe() and PVBaseOut::subscribeTo().
     *
     * @warning Only one thread can push data on one PV at any given time:
     *          two or more different threads can push data on different PVs
     *          but not on the same PV.\n
     *          If several threads push data on the same PV then the decimation
     *           counter may become corrupted and stop the pushing operation.
     *
     * @param timestamp    the new value's timestamp
     * @param value        the value to push to the control system
     */
    template<typename T>
    void push(const timespec& timestamp, const T& value);

    /**
     * @ingroup datareadwrite
     * @brief Specifies the decimation factor used when pushing data to the control system.
     *
     * When calling push() the data is transferred to the subscribed output PVs (see
     *  PVBaseOut::subscribeTo() ) and to the control system. However, the data pushed
     *  to the control system also takes into account the decimation factor.
     *
     * If the decimation factor is 1 then all the pushed data is also passed to the control
     *  system, if the decimation factor is 0 then none of the data is passed to the
     *  control system, if the decimation factor is higher than one then it specifies
     *  the ratio pushed_data/passed_to_CS which express the number of pushed data versus
     *  the number of data actually passed to the control system.
     *
     * @warning It is not safe to set the decimation factor while push() is being called.
     *          The decimation factor should be set before starting the data acquisition.
     *
     * @param decimation   the decimation factor (ratio pusehd_data/data_passd_to_cs)
     */
    void setDecimation(const std::int32_t decimation);

};

}

#endif // NDSPVBASEIN_H
