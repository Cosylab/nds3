#ifndef NDSPVBASE_H
#define NDSPVBASE_H

#include "definitions.h"
#include "base.h"
#include <cstdint>
#include <time.h>
#include <string>

namespace nds
{

class PVBaseImpl;

/**
 * @brief Base class for objects that process a single PV (or attribute in Tango parlance).
 *
 * In NDS you can define Input PVs (represented by the class PVBaseIn and its derivates) or
 * Output PVs (represented by the class PVBaseOut and its derivates).
 *
 * The data in the input records are written by the device support and read by the control system,
 * while the output records are written by the control system and read by the device support.
 */
class NDS3_API PVBase: public Base
{
public:
    PVBase();

#ifndef SWIG
    PVBase(std::shared_ptr<PVBaseImpl> pvImpl);
#endif

    /**
     * @brief Set a human readable description of the PV.
     *
     * @param description the PV description
     */
    void setDescription(const std::string& description);

    /**
     * @brief Set the name of the PV as seen by the control system.
     *
     * By default correspond to the name given in the constructor.
     *
     * @param interfaceName the name of the PV as seen by the control system
     */
    void setInterfaceName(const std::string& interfaceName);

    /**
     * @brief Set how the PV value is retrieved by the control system.
     *
     * @param scanType      the method used by the control system to retrieve the value
     * @param periodSeconds if the scanType is set to scanType_t::periodic then specifies
     *                       the amount of seconds between the polling, otherwise it is
     *                       ignored
     */
    void setScanType(const scanType_t scanType, const double periodSeconds = 1);

    /**
     * @brief If the data type of the PV is a vector then set the maximum number
     *        of elements that the vector will contain
     *
     * @param maxElements the maximum number of elements that the vector will contain
     */
    void setMaxElements(const size_t maxElements);

    /**
     * @brief If the data type is an integer then assigns a list of strings that
     *        enumerate the values (starting from zero).
     *
     * @param enumerations list of strings that enumerate the PV's values
     */
    void setEnumeration(const enumerationStrings_t& enumerations);

};

}
#endif // NDSPVBASE_H
