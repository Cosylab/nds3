#ifndef NDSPVDELEGATEIN_H
#define NDSPVDELEGATEIN_H

#include "definitions.h"
#include "pvBaseIn.h"

#ifndef SWIG // PVDelegate will not be present in SWIG generated files

namespace nds
{

/**
 * @brief An input PV that delegates the read operation to an external
 *        function.
 *
 * Calling PVDelegateIn::read() will result in a call to the delegated functions.
 *
 * @warning for all the vector data types and for std::string remember to call
 *          setMaxElements() to specify the maximum size of the vector or string.
 *
 * @tparam T  the PV data type.
 *            The following data types are supported:
 *            - std::int32_t
 *            - std::double
 *            - std::vector<std::int8_t>
 *            - std::vector<std::uint8_t>
 *            - std::vector<std::int32_t>
 *            - std::vector<double>
 *            - std::string
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
