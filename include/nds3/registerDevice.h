#ifndef NDSREGISTER_DEVICE_H
#define NDSREGISTER_DEVICE_H

namespace nds
{

/**
 * @brief This is a class intended to be used as a static class for automatic registering of device supports
 */
template <class T>
class NDS3_API RegisterDevice
{
private:
    const std::string m_driverName;

public:
    RegisterDevice(const char *driverName) : m_driverName(std::string(driverName)) {
        nds::Factory::registerDriver(m_driverName, RegisterDevice<T>::allocateDevice, RegisterDevice<T>::deallocateDevice);
    }

    static void *allocateDevice(nds::Factory& factory, const std::string& device, const nds::namedParameters_t& parameters){
        return new T(factory, device, parameters);
    }

    static void deallocateDevice(void *device) {
        delete (T *)device;
    }

    const char *getDriverName() {
        return m_driverName;
    }

protected:
    RegisterDevice();

};
} /* namespace nds */


#endif /* NDSREGISTER_DEVICE_H */
