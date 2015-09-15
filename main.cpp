#include <iostream>
#include <memory>
#include <vector>

#include <math.h>
#include <thread>
#include <cstdint>
#include <functional>

#include <unistd.h>

#include "library/include/nds3/nds3.h"
#include "iocsh.h"

//using namespace std;


/**
 * @brief Sadly this class made it into this file.
 *
 * Used as example of a business logic class which implements the reads and writes and one
 * day may react to changes in a state machine class.
 *
 * This oscilloscope has 2 channels:
 * - one channel provides a sinusoidal wave as a floating point scalar number that varies over time
 * - another channel provides a square wave, but this time pushed as an array of int32 values
 *
 */
class Oscilloscope
{
public:

    nds::Node root;
    Oscilloscope()
    {
        // We create a device. We could use directly a port here, but for fun this device will have
        //  more than one Asyn port
        ///////////////////////////////////////////////////////////////////////////////////////////
        root = nds::Node("Dev");

        // We add the first port that will be registered as "Dev-MyPort".
        //////////////////////////////////////////////////////////////////////////
        nds::Port port = root.addChild(nds::Port("MyPort"));

        // I have two channels: one connected to a sinusoidal wave generator, one to a square w. gen
        channel0 = port.addChild(nds::DataAcquisition<double>("Ch0",
                                          nds::recordType_t::ai,
                                          1,
                                          std::bind(&Oscilloscope::doNothing, this),     // Transition from off to on. Nothing to do
                                          std::bind(&Oscilloscope::doNothing, this),     // Transition from on to off. Nothing to do
                                          std::bind(&Oscilloscope::startChannel0, this), // Transition from on to running. Launch the thread
                                          std::bind(&Oscilloscope::stopChannel0, this),  // Transition from running to on. Stop the thread
                                          std::bind(&Oscilloscope::doNothing, this),     // Try to recover from errors. Now does nothing
                                          std::bind(&Oscilloscope::canChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
                                          ));

        channel1 = port.addChild(nds::DataAcquisition<std::vector<std::int32_t> >("Ch1",
                                          nds::recordType_t::waveformIn,
                                          100,
                                          std::bind(&Oscilloscope::doNothing, this),
                                          std::bind(&Oscilloscope::doNothing, this),
                                          std::bind(&Oscilloscope::startChannel1, this),
                                          std::bind(&Oscilloscope::stopChannel1, this),
                                          std::bind(&Oscilloscope::doNothing, this),
                                          std::bind(&Oscilloscope::canChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
                                          ));

        // All the structure has been setup. Register the ports and the PVs
        ///////////////////////////////////////////////////////////////////
        root.initialize();

    }

    // Used for transitions that don't require any action
    /////////////////////////////////////////////////////
    void doNothing()
    {}

    // Thread that produce a sinusoidal wave and push it. Takes channel 0
    //  as a parameter.
    /////////////////////////////////////////////////////////////////////
    void sinThread(nds::DataAcquisition<double> dataAcquisition)
    {
        double hz = dataAcquisition.getFrequencyHz();

        const std::uint64_t secMultiplier(1000000000);
        const timespec startTime = dataAcquisition.getStartTimestamp();
        const std::uint64_t startTimeNsec(startTime.tv_sec * secMultiplier + startTime.tv_nsec);

        const std::uint64_t hzMultiplier = 1000000;
        const std::uint64_t periodNsec = (secMultiplier * hzMultiplier) / (std::uint64_t)(hz * (double)hzMultiplier);

        while(m_bSinContinue)
        {
            timespec currentTime = dataAcquisition.getTimestamp();
            std::uint64_t currentTimeNsec(currentTime.tv_sec * secMultiplier + currentTime.tv_nsec);
            std::uint64_t difference(currentTimeNsec - startTimeNsec);

            double angle = M_PI * 2 * (double)(difference % periodNsec) / (double)periodNsec;

            dataAcquisition.pushData(currentTime, sin(angle) * double(100));

            ::sleep(0); // Give other threads a chance
        }
    }

    // Thread that produce a square wave and push it. Takes channel 1
    //  as a parameter.
    /////////////////////////////////////////////////////////////////////
    void sqrThread(nds::DataAcquisition<std::vector<std::int32_t> > dataAcquisition)
    {
        double hz = dataAcquisition.getFrequencyHz();

        const std::uint64_t secMultiplier(1000000000);
        const timespec startTime = dataAcquisition.getStartTimestamp();
        const std::uint64_t startTimeNsec(startTime.tv_sec * secMultiplier + startTime.tv_nsec);

        const std::uint64_t hzMultiplier = 1000000;
        const std::uint64_t periodNsec = (secMultiplier * hzMultiplier) / (std::uint64_t)(hz * (double)hzMultiplier);

        // Prepare the wave
        ///////////////////
        std::vector<std::int32_t> squareWave;
        for(size_t fillSquare(0); fillSquare != dataAcquisition.getMaxElements(); ++fillSquare)
        {
            squareWave.push_back(fillSquare < dataAcquisition.getMaxElements() / 2 ? 0 : 100);
        }

        // Push the wave until we have to stop
        //////////////////////////////////////
        while(m_bSqrContinue)
        {
            // Here we are not really caring about the frequency, we just push this wave
            // as fast as we can
            dataAcquisition.pushData(dataAcquisition.getTimestamp(), squareWave);

            ::sleep(0); // Give other threads a chance
        }
    }

    void startChannel0()
    {
        m_bSinContinue = true;
        m_sinThread = std::thread(std::bind(&Oscilloscope::sinThread, this, channel0));
    }

    void stopChannel0()
    {
        m_bSinContinue = false;
        m_sinThread.join();
    }

    void startChannel1()
    {
        m_bSqrContinue = true;
        m_sqrThread = std::thread(std::bind(&Oscilloscope::sqrThread, this, channel1));
    }

    void stopChannel1()
    {
        m_bSqrContinue = false;
        m_sqrThread.join();
    }

    bool canChange(const nds::state_t, const nds::state_t, const nds::state_t)
    {
        return true;
    }

private:
    nds::DataAcquisition<double> channel0;
    nds::DataAcquisition<std::vector<std::int32_t> > channel1;

    std::thread m_sinThread;
    std::thread m_sqrThread;

    bool m_bSinContinue;
    bool m_bSqrContinue;

};

void* allocateDevice(const std::string& parameter)
{
    return new Oscilloscope();
}

void deallocateDevice(void* device)
{
    delete (Oscilloscope*)device;
}

int main()
{
    nds::Factory factory(nds::controlSystem_t::epics);
    factory.registerDriver("Oscilloscope", std::bind(&allocateDevice, std::placeholders::_1), std::bind(&deallocateDevice, std::placeholders::_1));

    iocshCmd("dbLoadDatabase ndsTest.dbd /home/codac-dev/Documents/m-nds-test/target/main/epics/dbd");
    iocshCmd("createNdsDevice Oscilloscope");

    factory.run();
}

