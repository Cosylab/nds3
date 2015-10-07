#ifndef NDSDELEGATEPVINIMPL_H
#define NDSDELEGATEPVINIMPL_H

#include "pvBaseInImpl.h"
#include "../include/nds3/definitions.h"

namespace nds
{

/**
 * @brief Delegating PV. Delegates the read/write to another object
 */
template <typename T>
class PVDelegateInImpl: public PVBaseInImpl
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
     * @brief Constructor for read-only PVs. Specify the methods used for reading
     *
     * @param name          PV's name
     * @param readFunction  read method
     */
    PVDelegateInImpl(const std::string& name, read_t readFunction);

    virtual void read(timespec* pTimestamp, T* pValue) const;

    virtual dataType_t getDataType() const;


private:
    read_t m_reader;

};

}
#endif // NDSDELEGATEPVINIMPL_H

