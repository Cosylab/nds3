#ifndef NDSDELEGATEPVIMPL_H
#define NDSDELEGATEPVIMPL_H

#include "ndspvbaseimpl.h"

namespace nds
{

/**
 * @brief Delegating PV. Delegates the read/write to another object
 */
template <typename T>
class DelegatePVImpl: public PVBaseImpl
{
public:
    /**
     * @brief Definition of the method used to read.
     *
     * We should include also the timestamp here or AsynUser (now there isn't)
     */
    typedef std::function<void (timespec* pTimestamp, T*)> tRead;

    /**
     * @brief Definition of the method used to write.
     */
    typedef std::function<void (const timespec& pTimestamp, const T&)> tWrite;

    /**
     * @brief Constructor. Specify the methods used for read/write
     *
     * @param name          PV's name
     * @param readFunction  read method
     * @param writeFunction write method
     */
    DelegatePVImpl(const std::string& name, tRead readFunction, tWrite writeFunction): AsynPVBase(name),
        m_reader(readFunction), m_writer(writeFunction)
    {}

    virtual void read(timespec* pTimestamp, std::int32_t* pValue)
    {
        m_reader(pTimestamp, pValue);
    }

    virtual void write(const timespec& timestamp, const std::int32_t& value)
    {
        m_writer(timestamp, value);
    }

private:
    tRead m_reader;
    tWrite m_writer;

}
;
}
#endif // NDSDELEGATEPVIMPL_H
