#ifndef NDSDELEGATEPVIMPL_H
#define NDSDELEGATEPVIMPL_H

#include "ndspvbaseimpl.h"
#include "../include/nds3/definitions.h"

namespace nds
{

/**
 * @brief Delegating PV. Delegates the read/write to another object
 */
template <typename T>
class PVDelegateImpl: public PVBaseImpl
{
public:
    /**
     * @brief Definition of the method used to read.
     *
     * The read function will receive a pointer to a timespec and a pointer to
     *  a value: the function will have to fill both the timespec and value
     *  with proper data.
     */
    typedef std::function<void (timespec*, T*)> read_t;

    /**
     * @brief Definition of the method used to write.
     */
    typedef std::function<void (const timespec&, const T&)> write_t;

    /**
     * @brief Constructor. Specify the methods used for read/write
     *
     * @param name          PV's name
     * @param readFunction  read method
     * @param writeFunction write method
     */
    PVDelegateImpl(const std::string& name, read_t readFunction, write_t writeFunction): PVBaseImpl(name),
        m_reader(readFunction), m_writer(writeFunction)
    {}

    virtual void read(timespec* pTimestamp, T* pValue)
    {
        m_reader(pTimestamp, pValue);
    }

    virtual void write(const timespec& timestamp, const T& value)
    {
        m_writer(timestamp, value);
    }

    virtual dataType_t getDataType();

private:
    read_t m_reader;
    write_t m_writer;

};

}
#endif // NDSDELEGATEPVIMPL_H
