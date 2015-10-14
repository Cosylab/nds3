#ifndef NDSFACTORYIMPL_H
#define NDSFACTORYIMPL_H

#include <string>
#include <map>
#include <list>
#include <memory>
#include <dirent.h>
#include "../include/nds3/definitions.h"

namespace nds
{

class FactoryBaseImpl;
class DynamicModule;

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
     * @brief Subscribe an output PV to changes pushed or written from another input PV.
     *        The input PV can be located on any device running in the NDS process.
     *
     * @param pushFrom  name of the PV from which we want to receive the values
     * @param pReceiver PV that will receive the values coming from the input PV
     *                  (both pushes and write operations)
     */
    //void subscribe(const std::string& pushFrom, PVBaseOutImpl* pReceiver);

    //void unsubscribe(PVBaseOutImpl* pReceiver);

    //void registerInputPV(PVBaseInImpl* pSender);
    //void deregisterInputPV(PVBaseInImpl* pSender);

private:
    typedef std::list<std::string> fileNames_t;

    static fileNames_t listFiles(const fileNames_t& folders, const std::string& prefix, const std::string& suffix);

    static fileNames_t separateFoldersList(const char* foldersList);

    typedef std::map<std::string, std::shared_ptr<FactoryBaseImpl> > controlSystems_t;
    controlSystems_t m_controlSystems;

    typedef std::list<std::shared_ptr<DynamicModule> > modules_t;

    modules_t m_modules;
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
