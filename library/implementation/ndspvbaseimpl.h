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
    virtual void read(timespec* pTimestamp, std::int32_t* pValue);
    virtual void write(const timespec& pTimestamp, const std::int32_t& value);

    virtual void read(timespec* pTimestamp, double* pValue);
    virtual void write(const timespec& pTimestamp, const double& value);

    virtual void initialize();

    /**
     * @brief Return the PV's data type.
     *
     * Used to declare Tango attributes or when auto-generating db files for EPICS.
     *
     * @return the data type used by the PV
     */
    virtual dataType_t getDataType() = 0;

    void setType(const std::string& type);
    void setDescription(const std::string& description);
    void setInterfaceName(const std::string& interfaceName);

    std::string getType();
    std::string getDescription();
    std::string getInterfaceName();

protected:
    std::string m_type;
    std::string m_description;
    std::string m_interfaceName;
};
}
#endif // NDSPVBASEIMPL_H
