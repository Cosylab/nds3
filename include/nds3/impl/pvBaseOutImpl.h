/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSPVBASEOUTIMPL_H
#define NDSPVBASEOUTIMPL_H

#include <string>
#include "nds3/definitions.h"
#include "nds3/impl/baseImpl.h"
#include "nds3/impl/pvBaseImpl.h"

namespace nds
{

class PVBase;

/**
 * @brief Base class for all the output PVs.
 */
class PVBaseOutImpl: public PVBaseImpl
{
public:
    /**
     * @brief Constructor.
     *
     * @param name the PV's name
     */
    PVBaseOutImpl(const std::string& name, const outputPvType_t pvType);

    /**
     * @brief Subscribe the PV to another input PV.
     *
     * @param inputPVName the name of the input PV from which we want to receive
     *                    the values
     */
    void subscribeTo(const std::string& inputPVName);

    virtual void initialize(FactoryBaseImpl& controlSystem);

    virtual void deinitialize();

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

    virtual std::string buildFullExternalName(const FactoryBaseImpl& controlSystem) const;
    virtual std::string buildFullExternalNameFromPort(const FactoryBaseImpl& controlSystem) const;

protected:
    std::string buildFullExternalName(const FactoryBaseImpl& controlSystem, const bool bStopAtPort) const;

    outputPvType_t m_pvType;

private:

    parameters_t commandSubscribeTo(const parameters_t& parameters);

};

}
#endif // NDSPVBASEIMPL_H
