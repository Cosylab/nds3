#ifdef NDS3_TANGO

#ifndef NDSTANGOINTERFACEIMPL_H
#define NDSTANGOINTERFACEIMPL_H

#include <string>
#include <vector>
#include <set>
#include "ndsinterfacebaseimpl.h"
#include <tango.h>

namespace nds
{

class PVBaseImpl;
class NdsDeviceClass;
class NdsDevice;

/**
 * @internal
 * @brief The AsynInterface class. Allocated by AsynPort
 *        to communicate with the AsynDriver
 */
class TangoInterfaceImpl: public InterfaceBaseImpl
{
public:
    typedef std::list<std::shared_ptr<PVBaseImpl> > PVList_t;

    TangoInterfaceImpl(const std::string& portName, NdsDevice* pDevice);

    virtual void registerPV(std::shared_ptr<PVBaseImpl> pv);

    virtual void registrationTerminated();

    virtual void push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::int32_t& value);
    virtual void push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const double& value);
    virtual void push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const std::vector<std::int32_t> & value);

private:
    PVList_t m_registeredPVs;

    NdsDevice* m_pDevice;

    std::shared_ptr<PVBaseImpl> m_lastRegisteredPV;
};


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


class NdsDevice: public TANGO_BASE_CLASS
{
public:
    NdsDevice(Tango::DeviceClass* pClass, string &name);

    void init_device();

    void setRootNode(std::shared_ptr<NodeImpl> pRootNode);

    virtual Tango::DevState get_state();

protected:
    Tango::DeviceClass* m_pClass;
    std::string m_parameter;
    std::shared_ptr<NodeImpl> m_pRootNode;
};


class NdsAttributeBase
{
protected:
    NdsAttributeBase(std::shared_ptr<PVBaseImpl> pv);

    void setAttributeProperties(Tango::Attr& attr);

    std::shared_ptr<PVBaseImpl> m_pPV;

    static timeval NDSTimeToTangoTime(const timespec& time);
    static timespec TangoTimeToNDSTime(const timeval& time);
};

/**
 * @brief NDS<->Tango attribute proxy
 *
 */
class NdsAttributeScalar: public NdsAttributeBase, public Tango::Attr
{
public:
    NdsAttributeScalar(const std::string& name, std::shared_ptr<PVBaseImpl> pPV, Tango::CmdArgType dataType, Tango::AttrWriteType writeType);

    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty);

private:
    template<typename NdsType_t, typename TangoType_t>
    void readValue(Tango::Attribute &att);

    template<typename NdsType_t, typename TangoType_t>
    void writeValue(Tango::Attribute &att, const TangoType_t& value);
};


class NdsAttributeSpectrum: public NdsAttributeBase, public Tango::SpectrumAttr
{
public:
    NdsAttributeSpectrum(const std::string& name, std::shared_ptr<PVBaseImpl> pPV, Tango::CmdArgType dataType, Tango::AttrWriteType writeType, size_t maxLength);

    virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att);
    virtual void write(Tango::DeviceImpl *dev,Tango::WAttribute &att);
    virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty);

private:
    template<typename NdsType_t, typename TangoType_t>
    void readValue(Tango::Attribute &att);

    template<typename NdsType_t, typename TangoType_t>
    void writeValue(Tango::Attribute &att, const TangoType_t& value);
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

