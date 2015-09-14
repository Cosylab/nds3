#ifndef NDSFACTORYBASEIMPL_H
#define NDSFACTORYBASEIMPL_H

#include "../include/nds3/definitions.h"
#include <map>
#include <mutex>


namespace nds
{

class InterfaceBaseImpl;

class FactoryBaseImpl
{
public:
    virtual ~FactoryBaseImpl();

    virtual void registerDriver(const std::string& driverName, allocateDriver_t allocateFunction, deallocateDriver_t deallocateFunction);

    virtual InterfaceBaseImpl* getNewInterface(const std::string& fullName) = 0;

    virtual void run() = 0;

protected:
    void createDriver(const std::string& name, const std::string& parameter);

private:

    typedef std::map<std::string, std::pair<allocateDriver_t, deallocateDriver_t> > driverAllocDeallocMap_t;
    driverAllocDeallocMap_t m_driversAllocDealloc;

    typedef std::multimap<std::string, void*> allocatedDrivers_t;
    allocatedDrivers_t m_allocatedDrivers;

    std::mutex m_mutex;

};

}


#endif // NDSFACTORYBASEIMPL_H
