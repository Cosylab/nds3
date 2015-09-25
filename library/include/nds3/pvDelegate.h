#ifndef NDSPVDELEGATE_H
#define NDSPVDELEGATE_H

#include "definitions.h"
#include "pvBase.h"

#ifndef SWIG // PVDelegate will not be present in SWIG generated files

namespace nds
{

/**
 * @brief A PV that delegates the read and write operations to two external
 *        functions
 *
 * Calling PVDelegate::read() or PVDelegate::write() will result in a call
 *  to the delegated functions.
 */
template <typename T>
class NDS3_API PVDelegate: public PVBase
{
protected:
    PVDelegate();

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
     * @brief Construct the PVDelegate object and specifies the external functions
     *        that must be called to read & write data.
     *
     * @param name          name of the PV
     * @param readFunction  function to be used for reading
     * @param writeFunction function to be used for writing
     */
    PVDelegate(const std::string& name, read_t readFunction, write_t writeFunction);

    PVDelegate(const std::string& name, read_t readFunction);

#ifndef SWIG
private:
    read_t m_reader;
    write_t m_writer;
#endif
};

}

#endif // SWIG
#endif // NDSPVDELEGATE_H
