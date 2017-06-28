/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include <cstdlib>
#include <link.h>
#include <elf.h>
#include <dlfcn.h>
#include <errno.h>
#include <sstream>
#include <string.h>
#include <iostream>

#include "nds3/exceptions.h"
#include "nds3/factory.h"
#include "nds3/impl/factoryBaseImpl.h"
#include "nds3/impl/ndsFactoryImpl.h"
#include "nds3/impl/pvBaseInImpl.h"
#include "nds3/impl/pvBaseOutImpl.h"

namespace nds
{

NdsFactoryImpl& NdsFactoryImpl::getInstance()
{
    static NdsFactoryImpl factory;
    return factory;
}


NdsFactoryImpl::NdsFactoryImpl()
{
    // Load all the available control systems
    /////////////////////////////////////////
    fileNames_t controlSystemsFolders;
    controlSystemsFolders.push_back(".");
    controlSystemsFolders.splice(controlSystemsFolders.end(), separateFoldersList(std::getenv("LD_LIBRARY_PATH")));
    controlSystemsFolders.splice(controlSystemsFolders.end(), separateFoldersList(std::getenv("NDS_CONTROL_SYSTEMS")));

    typedef FactoryBaseImpl* (*controSystemAllocateFunction_t)() ;

    fileNames_t controlSystemModules = listFiles(controlSystemsFolders, "lib", "NdsControlSystem.so");
    controlSystemModules.splice(controlSystemModules.end(), listFiles(controlSystemsFolders, "lib", "NdsControlSystem"));
    for(fileNames_t::const_iterator scanFiles(controlSystemModules.begin()), endFiles(controlSystemModules.end());
        scanFiles != endFiles;
        ++scanFiles)
    {
        std::shared_ptr<DynamicModule> module(std::make_shared<DynamicModule>(*scanFiles));

        controSystemAllocateFunction_t allocateFunction = (controSystemAllocateFunction_t)(module->getAddress("allocateControlSystem"));
        if(allocateFunction == 0)
        {
            continue;
        }

        m_modules.push_back(module);
        FactoryBaseImpl* pFactory = allocateFunction();
        m_controlSystems[pFactory->getName()] = std::shared_ptr<FactoryBaseImpl>(pFactory);
    }

    // Load all the available devices
    /////////////////////////////////
    fileNames_t devicesFolders;
    devicesFolders.push_back(".");
    devicesFolders.splice(devicesFolders.end(), separateFoldersList(std::getenv("LD_LIBRARY_PATH")));
    devicesFolders.splice(devicesFolders.end(), separateFoldersList(std::getenv("NDS_DEVICES")));

    fileNames_t deviceModules = listFiles(devicesFolders, "lib", "NdsDevice.so");
    for(fileNames_t::const_iterator scanFiles(deviceModules.begin()), endFiles(deviceModules.end());
        scanFiles != endFiles;
        ++scanFiles)
    {
        try
        {
            loadDriver(*scanFiles);
        }
        catch(const DriverDoesNotExportRegistrationFunctions& e)
        {
            std::cout << "Skipped library " << *scanFiles << " because it does not export the registration functions" << std::endl;
        }
    }
}

NdsFactoryImpl::~NdsFactoryImpl()
{
    // Deregister all the nodes before deleting the control systems
    ///////////////////////////////////////////////////////////////
    controlSystems_t controlSystems;
    {
        std::lock_guard<std::mutex> lock(m_lockControlSystems);
         controlSystems = m_controlSystems;
    }
    for(controlSystems_t::iterator scanControlSystems(controlSystems.begin()), endControlSystems(controlSystems.end());
        scanControlSystems != endControlSystems;
        ++scanControlSystems)
    {
        scanControlSystems->second->preDelete();
    }
}


void NdsFactoryImpl::loadDriver(const std::string& driverModuleName)
{
    std::shared_ptr<DynamicModule> module(std::make_shared<DynamicModule>(driverModuleName));

    typedef void* (*deviceAllocateFunction_t)(Factory& factory, const std::string& deviceName, const namedParameters_t& parameters) ;
    typedef void (*deviceDeallocateFunction_t)(void*) ;
    typedef const char* (*getDeviceNameFunction_t)() ;

    getDeviceNameFunction_t nameFunction = (getDeviceNameFunction_t)(module->getAddress("getDeviceName"));
    deviceAllocateFunction_t allocateFunction = (deviceAllocateFunction_t)(module->getAddress("allocateDevice"));
    deviceDeallocateFunction_t deallocateFunction = (deviceDeallocateFunction_t)(module->getAddress("deallocateDevice"));

    if(allocateFunction == 0 || deallocateFunction == 0 || nameFunction == 0)
    {
        std::ostringstream error;
        error << "The driver " << driverModuleName << " does not export the registration functions";
        throw DriverDoesNotExportRegistrationFunctions(error.str());
    }

    registerDriver(nameFunction(),
                   std::bind(allocateFunction, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                   std::bind(deallocateFunction, std::placeholders::_1)
                   );
    m_modules.push_back(module);
}

void NdsFactoryImpl::registerDriver(const std::string &driverName, allocateDriver_t allocateFunction, deallocateDriver_t deallocateFunction)
{
    std::lock_guard<std::mutex> lock(m_lockDrivers);

    if(m_driversAllocDealloc.find(driverName) != m_driversAllocDealloc.end())
    {
        // TODO(nc): Implement NDS debug. Below is not an error, but would it be
        //           nice to print if debugging NDS.
        //std::ostringstream error;
        //error << "The driver " << driverName << " has already been registered";
        //std::cerr << error.str();
        //throw DriverAlreadyRegistered(error.str());
    }
    m_driversAllocDealloc[driverName] = std::pair<allocateDriver_t, deallocateDriver_t>(allocateFunction, deallocateFunction);
}

void NdsFactoryImpl::registerControlSystem(std::shared_ptr<FactoryBaseImpl> pControlSystem)
{
    std::lock_guard<std::mutex> lock(m_lockControlSystems);
    m_controlSystems[pControlSystem->getName()] = pControlSystem;
}

std::pair<void*, deallocateDriver_t> NdsFactoryImpl::createDevice(FactoryBaseImpl& factory, const std::string& driverName, const std::string& deviceName, const namedParameters_t& parameters)
{
    allocateDriver_t allocationFunction;
    deallocateDriver_t deallocationFunction;

    {
        std::lock_guard<std::mutex> lock(m_lockDrivers);

        driverAllocDeallocMap_t::const_iterator findDriver = m_driversAllocDealloc.find(driverName);
        if(findDriver == m_driversAllocDealloc.end())
        {
            std::ostringstream error;
            error << "The driver " << driverName << " has not been registered";
            throw DriverNotFound(error.str());
        }
        allocationFunction = findDriver->second.first;
        deallocationFunction = findDriver->second.second;
    }

    Factory userFactory(factory.shared_from_this());
    void* device = allocationFunction(userFactory, deviceName, parameters);

    return std::pair<void*, deallocateDriver_t>(device, deallocationFunction);

}


void NdsFactoryImpl::registerInputPV(PVBaseInImpl *pSender)
{
    std::lock_guard<std::recursive_mutex> lockRegisteredPVs(m_lockRegisteredPVs);

    std::string fullName(pSender->getFullName());
    if(m_registeredInputPVs.find(fullName) != m_registeredInputPVs.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "The input PV with name " << fullName << " has already been registered";
        throw PVAlreadyDeclared(errorMessage.str());
    }
    if(m_registeredOutputPVs.find(fullName) != m_registeredOutputPVs.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "The input PV with name " << fullName << " has already been registered as an output PV";
        throw PVAlreadyDeclared(errorMessage.str());
    }
    m_registeredInputPVs[fullName] = pSender;
}

void NdsFactoryImpl::registerOutputPV(PVBaseOutImpl *pReceiver)
{
    std::lock_guard<std::recursive_mutex> lockRegisteredPVs(m_lockRegisteredPVs);

    std::string fullName(pReceiver->getFullName());
    if(m_registeredOutputPVs.find(fullName) != m_registeredOutputPVs.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "The output PV with name " << fullName << " has already been registered";
        throw PVAlreadyDeclared(errorMessage.str());
    }
    if(m_registeredInputPVs.find(fullName) != m_registeredInputPVs.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "The output PV with name " << fullName << " has already been registered as an input PV";
        throw PVAlreadyDeclared(errorMessage.str());
    }
    m_registeredOutputPVs[fullName] = pReceiver;
}

void NdsFactoryImpl::deregisterInputPV(PVBaseInImpl *pSender)
{
    std::lock_guard<std::recursive_mutex> lockRegisteredPVs(m_lockRegisteredPVs);

    m_registeredInputPVs.erase(pSender->getFullName());

    for(registeredInputPVs_t::iterator scanInputs(m_registeredInputPVs.begin()), endInputs(m_registeredInputPVs.end());
        scanInputs != endInputs;
        ++scanInputs)
    {
        scanInputs->second->stopReplicationTo(pSender);
    }
}

void NdsFactoryImpl::deregisterOutputPV(PVBaseOutImpl *pReceiver)
{
    std::lock_guard<std::recursive_mutex> lockRegisteredPVs(m_lockRegisteredPVs);

    m_registeredOutputPVs.erase(pReceiver->getFullName());

    for(registeredInputPVs_t::iterator scanInputs(m_registeredInputPVs.begin()), endInputs(m_registeredInputPVs.end());
        scanInputs != endInputs;
        ++scanInputs)
    {
        scanInputs->second->unsubscribeReceiver(pReceiver);
    }
}

void NdsFactoryImpl::subscribe(const std::string &pushFrom, PVBaseOutImpl *pReceiver)
{
    std::lock_guard<std::recursive_mutex> lockRegisteredPVs(m_lockRegisteredPVs);

    // Sanity check
    registeredOutputPVs_t::const_iterator findOutput = m_registeredOutputPVs.find(pReceiver->getFullName());
    if(findOutput == m_registeredOutputPVs.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "The output PV " << pReceiver->getFullName() << " was never registered";
        throw std::logic_error(errorMessage.str());
    }
    if(findOutput->second != pReceiver)
    {
        std::ostringstream errorMessage;
        errorMessage << "A different output PV " << pReceiver->getFullName() << " was registered";
        throw std::logic_error(errorMessage.str());
    }

    registeredInputPVs_t::iterator findInput = m_registeredInputPVs.find(pushFrom);
    if(findInput == m_registeredInputPVs.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "Cannot subscribe " << pReceiver->getFullName() << " to " << pushFrom << " because the input PV cannot be located";
        throw MissingInputPV(errorMessage.str());
    }

    findInput->second->subscribeReceiver(pReceiver);
}

void NdsFactoryImpl::subscribe(const std::string& pushFrom, const std::string& pushTo)
{
    std::lock_guard<std::recursive_mutex> lockRegisteredPVs(m_lockRegisteredPVs);

    registeredOutputPVs_t::const_iterator findOutput = m_registeredOutputPVs.find(pushTo);
    if(findOutput == m_registeredOutputPVs.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "Cannot subscribe " << pushTo << " to " << pushFrom << " because the output PV cannot be located";
        throw MissingOutputPV(errorMessage.str());
    }
    subscribe(pushFrom, findOutput->second);

}

void NdsFactoryImpl::unsubscribe(PVBaseOutImpl *pReceiver)
{
    std::lock_guard<std::recursive_mutex> lockRegisteredPVs(m_lockRegisteredPVs);

    for(registeredInputPVs_t::iterator scanInputs(m_registeredInputPVs.begin()), endInputs(m_registeredInputPVs.end());
        scanInputs != endInputs;
        ++scanInputs)
    {
        scanInputs->second->unsubscribeReceiver(pReceiver);
    }
}

void NdsFactoryImpl::unsubscribe(const std::string &pushTo)
{
    std::lock_guard<std::recursive_mutex> lockRegisteredPVs(m_lockRegisteredPVs);

    registeredOutputPVs_t::const_iterator findOutput = m_registeredOutputPVs.find(pushTo);
    if(findOutput == m_registeredOutputPVs.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "Cannot unsubscribe " << pushTo << " because the output PV cannot be located";
        throw MissingOutputPV(errorMessage.str());
    }
    unsubscribe(findOutput->second);
}

void NdsFactoryImpl::replicate(const std::string &replicateSource, const std::string &replicateDestination)
{
    std::lock_guard<std::recursive_mutex> lockRegisteredPVs(m_lockRegisteredPVs);

    registeredInputPVs_t::const_iterator findDestination = m_registeredInputPVs.find(replicateDestination);
    if(findDestination == m_registeredInputPVs.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "Cannot replicate " << replicateSource << " to " << replicateDestination << " because the destination PV cannot be located";
        throw MissingDestinationPV(errorMessage.str());
    }
    replicate(replicateSource, findDestination->second);
}

void NdsFactoryImpl::replicate(const std::string &replicateSource, PVBaseInImpl *pDestination)
{
    std::lock_guard<std::recursive_mutex> lockRegisteredPVs(m_lockRegisteredPVs);

    // Sanity check
    registeredInputPVs_t::const_iterator findDestination = m_registeredInputPVs.find(pDestination->getFullName());
    if(findDestination == m_registeredInputPVs.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "The input PV " << pDestination->getFullName() << " was never registered";
        throw std::logic_error(errorMessage.str());
    }
    if(findDestination->second != pDestination)
    {
        std::ostringstream errorMessage;
        errorMessage << "A different input PV " << pDestination->getFullName() << " was registered";
        throw std::logic_error(errorMessage.str());
    }

    registeredInputPVs_t::iterator findInput = m_registeredInputPVs.find(replicateSource);
    if(findInput == m_registeredInputPVs.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "Cannot replicate " << replicateSource << " to " << pDestination->getFullName() <<" because the source PV cannot be located";
        throw MissingInputPV(errorMessage.str());
    }

    findInput->second->replicateTo(pDestination);
}

void NdsFactoryImpl::stopReplicationTo(PVBaseInImpl *pDestination)
{
    std::lock_guard<std::recursive_mutex> lockRegisteredPVs(m_lockRegisteredPVs);

    for(registeredInputPVs_t::iterator scanInputs(m_registeredInputPVs.begin()), endInputs(m_registeredInputPVs.end());
        scanInputs != endInputs;
        ++scanInputs)
    {
        scanInputs->second->stopReplicationTo(pDestination);
    }
}

void NdsFactoryImpl::stopReplicationTo(const std::string &replicateDestination)
{
    std::lock_guard<std::recursive_mutex> lockRegisteredPVs(m_lockRegisteredPVs);

    registeredInputPVs_t::const_iterator findDestination = m_registeredInputPVs.find(replicateDestination);
    if(findDestination == m_registeredInputPVs.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "Cannot stop the replication of " << replicateDestination << " because the input PV cannot be located";
        throw MissingInputPV(errorMessage.str());
    }
    stopReplicationTo(findDestination->second);

}


std::shared_ptr<FactoryBaseImpl> NdsFactoryImpl::getControlSystem(const std::string& controlSystem)
{
    if(controlSystem.empty())
    {
        if(m_controlSystems.empty())
        {
            throw;
        }
        return m_controlSystems.begin()->second;
    }
    controlSystems_t::iterator findControlSystem = m_controlSystems.find(controlSystem);
    if(findControlSystem == m_controlSystems.end())
    {
        std::ostringstream errorMessage;
        errorMessage << "The control system " << controlSystem << " cannot be located.";
        throw ControlSystemNotFound(errorMessage.str());
    }
    return findControlSystem->second;
}

NdsFactoryImpl::fileNames_t NdsFactoryImpl::listFiles(const fileNames_t& folders, const std::string& prefix, const std::string& suffix)
{
    fileNames_t files;

    for(fileNames_t::const_iterator scanFolders(folders.begin()), endFolders(folders.end());
        scanFolders != endFolders;
        ++scanFolders)
    {
        Directory directory(*scanFolders);

        for(std::string fileName = directory.getNextFileName(); !fileName.empty(); fileName = directory.getNextFileName())
        {
            if(fileName.size() < prefix.size() + suffix.size())
            {
                continue;
            }
            if(fileName.substr(0, prefix.size()) != prefix ||
                    fileName.substr(fileName.size() - suffix.size()) != suffix)
            {
                continue;
            }

            std::ostringstream fullName;
            fullName << *scanFolders << "/" << fileName;
            files.push_back(fullName.str());
        }
    }

    return files;

}

NdsFactoryImpl::fileNames_t NdsFactoryImpl::separateFoldersList(const char* foldersList)
{
    fileNames_t folders;

    if(foldersList == 0)
    {
        return folders;
    }

    const std::string foldersListString(foldersList);

    static const std::string separators(":; ");
    static const std::string quotes("'\"");

    char openQuotes = 0;

    size_t startString (0);

    for(size_t scanString(0), endString(foldersListString.size()); scanString != endString; ++scanString)
    {
        const char currentChar(foldersListString.at(scanString));
        if(currentChar == openQuotes)
        {
            openQuotes = 0;
            continue;
        }
        if(openQuotes != 0)
        {
            continue;
        }
        if(quotes.find(currentChar) != std::string::npos)
        {
            openQuotes = currentChar;
            continue;
        }
        if(separators.find(currentChar) != std::string::npos)
        {
            if(scanString != startString)
            {
                folders.push_back(foldersListString.substr(startString, scanString - startString));
            }
            startString = scanString + 1;
        }
    }
    if(startString != foldersListString.size())
    {
        folders.push_back(foldersListString.substr(startString));
    }

    return folders;
}


DynamicModule::DynamicModule(const std::string& libraryName):
    m_moduleHandle(dlopen(libraryName.c_str(), RTLD_NOW | RTLD_GLOBAL | RTLD_NODELETE))
{
    if (!m_moduleHandle)
            fprintf(stderr, "%s\n", dlerror());
}

DynamicModule::~DynamicModule()
{
    if(m_moduleHandle != 0)
    {
        dlclose(m_moduleHandle);
    }
}

void* DynamicModule::getAddress(const std::string &functionName)
{
    return dlsym(m_moduleHandle, functionName.c_str());
}


Directory::Directory(const std::string &directory)
{
    m_pDirectory = opendir(directory.c_str());
    if(m_pDirectory == 0)
    {
        std::ostringstream exceptionMessage;
        exceptionMessage << strerror(errno) << " while opening the directory " << directory << std::endl;
        throw DirectoryNotFoundError(exceptionMessage.str());
    }
}

Directory::~Directory()
{
    closedir(m_pDirectory);
}

std::string Directory::getNextFileName()
{
    if(m_pDirectory == 0)
    {
        return "";
    }

    dirent* directoryEntry = readdir(m_pDirectory);
    if(directoryEntry == 0)
    {
        return "";
    }

    return directoryEntry->d_name;
}

}
