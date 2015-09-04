#ifndef NDSPVBASEIMPL_H
#define NDSPVBASEIMPL_H

#include "ndsbaseimpl.h"
#include "../include/nds3/definitions.h"

#include <string>

namespace nds
{

class PVBase;

/**
 * @brief Base class for all the PVs.
 */
class PVBaseImpl: public BaseImpl
{
public:
    PVBaseImpl(const std::string& name);

    // All the base versions throw.
    // Only the overwritten ones in the AsynPV or AsynDelegatePV
    // will function correctly.
    ////////////////////////////////////////////////////////////
    template<typename T>
    void read(timespec* pTimestamp, T* pValue);

    template<typename T>
    void write(const timespec& timestamp, const T& value);

    template<typename T>
    void push(const timespec& timestamp, const T& value);

    virtual void initialize();

    /**
     * @brief Return the PV's data type.
     *
     * Used to declare Tango attributes or when auto-generating db files for EPICS.
     *
     * @return the data type used by the PV
     */
    virtual dataType_t getDataType() = 0;

    void setType(const recordType_t type);
    void setDescription(const std::string& description);
    void setInterfaceName(const std::string& interfaceName);
    void setScanType(const scanType_t scanType, const double periodSeconds);
    void setMaxElements(const size_t maxElements);

    recordType_t getType() const;
    std::string getDescription() const;
    std::string getInterfaceName() const;
    scanType_t getScanType() const;
    double getScanPeriodSeconds() const;
    size_t getMaxElements() const;

protected:
    recordType_t m_type;
    std::string m_description;
    std::string m_interfaceName;
    scanType_t m_scanType;
    double m_periodicScanSeconds;
    size_t m_maxElements;
};
}
#endif // NDSPVBASEIMPL_H
