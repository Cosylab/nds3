#ifdef NDS3_TANGO

#ifndef NDSTANGOINTERFACEIMPL_H
#define NDSTANGOINTERFACEIMPL_H

#include <string>
#include <vector>
#include <set>
#include "interfaceBaseImpl.h"
#include <tango.h>

namespace nds
{

class PVBaseImpl;
class NdsDeviceClass;
class NdsDevice;

/**
 * @internal
 * @brief Tango interface class: used by Port objects to communicate
 *        with Tango
 */
class TangoInterfaceImpl: public InterfaceBaseImpl
{
public:
    TangoInterfaceImpl(const std::string& portName, NdsDevice* pDevice);

    virtual void registerPV(std::shared_ptr<PVBaseImpl> pv);
    virtual void deregisterPV(std::shared_ptr<PVBaseImpl> pv);

    virtual void registrationTerminated();

    virtual void push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::int8_t& value);
    virtual void push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::uint8_t& value);
    virtual void push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::int32_t& value);
    virtual void push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::uint32_t& value);
    virtual void push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const double& value);
    virtual void push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::vector<std::int8_t> & value);
    virtual void push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::vector<std::uint8_t> & value);
    virtual void push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::vector<std::int32_t> & value);
    virtual void push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::vector<double> & value);

private:
    NdsDevice* m_pDevice;

};


/**
 * @internal
 * @brief Describes a Tango device and takes care of allocating it.
 */
class NdsDeviceClass: public Tango::DeviceClass
{
public:
    NdsDeviceClass(std::string& name, allocateDriver_t allocateDriverFunction, deallocateDriver_t deallocateDriverFunction);


    virtual void command_factory();
    virtual void attribute_factory(vector<Tango::Attr*>& attributes);
    virtual void device_factory(const Tango::DevVarStringArray* dev_list);


protected:
    allocateDriver_t m_allocateDriverFunction;
    deallocateDriver_t m_deallocateDriverFunction;
};


/**
 * @internal
 * @brief Overwrites the Tango Device class and implements the necessary virtual
 *        functions.
 *
 * Used by TangoInterfaceImpl to communicate with Tango.
 */
class NdsDevice: public TANGO_BASE_CLASS
{
public:
    NdsDevice(Tango::DeviceClass* pClass, string &name);
    ~NdsDevice();

    virtual void init_device();

    virtual void delete_device();

    void setRootNode(std::shared_ptr<NodeImpl> pRootNode);

    virtual Tango::DevState get_state();

protected:
    Tango::DeviceClass* m_pClass;
    std::string m_parameter;
    std::shared_ptr<NodeImpl> m_pRootNode;

    void* m_pDevice;
};


/**
 * @internal
 * @brief Base class for NdsAttributeScalar and NdsAttributeSpectrum.
 *
 */
class NdsAttributeBase
{
public:

    /**
     * @brief Convert the NDS timestamp to a Tango timestamp.
     * @param time  timestamp in NDS format
     * @return      timestamp in Tango format
     */
    static timeval NDSTimeToTangoTime(const timespec& time);

    /**
     * @brief Convert the Tango timestamp to NDS timestamp.
     * @param time  timestamp in Tango format
     * @return      timestamp in NDS format
     */
    static timespec TangoTimeToNDSTime(const timeval& time);

protected:
    NdsAttributeBase(std::shared_ptr<PVBaseImpl> pv);

    void setAttributeProperties(Tango::Attr& attr);

    std::shared_ptr<PVBaseImpl> m_pPV;

};


/**
 * @brief NDS to Tango attribute proxy for scalar values
 *
 */
template <typename ndsType_t, typename tangoType_t>
class NdsAttributeScalar: public NdsAttributeBase, public Tango::Attr
{
public:
    NdsAttributeScalar(const std::string& name, std::shared_ptr<PVBaseImpl> pPV, Tango::CmdArgType dataType, Tango::AttrWriteType writeType);

    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty);

private:
    tangoType_t m_value;
};


/**
 * @brief NDS to Tango attribute proxy for spectrum values
 *
 */
template <typename ndsType_t, typename tangoType_t>
class NdsAttributeSpectrum: public NdsAttributeBase, public Tango::SpectrumAttr
{
public:
    NdsAttributeSpectrum(const std::string& name, std::shared_ptr<PVBaseImpl> pPV, Tango::CmdArgType dataType, Tango::AttrWriteType writeType, size_t maxLength);

    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty);

private:
    std::vector<tangoType_t> m_value;
};

/**
 * @brief Commands supported by Tango
 *
 */

/*
class OnClass : public Tango::Command
{
public:
    OnClass();
    ~OnClass();
    virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
    virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any);
};

class OffClass : public Tango::Command
{
public:
    OffClass();
    ~OffClass();
    virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
    virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any);
};

class StartClass : public Tango::Command
{
public:
    StartClass();
    ~StartClass();
    virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
    virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any);
};

class StopClass : public Tango::Command
{
public:
    StopClass();
    ~StopClass();
    virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
    virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any);
};

class ResetClass : public Tango::Command
{
public:
    ResetClass();
    ~ResetClass();
    virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
    virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any);
};


*/

}

#endif // NDSTANGOINTERFACEIMPL_H

#endif // NDS3_TANGO

