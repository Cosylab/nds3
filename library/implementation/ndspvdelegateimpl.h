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
    PVDelegateImpl(const std::string& name, read_t readFunction, write_t writeFunction);

    /**
     * @brief Constructor for read-only PVs. Specify the methods used for reading
     *
     * @param name          PV's name
     * @param readFunction  read method
     */
    PVDelegateImpl(const std::string& name, read_t readFunction);

    virtual void read(timespec* pTimestamp, T* pValue);

    virtual void write(const timespec& timestamp, const T& value);

    virtual dataType_t getDataType();

    /**
     * @brief Pass this function as writing delegate when the PVis read-only.
     */
    void dontWrite(const timespec&, const T&);


private:
    read_t m_reader;
    write_t m_writer;

};

}
#endif // NDSDELEGATEPVIMPL_H
