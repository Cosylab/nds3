#include "ndsFactoryImpl.h"
#include "factoryBaseImpl.h"
#include "scansymbols.h"
#include "/usr/include/link.h"
#include "../include/nds3/exceptions.h"
#include <cstdlib>
#include <elf.h>
#include <dlfcn.h>
#include <errno.h>
#include <sstream>
#include <string.h>

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

    typedef void* (*deviceAllocateFunction_t)(Factory& factory, const std::string& deviceName, const namedParameters_t& parameters) ;
    typedef void (*deviceDeallocateFunction_t)(void*) ;
    typedef const char* (*getDeviceNameFunction_t)() ;

    fileNames_t deviceModules = listFiles(devicesFolders, "lib", "NdsDevice.so");
    for(fileNames_t::const_iterator scanFiles(deviceModules.begin()), endFiles(deviceModules.end());
        scanFiles != endFiles;
        ++scanFiles)
    {
        std::shared_ptr<DynamicModule> module(std::make_shared<DynamicModule>(*scanFiles));

        getDeviceNameFunction_t nameFunction = (getDeviceNameFunction_t)(module->getAddress("getDeviceName"));
        deviceAllocateFunction_t allocateFunction = (deviceAllocateFunction_t)(module->getAddress("allocateDevice"));
        deviceDeallocateFunction_t deallocateFunction = (deviceDeallocateFunction_t)(module->getAddress("deallocateDevice"));

        if(allocateFunction == 0 || deallocateFunction == 0 || nameFunction == 0)
        {
            continue;
        }

        m_modules.push_back(module);
        registerDriver(nameFunction(),
                       std::bind(allocateFunction, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                       std::bind(deallocateFunction, std::placeholders::_1)
                       );

    }
}

NdsFactoryImpl::~NdsFactoryImpl()
{
    // Deregister all the nodes before deleting the control systems
    ///////////////////////////////////////////////////////////////
    for(controlSystems_t::iterator scanControlSystems(m_controlSystems.begin()), endControlSystems(m_controlSystems.end());
        scanControlSystems != endControlSystems;
        ++scanControlSystems)
    {
        scanControlSystems->second->preDelete();
    }
}

void NdsFactoryImpl::registerDriver(const std::string &driverName, allocateDriver_t allocateFunction, deallocateDriver_t deallocateFunction)
{
    for(controlSystems_t::iterator scanControlSystems(m_controlSystems.begin()), endControlSystems(m_controlSystems.end());
        scanControlSystems != endControlSystems;
        ++scanControlSystems)
    {
        scanControlSystems->second->registerDriver(driverName, allocateFunction, deallocateFunction);
    }
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
        throw;
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
