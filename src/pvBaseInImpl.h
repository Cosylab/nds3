#ifndef NDSPVBASEINIMPL_H
#define NDSPVBASEINIMPL_H

#include "baseImpl.h"
#include "pvBaseImpl.h"
#include "../include/nds3/definitions.h"

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
    PVBaseInImpl(const std::string& name);

    // All the base versions throw.
    // Only the overwritten ones in the derived classes will function correctly.
    ////////////////////////////////////////////////////////////////////////////
    virtual void read(timespec* pTimestamp, std::int32_t* pValue) const;
    virtual void read(timespec* pTimestamp, double* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::int8_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::uint8_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::int32_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<double>* pValue) const;

    template<typename T>
    void push(const timespec& timestamp, const T& value);

    void subscribeReceiver(PVBaseOutImpl* pReceiver);
    void unsubscribeReceiver(PVBaseOutImpl* pReceiver);

    virtual dataDirection_t getDataDirection() const;

protected:
    typedef std::set<PVBaseOutImpl*> subscribersList_t;
    subscribersList_t m_subscriberOutputPVs;
    std::mutex m_lockSubscribersList;

};

}
#endif // NDSPVBASEINIMPL_H
