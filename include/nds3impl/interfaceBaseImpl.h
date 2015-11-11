/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSINTERFACEIMPL_H
#define NDSINTERFACEIMPL_H

#include <list>
#include <memory>
#include "pvBaseImpl.h"

namespace nds
{

/**
 * @brief Represents the interface between NDS and the control system API.
 *
 * Classes derived from this one implement specific code for the control system they
 * interact with.
 */
class NDS3_API InterfaceBaseImpl
{
public:
    virtual ~InterfaceBaseImpl();

    /**
     * @brief Register a PV (attribute) with the control system.
     *
     * The interface will take care of calling the PV's read and write
     * functions when the control system requires it.
     *
     * In order to push values immediately to the control system use push().
     *
     * @param pv the PV to be registered
     */
    virtual void registerPV(std::shared_ptr<PVBaseImpl> pv) = 0;

    /**
     * @brief Deregister a PV (attribute) from the control system.
     *
     * @param pv the PV to be deregistered
     */
    virtual void deregisterPV(std::shared_ptr<PVBaseImpl> pv) = 0;

    /**
     * @brief Called by the nodes after all the PVs have been registered.
     *        The interface may commit all the registered PV at this point or
     *        perform additional tasks.
     */
    virtual void registrationTerminated() = 0;

    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::int32_t& value) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const double& value) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::int8_t> & value) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::uint8_t> & value) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::int32_t> & value) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<double> & value) = 0;
    virtual void push(const PVBaseImpl& pv, const timespec& timestamp, const std::string & value) = 0;
};

}
#endif // NDSINTERFACEIMPL_H
