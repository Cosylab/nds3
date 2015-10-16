#ifndef NDSDELEGATEPVOUTIMPL_H
#define NDSDELEGATEPVOUTIMPL_H

#include "pvBaseOutImpl.h"
#include "../nds3/definitions.h"

namespace nds
{

/**
 * @brief Delegating PV. Delegates the read/write to another object
 */
template <typename T>
class PVDelegateOutImpl: public PVBaseOutImpl
{
public:
    typedef std::function<void (timespec*, T*)> initValue_t;

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
    PVDelegateOutImpl(const std::string& name, write_t writeFunction, initValue_t initValueFunction);

    PVDelegateOutImpl(const std::string& name, write_t writeFunction);

    virtual void read(timespec* pTimestamp, T* pValue) const;

    virtual void write(const timespec& timestamp, const T& value);

    virtual dataType_t getDataType() const;


private:
    write_t m_writer;
    initValue_t m_initializer;

    bool m_bInitialize;

    void dontInitialize(timespec*, T*);

};

}
#endif // NDSDELEGATEPVOUTIMPL_H
