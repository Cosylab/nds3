#include "../include/ndsTestInterface.h"

namespace nds
{

namespace tests
{

static std::map<std::string, TestControlSystemInterfaceImpl*> m_interfacesMap;
static std::mutex m_lockInterfacesMap;


TestControlSystemInterfaceImpl::TestControlSystemInterfaceImpl(const std::string &fullName):
    m_name(fullName)
{
    std::lock_guard<std::mutex> lock(m_lockInterfacesMap);
    if(m_interfacesMap.find(fullName) != m_interfacesMap.end())
    {
        throw std::logic_error("Interface with the same name already allocated. This should not happen");
    }
    m_interfacesMap[fullName] = this;
}

TestControlSystemInterfaceImpl::~TestControlSystemInterfaceImpl()
{
    std::lock_guard<std::mutex> lock(m_lockInterfacesMap);
    m_interfacesMap.erase(m_name);

}

TestControlSystemInterfaceImpl* TestControlSystemInterfaceImpl::getInstance(const std::string &fullName)
{
    std::lock_guard<std::mutex> lock(m_lockInterfacesMap);

    std::map<std::string, TestControlSystemInterfaceImpl*>::const_iterator findInterface = m_interfacesMap.find(fullName);
    if(findInterface == m_interfacesMap.end())
    {
        return 0;
    }
    return findInterface->second;

}

void TestControlSystemInterfaceImpl::registerPV(std::shared_ptr<PVBaseImpl> pv)
{
    m_registeredPVs[pv->getFullName()] = pv.get();

}

void TestControlSystemInterfaceImpl::deregisterPV(std::shared_ptr<PVBaseImpl> pv)
{
    m_registeredPVs.erase(pv->getFullName());
}

void TestControlSystemInterfaceImpl::registrationTerminated()
{

}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::int32_t& value)
{
    storePushedData(pv.getFullName(), m_pushedInt32, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const double& value)
{
    storePushedData(pv.getFullName(), m_pushedDouble, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::int8_t> & value)
{
    storePushedData(pv.getFullName(), m_pushedVectorInt8, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::uint8_t> & value)
{
    storePushedData(pv.getFullName(), m_pushedVectorUint8, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::int32_t> & value)
{
    storePushedData(pv.getFullName(), m_pushedVectorInt32, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<double> & value)
{
    storePushedData(pv.getFullName(), m_pushedVectorDouble, timestamp, value);
}



template<typename T>
void TestControlSystemInterfaceImpl::readCSValue(const std::string& pvName, timespec* pTimestamp, T* pValue)
{
    registeredPVs_t::iterator findPV = m_registeredPVs.find(pvName);
    if(findPV == m_registeredPVs.end())
    {
        throw std::runtime_error("PV not found");
    }
    findPV->second->read(pTimestamp, pValue);
}

template void TestControlSystemInterfaceImpl::readCSValue<std::int32_t>(const std::string& pvName, timespec* timestamp, std::int32_t* value);
template void TestControlSystemInterfaceImpl::readCSValue<double>(const std::string& pvName, timespec* timestamp, double* value);
template void TestControlSystemInterfaceImpl::readCSValue<std::vector<std::int8_t> >(const std::string& pvName, timespec* timestamp, std::vector<std::int8_t>* value);
template void TestControlSystemInterfaceImpl::readCSValue<std::vector<std::uint8_t> >(const std::string& pvName, timespec* timestamp, std::vector<std::uint8_t>* value);
template void TestControlSystemInterfaceImpl::readCSValue<std::vector<std::int32_t> >(const std::string& pvName, timespec* timestamp, std::vector<std::int32_t>* value);
template void TestControlSystemInterfaceImpl::readCSValue<std::vector<double> >(const std::string& pvName, timespec* timestamp, std::vector<double>* value);


template<typename T>
void TestControlSystemInterfaceImpl::writeCSValue(const std::string& pvName, const timespec& timestamp, const T& value)
{
    registeredPVs_t::iterator findPV = m_registeredPVs.find(pvName);
    if(findPV == m_registeredPVs.end())
    {
        throw std::runtime_error("PV not found");
    }
    findPV->second->write(timestamp, value);
}

template void TestControlSystemInterfaceImpl::writeCSValue<std::int32_t>(const std::string& pvName, const timespec& timestamp, const std::int32_t& value);
template void TestControlSystemInterfaceImpl::writeCSValue<double>(const std::string& pvName, const timespec& timestamp, const double& value);
template void TestControlSystemInterfaceImpl::writeCSValue<std::vector<std::int8_t> >(const std::string& pvName, const timespec& timestamp, const std::vector<std::int8_t>& value);
template void TestControlSystemInterfaceImpl::writeCSValue<std::vector<std::uint8_t> >(const std::string& pvName, const timespec& timestamp, const std::vector<std::uint8_t>& value);
template void TestControlSystemInterfaceImpl::writeCSValue<std::vector<std::int32_t> >(const std::string& pvName, const timespec& timestamp, const std::vector<std::int32_t>& value);
template void TestControlSystemInterfaceImpl::writeCSValue<std::vector<double> >(const std::string& pvName, const timespec& timestamp, const std::vector<double>& value);


void TestControlSystemInterfaceImpl::getPushedInt32(const std::string& pvName, const timespec*& pTime, const std::int32_t*& pValue)
{
    getPushedData(pvName, m_pushedInt32, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedDouble(const std::string& pvName, const timespec*& pTime, const double*& pValue)
{
    return getPushedData(pvName, m_pushedDouble, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedVectorInt8(const std::string& pvName, const timespec*& pTime, const std::vector<std::int8_t>*& pValue)
{
    return getPushedData(pvName, m_pushedVectorInt8, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedVectorUint8(const std::string& pvName, const timespec*& pTime, const std::vector<std::uint8_t>*& pValue)
{
    return getPushedData(pvName, m_pushedVectorUint8, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedVectorInt32(const std::string& pvName, const timespec*& pTime, const std::vector<std::int32_t>*& pValue)
{
    return getPushedData(pvName, m_pushedVectorInt32, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedVectorDouble(const std::string& pvName, const timespec*& pTime, const std::vector<double>*& pValue)
{
    return getPushedData(pvName, m_pushedVectorDouble, pTime, pValue);
}

}

}
