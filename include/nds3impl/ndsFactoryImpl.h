/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSFACTORYIMPL_H
#define NDSFACTORYIMPL_H

#include <string>
#include <map>
#include <list>
#include <memory>
#include <mutex>
#include <dirent.h>
#include "../nds3/definitions.h"

namespace nds
{

class FactoryBaseImpl;
class DynamicModule;
class PVBaseInImpl;
class PVBaseOutImpl;

class NdsFactoryImpl
{
public:
    static NdsFactoryImpl& getInstance();

    NdsFactoryImpl();

    ~NdsFactoryImpl();

    std::shared_ptr<FactoryBaseImpl> getControlSystem(const std::string& controlSystem);

    void loadDriver(const std::string& driverModuleName);

    /**
     * @brief Called to register the functions that allocate and deallocate a device.
     *
     * @param driverName         the name of the device driver
     * @param allocateFunction   the allocation function.
     * @param deallocateFunction the deallocation function
     */
    void registerDriver(const std::string& driverName, allocateDriver_t allocateFunction, deallocateDriver_t deallocateFunction);

    /**
     * @brief Register a new control system
     * @param pControlSystem the control system to register
     */
    void registerControlSystem(std::shared_ptr<FactoryBaseImpl> pControlSystem);

    /**
     * @brief Creates a device for the specified control system
     *
     * @param factory            the control system to which the device must be bound
     * @param driverName         the name of the device driver
     * @param allocateFunction   the allocation function.
     * @param deallocateFunction the deallocation function
     */
    std::pair<void*, deallocateDriver_t> createDevice(FactoryBaseImpl& factory, const std::string& driverName, const std::string& deviceName, const namedParameters_t& parameters);

    /**
     * @brief Subscribe an output PV to changes pushed or written from another input PV.
     *        The input PV can be located on any device running in the NDS process.
     *
     * @param pushFrom  name of the PV from which we want to receive the values
     * @param pReceiver PV that will receive the values coming from the input PV
     *                  (both pushes and write operations)
     */
    void subscribe(const std::string& pushFrom, PVBaseOutImpl* pReceiver);

    void subscribe(const std::string& pushFrom, const std::string& pushTo);

    void unsubscribe(PVBaseOutImpl* pReceiver);

    void unsubscribe(const std::string& pushTo);

    void replicate(const std::string& replicateSource, PVBaseInImpl* pDestination);

    void replicate(const std::string& replicateSource, const std::string& replicateDestination);

    void stopReplicationTo(PVBaseInImpl* pDestination);

    void stopReplicationTo(const std::string& replicateDestination);


    void registerInputPV(PVBaseInImpl* pSender);
    void deregisterInputPV(PVBaseInImpl* pSender);
    void registerOutputPV(PVBaseOutImpl* pReceiver);
    void deregisterOutputPV(PVBaseOutImpl* pReceiver);

private:
    typedef std::list<std::string> fileNames_t;

    static fileNames_t listFiles(const fileNames_t& folders, const std::string& prefix, const std::string& suffix);

    static fileNames_t separateFoldersList(const char* foldersList);

    typedef std::map<std::string, std::shared_ptr<FactoryBaseImpl> > controlSystems_t;
    controlSystems_t m_controlSystems;
    std::mutex m_lockControlSystems;

    typedef std::map<std::string, std::pair<allocateDriver_t, deallocateDriver_t> > driverAllocDeallocMap_t;
    driverAllocDeallocMap_t m_driversAllocDealloc;
    std::mutex m_lockDrivers;

    typedef std::list<std::shared_ptr<DynamicModule> > modules_t;
    modules_t m_modules;

    typedef std::map<std::string, PVBaseInImpl*> registeredInputPVs_t;
    registeredInputPVs_t m_registeredInputPVs;

    typedef std::map<std::string, PVBaseOutImpl*> registeredOutputPVs_t;
    registeredOutputPVs_t m_registeredOutputPVs;

    std::recursive_mutex m_lockRegisteredPVs;
};

/**
 * @internal
 * @brief Wrapper around a dynamically loaded module.
 *
 * The constructor loads the specified module and keeps it in memory until
 *  the destructor is called.
 */
class DynamicModule
{
public:
    DynamicModule(const std::string& libraryName);
    ~DynamicModule();

    void* getAddress(const std::string& functionName);

public:
    void* m_moduleHandle;
};


/**
 * @internal
 * @brief Wrapper around a directory.
 */
class Directory
{
public:
    Directory(const std::string& directory);
    ~Directory();

    std::string getNextFileName();

private:
    DIR* m_pDirectory;
};

}
#endif // NDSFACTORYIMPL_H
