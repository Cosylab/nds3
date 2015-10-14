#ifndef NDSPVDELEGATEOUT_H
#define NDSPVDELEGATEOUT_H

#include "definitions.h"
#include "pvBaseOut.h"

#ifndef SWIG // PVDelegate will not be present in SWIG generated files

namespace nds
{

/**
 * @brief An output PV that delegates the read and write operations to two external
 *        functions.
 *
 * Calling PVDelegateOut::read() or PVDelegate::write() will result in a call
 *  to the delegated functions.
 */
template <typename T>
class NDS3_API PVDelegateOut: public PVBaseOut
{
protected:
    PVDelegateOut();

public:
    /**
     * @ingroup datareadwrite
     * @brief Definition of the method used to read the initial PV value.
     *
     * The init function will receive two pointers to the timestamp and value
     *  that the function will have to fill with proper data.
     *
     */
    typedef std::function<void (timespec*, T*)> initValue_t;

    /**
     * @ingroup datareadwrite
     * @brief Definition of the method used to write.
     */
    typedef std::function<void (const timespec&, const T&)> write_t;

    /**
     * @brief Construct the PVDelegateOut object and specifies the external functions
     *        that must be called to write the data and to retrieve the initial value.
     *
     * @param name          name of the PV
     * @param readFunction  function to be used for reading
     * @param initValueFunction function to be used for initializing the value
     */
    PVDelegateOut(const std::string& name, write_t writeFunction, initValue_t initValueFunction);

#ifndef SWIG
private:
    initValue_t m_initializer;
    write_t m_writer;
#endif
};

}

#endif // SWIG
#endif // NDSPVDELEGATEIN_H
