#ifndef NDSPVDELEGATE_H
#define NDSPVDELEGATE_H

#include "ndspvbase.h"

namespace nds
{

/**
 * @brief Definition of the method used to read.
 *
 * We should include also the timestamp here or AsynUser (now there isn't)
 */
typedef std::function<void (timespec*, T*)> tRead;

/**
 * @brief Definition of the method used to write.
 */
typedef std::function<void (const timespec&, const T&)> tWrite;

template <typename T>
class PVDelegate: public PVBase
{
public:
    PVDelegate(const std::string& name, tRead readFunction, tWrite writeFunction):
        PVBase(name), m_reader(readFunction), m_writer(writeFunction)
    {}

    virtual void read(timespec* pTimestamp, T* pValue)
    {
        m_reader(pTimestamp, pValue);
    }

    virtual void write(const timespec& timestamp, const T& value)
    {
        m_writer(timestamp, value);
    }

private:
    tRead m_reader;
    tWrite m_writer;
};

}
#endif // NDSPVDELEGATE_H
