#ifndef NDSPVBASE_H
#define NDSPVBASE_H

#include "definitions.h"
#include "ndsbase.h"
#include <cstdint>
#include <time.h>
#include <string>

namespace nds
{

class PVBaseImpl;

/**
 * @brief Base class for objects that process a single PV (or attribute in Tango parlance).
 */
class NDS3_API PVBase: public Base
{
public:
    PVBase();

#ifndef SWIG
    PVBase(std::shared_ptr<PVBaseImpl> pvImpl);
#endif

    /**
     * @brief Call to read the value of the PV.
     *
     * If this object manages a delegated PV then the delegated function
     * will be called.
     *
     * @tparam T           the data type to read
     * @param pTimestamp   a pointer to a timestamp value that the called function will fill with
     *                     the proper timestamp
     * @param pValue       a pointer to a variable that the called function will fill with the
     *                     proper value
     */
    template<typename T>
    void read(timespec* pTimestamp, T* pValue);

    /**
     * @brief Call to write the value into the PV.
     *
     * If this object manages a delegated PV then the delegated function
     * will be called.
     *
     * @tparam T           the data type to write
     * @param timestamp    the timestamp to assign to the new value
     * @param pValue       the new value for the PV
     */
    template<typename T>
    void write(const timespec& timestamp, const T& value);

    /**
     * @brief Pushes a value to the control system.
     *
     * The control system will receive the value immediately, regardless of
     * when it calls the read() function.
     *
     * @param timestamp    the new value's timestamp
     * @param value        the value to push to the control system
     */
    template<typename T>
    void push(const timespec& timestamp, const T& value);

    void setType(const recordType_t type);
    void setDescription(const std::string& description);
    void setInterfaceName(const std::string& interfaceName);
    void setScanType(const scanType_t scanType, const double periodSeconds);
    void setMaxElements(const size_t maxElements);

    void setEnumeration(const enumerationStrings_t& enumerations);

};

}
#endif // NDSPVBASE_H
