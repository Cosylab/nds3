#ifdef NDS3_TANGO

#include "tangoInterfaceImpl.h"
#include "tangoFactoryImpl.h"
#include "pvBaseImpl.h"
#include "nodeImpl.h"
#include "../include/nds3/exceptions.h"

#include <cstdio>
namespace nds
{

TangoInterfaceImpl::TangoInterfaceImpl(const string &portName, NdsDevice* pDevice): InterfaceBaseImpl(portName), m_pDevice(pDevice)
{

}

void TangoInterfaceImpl::registerPV(std::shared_ptr<PVBaseImpl> pv)
{
    recordType_t pvType = pv->getType();
    Tango::AttrWriteType writeType(Tango::READ);
    if(((int)pvType & 0x00c0) == 0x0040)
    {
        writeType = Tango::WRITE;
    }
    else if(((int)pvType & 0x00c0) == 0x00c0)
    {
        throw;
    }

    Tango::CmdArgType tangoType;
    switch(pv->getDataType())
    {
    case dataType_t::dataInt8:
        tangoType = Tango::DEV_UCHAR;
        m_pDevice->add_attribute(new NdsAttributeScalar<std::uint8_t, Tango::DevUChar>(pv->getFullName(), pv, tangoType, writeType));
        break;
    case dataType_t::dataUint8:
        tangoType = Tango::DEV_UCHAR;
        m_pDevice->add_attribute(new NdsAttributeScalar<std::uint8_t, Tango::DevUChar>(pv->getFullName(), pv, tangoType, writeType));
        break;
    case dataType_t::dataInt32:
        tangoType = Tango::DEV_LONG;
        m_pDevice->add_attribute(new NdsAttributeScalar<std::int32_t, Tango::DevLong>(pv->getFullName(), pv, tangoType, writeType));
        break;
    case dataType_t::dataUint32:
        tangoType = Tango::DEV_ULONG;
        m_pDevice->add_attribute(new NdsAttributeScalar<std::uint32_t, Tango::DevULong>(pv->getFullName(), pv, tangoType, writeType));
        break;
    case dataType_t::dataFloat64:
        tangoType = Tango::DEV_DOUBLE;
        m_pDevice->add_attribute(new NdsAttributeScalar<double, Tango::DevDouble>(pv->getFullName(), pv, tangoType, writeType));
        break;
    case dataType_t::dataInt8Array:
        tangoType = Tango::DEV_UCHAR;
        m_pDevice->add_attribute(new NdsAttributeSpectrum<std::uint8_t, Tango::DevUChar>(pv->getFullName(), pv, tangoType, writeType, pv->getMaxElements()));
        break;
    case dataType_t::dataUint8Array:
        tangoType = Tango::DEV_UCHAR;
        m_pDevice->add_attribute(new NdsAttributeSpectrum<std::uint8_t, Tango::DevUChar>(pv->getFullName(), pv, tangoType, writeType, pv->getMaxElements()));
        break;
    case dataType_t::dataInt32Array:
        tangoType = Tango::DEV_LONG;
        m_pDevice->add_attribute(new NdsAttributeSpectrum<std::int32_t, Tango::DevLong>(pv->getFullName(), pv, tangoType, writeType, pv->getMaxElements()));
        break;
    case dataType_t::dataFloat64Array:
        tangoType = Tango::DEV_DOUBLE;
        m_pDevice->add_attribute(new NdsAttributeSpectrum<double, Tango::DevDouble>(pv->getFullName(), pv, tangoType, writeType, pv->getMaxElements()));
        break;
    case dataType_t::dataString:
        tangoType = Tango::DEV_STRING;
        break;
    }

    // Find the root node and store it in the device
    std::shared_ptr<NodeImpl> rootNode;
    for(rootNode = pv->getParent(); rootNode->getParent() != 0; rootNode = rootNode->getParent())
    {}
    m_pDevice->setRootNode(rootNode);

}

void TangoInterfaceImpl::deregisterPV(std::shared_ptr<PVBaseImpl> pv)
{
    try
    {
        std::string attributeName(pv->getFullName());
        std::cout << "Deregistering " << attributeName << std::endl;
        m_pDevice->remove_attribute(attributeName, true, true);
    }
    catch(const Tango::DevFailed& exception)
    {
        Tango::DevErrorList errors = exception.errors;
        for(size_t scanErrors(0); scanErrors != errors.length(); ++scanErrors)
        {
            std::cout << errors[scanErrors].reason << std::endl;
        }
    }

}

void TangoInterfaceImpl::registrationTerminated()
{

}

void TangoInterfaceImpl::push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::int8_t& value)
{
    timeval tangoTimestamp = NdsAttributeBase::NDSTimeToTangoTime(timestamp);
    m_pDevice->push_change_event(pv->getFullName(), (Tango::DevUChar*)&value, tangoTimestamp, Tango::ATTR_VALID, 1, 0, false);
}

void TangoInterfaceImpl::push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::uint8_t& value)
{
    timeval tangoTimestamp = NdsAttributeBase::NDSTimeToTangoTime(timestamp);
    m_pDevice->push_change_event(pv->getFullName(), (Tango::DevUChar*)&value, tangoTimestamp, Tango::ATTR_VALID, 1, 0, false);
}

void TangoInterfaceImpl::push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::int32_t& value)
{
    timeval tangoTimestamp = NdsAttributeBase::NDSTimeToTangoTime(timestamp);
    m_pDevice->push_change_event(pv->getFullName(), (Tango::DevLong*)&value, tangoTimestamp, Tango::ATTR_VALID, 1, 0, false);
}

void TangoInterfaceImpl::push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::uint32_t& value)
{
    timeval tangoTimestamp = NdsAttributeBase::NDSTimeToTangoTime(timestamp);
    m_pDevice->push_change_event(pv->getFullName(), (Tango::DevULong*)&value, tangoTimestamp, Tango::ATTR_VALID, 1, 0, false);
}

void TangoInterfaceImpl::push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const double& value)
{
    timeval tangoTimestamp = NdsAttributeBase::NDSTimeToTangoTime(timestamp);
    m_pDevice->push_change_event(pv->getFullName(), (Tango::DevDouble*)&value, tangoTimestamp, Tango::ATTR_VALID, 1, 0, false);
}


void TangoInterfaceImpl::push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::vector<std::int8_t> & value)
{
    timeval tangoTimestamp = NdsAttributeBase::NDSTimeToTangoTime(timestamp);
    m_pDevice->push_change_event(pv->getFullName(), (Tango::DevUChar*)value.data(), tangoTimestamp, Tango::ATTR_VALID, value.size(), 0, false);
}

void TangoInterfaceImpl::push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::vector<std::uint8_t> & value)
{
    timeval tangoTimestamp = NdsAttributeBase::NDSTimeToTangoTime(timestamp);
    m_pDevice->push_change_event(pv->getFullName(), (Tango::DevUChar*)value.data(), tangoTimestamp, Tango::ATTR_VALID, value.size(), 0, false);
}

void TangoInterfaceImpl::push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::vector<std::int32_t> & value)
{
    timeval tangoTimestamp = NdsAttributeBase::NDSTimeToTangoTime(timestamp);
    m_pDevice->push_change_event(pv->getFullName(), (Tango::DevLong*)value.data(), tangoTimestamp, Tango::ATTR_VALID, value.size(), 0, false);
}

void TangoInterfaceImpl::push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::vector<double> & value)
{
    timeval tangoTimestamp = NdsAttributeBase::NDSTimeToTangoTime(timestamp);
    m_pDevice->push_change_event(pv->getFullName(), (Tango::DevDouble*)value.data(), tangoTimestamp, Tango::ATTR_VALID, value.size(), 0, false);
}

NdsDevice::NdsDevice(Tango::DeviceClass* pClass, string &parameter):
    TANGO_BASE_CLASS(pClass, parameter.c_str()),
    m_pClass(pClass),
    m_parameter(m_pClass->get_name()),
    m_pDevice(0)
{
    init_device();
}

NdsDevice::~NdsDevice()
{
    delete_device();
}

void NdsDevice::init_device()
{
    TangoFactoryImpl::getInstance().setLastCreatedDevice(this);
    m_pDevice = TangoFactoryImpl::getInstance().createDevice(m_pClass->get_name(), m_parameter);
}

void NdsDevice::delete_device()
{
    TangoFactoryImpl::getInstance().destroyDevice(m_pDevice);
}

void NdsDevice::setRootNode(std::shared_ptr<NodeImpl> pRootNode)
{
    m_pRootNode = pRootNode;
}

Tango::DevState NdsDevice::get_state()
{
    state_t deviceState = m_pRootNode->getGlobalState();

    switch(deviceState)
    {
    case state_t::unknown:
        return Tango::UNKNOWN;
    case state_t::fault:
        return Tango::FAULT;
    case state_t::initializing:
        return Tango::INIT;
    case state_t::off:
        return Tango::OFF;
    case state_t::on:
        return Tango::ON;
    case state_t::running:
        return Tango::RUNNING;
    case state_t::starting:
        return Tango::ON;
    case state_t::stopping:
        return Tango::ON;
    case state_t::switchingOff:
        return Tango::OFF;
    case state_t::MAX_STATE_NUM:
        throw std::logic_error("The state MAX_STATE_NUM is used internally to count the number of available states and should not be used.");
    }
}

/*
 *
 * Base Tango Attribute class
 *
 *
 ****************************/

NdsAttributeBase::NdsAttributeBase(std::shared_ptr<PVBaseImpl> pv): m_pPV(pv)
{
}

/*
 * Copies the attribute properties from the NDS PV to the Tango attribute
 *
 ************************************************************************/
void NdsAttributeBase::setAttributeProperties(Tango::Attr& attr)
{
    Tango::UserDefaultAttrProp properties;
    properties.set_description(m_pPV->getDescription().c_str());
    properties.set_label(m_pPV->getComponentName().c_str());
    attr.set_default_properties(properties);
    if(m_pPV->getScanType() == scanType_t::periodic)
    {
        attr.set_polling_period(m_pPV->getScanPeriodSeconds() * 1000);
    }
    if(m_pPV->getScanType() == scanType_t::interrupt)
    {
        attr.set_change_event(true, false);
    }

}

/*
 * Tango scalar attribute
 *
 ************************/

template<typename ndsType_t, typename tangoType_t>
NdsAttributeScalar<ndsType_t, tangoType_t>::NdsAttributeScalar(const std::string& name, std::shared_ptr<PVBaseImpl> pPV, Tango::CmdArgType dataType, Tango::AttrWriteType writeType) :
    NdsAttributeBase(pPV), Tango::Attr(name.c_str(), dataType, writeType)
{
    setAttributeProperties(*this);
}

template<typename ndsType_t, typename tangoType_t>
void NdsAttributeScalar<ndsType_t, tangoType_t>::read(Tango::DeviceImpl* /* dev */,Tango::Attribute &att)
{
    timespec timestamp;
    m_pPV->read(&timestamp, (ndsType_t*) &m_value);
    timeval tangoTime(NDSTimeToTangoTime(timestamp));
    att.set_value_date_quality(&m_value, tangoTime, Tango::ATTR_VALID, 1, 0, false);
}

template<typename ndsType_t, typename tangoType_t>
void NdsAttributeScalar<ndsType_t, tangoType_t>::write(Tango::DeviceImpl* /* dev */,Tango::WAttribute &att)
{
    try
    {
        att.get_write_value(m_value);
        Tango::TimeVal timestamp(att.get_date());
        timespec ndsTime;
        ndsTime.tv_sec = timestamp.tv_sec;
        ndsTime.tv_nsec = timestamp.tv_nsec;
        m_pPV->write(ndsTime, (ndsType_t)m_value);
    }
    catch(const NdsError& error)
    {
        Tango::Except::throw_exception("OPERATION_NOT_ALLOWED", error.what(), __PRETTY_FUNCTION__, Tango::ERR);
    }
}

template<typename ndsType_t, typename tangoType_t>
bool NdsAttributeScalar<ndsType_t, tangoType_t>::is_allowed(Tango::DeviceImpl* /* dev */,Tango::AttReqType /* ty */)
{
    return true;
}


/*
 * Tango spectrum attribute
 *
 **************************/

template<typename ndsType_t, typename tangoType_t>
NdsAttributeSpectrum<ndsType_t, tangoType_t>::NdsAttributeSpectrum(const string &name, std::shared_ptr<PVBaseImpl> pPV, Tango::CmdArgType dataType, Tango::AttrWriteType writeType, size_t maxLength):
    NdsAttributeBase(pPV), Tango::SpectrumAttr(name.c_str(), dataType, writeType, maxLength)
{
    setAttributeProperties(*this);
}

template<typename ndsType_t, typename tangoType_t>
void NdsAttributeSpectrum<ndsType_t, tangoType_t>::read(Tango::DeviceImpl* /* dev */,Tango::Attribute &att)
{
    timespec timestamp;
    m_pPV->read(&timestamp, (ndsType_t*) &m_value);
    timeval tangoTime(NDSTimeToTangoTime(timestamp));
    att.set_value_date_quality(m_value.data(), tangoTime, Tango::ATTR_VALID, 1, 0, false);
}

template<typename ndsType_t, typename tangoType_t>
void NdsAttributeSpectrum<ndsType_t, tangoType_t>::write(Tango::DeviceImpl* /* dev */,Tango::WAttribute &att)
{
    // TODO
}

template<typename ndsType_t, typename tangoType_t>
bool NdsAttributeSpectrum<ndsType_t, tangoType_t>::is_allowed(Tango::DeviceImpl* /* dev */,Tango::AttReqType /* ty */)
{
    return true;
}

timeval NdsAttributeBase::NDSTimeToTangoTime(const timespec& time)
{
    timeval tangoTime;
    tangoTime.tv_sec = time.tv_sec;
    tangoTime.tv_usec = time.tv_nsec / 1000;
    return tangoTime;
}

timespec NdsAttributeBase::TangoTimeToNDSTime(const timeval& time)
{
    timespec ndsTime;
    ndsTime.tv_sec = time.tv_sec;
    ndsTime.tv_nsec = time.tv_usec * 1000;
    return ndsTime;
}

NdsDeviceClass::NdsDeviceClass(std::string& name, allocateDriver_t allocateDriverFunction, deallocateDriver_t deallocateDriverFunction):
    Tango::DeviceClass(name),
    m_allocateDriverFunction(allocateDriverFunction), m_deallocateDriverFunction(deallocateDriverFunction)
{

}

void NdsDeviceClass::command_factory()
{
    /*
    command_list.push_back(new OnClass());
    command_list.push_back(new OffClass());
    command_list.push_back(new StartClass());
    command_list.push_back(new StopClass());
    command_list.push_back(new ResetClass());

    */
}

void NdsDeviceClass::attribute_factory(vector<Tango::Attr *> &attributes)
{
}

void NdsDeviceClass::device_factory(const Tango::DevVarStringArray* dev_list)
{
    for (unsigned long i=0 ; i  < dev_list->length() ; i++)
    {
        std::cout << "Device name : " << (*dev_list)[i].in() << endl;
        std::string parameter((*dev_list)[i]);
        NdsDevice* pNewDevice(new NdsDevice(this, parameter));
        device_list.push_back(pNewDevice);

        //	Check before if database used.
        if ((Tango::Util::_UseDb == true) && (Tango::Util::_FileDb == false))
            export_device(pNewDevice);
        else
            export_device(pNewDevice, pNewDevice->get_name().c_str());

    }

}

}

#endif
