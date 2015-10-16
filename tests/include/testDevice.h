#ifndef TESTDEVICE_H
#define TESTDEVICE_H

#include <nds3/nds.h>

class TestDevice
{
public:
    TestDevice(nds::Factory& factory, const std::string& parameter);
    ~TestDevice();

    /*
     * Allocation/deallocation
     *
     *******************************************************/
    static void* allocateDevice(nds::Factory& factory, const std::string& device, const nds::namedParameters_t& parameters);
    static void deallocateDevice(void* device);

    /*
     * For test purposes we make it possible to retrieve running instances of
     *  the device
     */
    static TestDevice* getInstance(const std::string& deviceName);

    nds::PVVariableIn<std::int32_t> m_variableIn0;
    nds::PVVariableIn<std::vector<std::int32_t> > m_variableIn1;

    nds::PVVariableOut<std::int32_t> m_numberAcquisitions;

    nds::DataAcquisition<std::vector<std::int32_t> > m_dataAcquisition;


private:
    void switchOn();
    void switchOff();
    void start();
    void stop();
    void recover();
    bool allowChange(const nds::state_t, const nds::state_t, const nds::state_t);

    void acquire(size_t numAcquisition, size_t numSamples);

    std::thread m_acquisitionThread;

    std::string m_name;


};

#endif // TESTDEVICE_H
