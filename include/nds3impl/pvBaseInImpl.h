#ifndef NDSPVBASEINIMPL_H
#define NDSPVBASEINIMPL_H

#include "baseImpl.h"
#include "pvBaseImpl.h"
#include "../nds3/definitions.h"

#include <string>
#include <set>
#include <mutex>

namespace nds
{

class PVBase;
class PVBaseOutImpl;

/**
 * @brief Base class for all the PVs.
 */
class PVBaseInImpl: public PVBaseImpl
{
public:
    /**
     * @brief Constructor.
     *
     * @param name the PV's name
     */
    PVBaseInImpl(const std::string& name);

    virtual void initialize(FactoryBaseImpl& controlSystem);

    virtual void deinitialize();

    virtual void read(timespec* pTimestamp, std::int32_t* pValue) const;
    virtual void read(timespec* pTimestamp, double* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::int8_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::uint8_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::int32_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<double>* pValue) const;
    virtual void read(timespec* pTimestamp, std::string* pValue) const;

    /**
     * @brief Pushes data to the control system and to the subscribed PVs.
     *
     * @tparam T the data type
     * @param timestamp    the timestamp related to the data
     * @param value        the data to push
     */
    template<typename T>
    void push(const timespec& timestamp, const T& value);

    /**
     * @brief Subscribe an output PV to this PV.
     *
     * The subscribed PV will receive all the data pushed or written into this
     *  PV.
     *
     * @param pReceiver the PV that will receive al the data
     */
    void subscribeReceiver(PVBaseOutImpl* pReceiver);

    /**
     * @brief UNsubscribe an output PV from this PV.
     *
     * @param pReceiver the output PV to unsubscribe
     */
    void unsubscribeReceiver(PVBaseOutImpl* pReceiver);

    /**
     * @brief Set the decimation factor.
     *
     * The decimation factor is the ration pushed_data/data_passed_to_control_system.
     *
     * @param decimation  the decimation factor
     */
    void setDecimation(const std::int32_t decimation);

    virtual dataDirection_t getDataDirection() const;

protected:

    /**
     * @brief List of subscribed PVs.
     */
    typedef std::set<PVBaseOutImpl*> subscribersList_t;

    /**
     * @brief List of subscribed PVs.
     */
    subscribersList_t m_subscriberOutputPVs;

    std::mutex m_lockSubscribersList; ///< Lock the access to m_subscriberOutputPVs.

    std::int32_t m_decimationFactor;  ///< Decimation factor.
    std::int32_t m_decimationCount;   ///< Keeps track of the received data/vs data pushed to the control system.

};

}
#endif // NDSPVBASEINIMPL_H
