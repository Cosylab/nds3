#ifndef NDSPVBASEOUTIMPL_H
#define NDSPVBASEOUTIMPL_H

#include "baseImpl.h"
#include "pvBaseImpl.h"
#include "../nds3/definitions.h"

#include <string>

namespace nds
{

class PVBase;

/**
 * @brief Base class for all the PVs.
 */
class PVBaseOutImpl: public PVBaseImpl
{
public:
    PVBaseOutImpl(const std::string& name);

    void subscribeTo(const std::string& inputPVName);

    virtual void initialize(FactoryBaseImpl& controlSystem);

    virtual void deinitialize();

    // All the base versions throw.
    // Only the overwritten ones in the derived classes will function correctly.
    ////////////////////////////////////////////////////////////////////////////
    virtual void read(timespec* pTimestamp, std::int32_t* pValue) const;
    virtual void read(timespec* pTimestamp, double* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::int8_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::uint8_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<std::int32_t>* pValue) const;
    virtual void read(timespec* pTimestamp, std::vector<double>* pValue) const;
    virtual void read(timespec* pTimestamp, std::string* pValue) const;

    virtual void write(const timespec& timestamp, const std::int32_t& value);
    virtual void write(const timespec& timestamp, const double& value);
    virtual void write(const timespec& timestamp, const std::vector<std::int8_t>& value);
    virtual void write(const timespec& timestamp, const std::vector<std::uint8_t>& value);
    virtual void write(const timespec& timestamp, const std::vector<std::int32_t>& value);
    virtual void write(const timespec& timestamp, const std::vector<double>& value);
    virtual void write(const timespec& timestamp, const std::string& value);

    virtual dataDirection_t getDataDirection() const;



};

}
#endif // NDSPVBASEIMPL_H
