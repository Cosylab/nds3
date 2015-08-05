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
    typedef std::function<void (T*)> tRead;

    /**
     * @brief Definition of the method used to write.
     */
    typedef std::function<void (T)> tWrite;

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

    virtual void read(asynUser* pUser, T* pValue)
    {
        m_reader(pValue);
    }

    virtual void write(asynUser* pUser, T* pValue)
    {
        m_writer(*pValue);
    }

private:
    tRead m_reader;
    tWrite m_writer;

}
;
}
#endif // NDSDELEGATEPVIMPL_H
