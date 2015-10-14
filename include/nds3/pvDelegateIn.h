#ifndef NDSPVDELEGATEIN_H
#define NDSPVDELEGATEIN_H

#include "definitions.h"
#include "pvBaseIn.h"

#ifndef SWIG // PVDelegate will not be present in SWIG generated files

namespace nds
{

/**
 * @brief An input PV that delegates the read operation to a external
 *        function.
 *
 * Calling PVDelegateIn::read() will result in a call to the delegated functions.
 *
 */
template <typename T>
class NDS3_API PVDelegateIn: public PVBaseIn
{
protected:
    PVDelegateIn();

public:

    /**
     * @ingroup datareadwrite
     * @brief Definition of the method used to read the PV value.
     *
     * The read function will receive two pointers to the timestamp and value
     *  that the function will have to fill with proper data.
     *
     */
    typedef std::function<void (timespec*, T*)> read_t;

    /**
     * @brief Construct the PVDelegateIn object and specifies the external function
     *        that must be called to read the data.
     *
     * @param name          name of the PV
     * @param readFunction  function to be used for reading
     */
    PVDelegateIn(const std::string& name, read_t readFunction);

#ifndef SWIG
private:
    read_t m_reader;
#endif
};

}

#endif // SWIG
#endif // NDSPVDELEGATEIN_H
