#include <iostream>
#include <memory>
#include <vector>

#include <math.h>
#include <thread>
#include <cstdint>
#include <functional>

#include "library/implementation/ndsfactoryimpl.h"
#include "iocsh.h"

#include "library/include/nds3/nds3.h"

//using namespace std;


/**
 * @brief Sadly this class made it into this file.
 *
 * Used as example of a business logic class which implements the reads and writes and one
 * day may react to changes in a state machine class.
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
                                          nds::ai,
                                          1,
                                          std::bind(&Oscilloscope::doNothing, this),     // Transition from off to on. Nothing to do
                                          std::bind(&Oscilloscope::doNothing, this),     // Transition from on to off. Nothing to do
                                          std::bind(&Oscilloscope::startChannel0, this), // Transition from on to running. Launch the thread
                                          std::bind(&Oscilloscope::stopChannel0, this),  // Transition from running to on. Stop the thread
                                          std::bind(&Oscilloscope::doNothing, this),     // Try to recover from errors. Now does nothing
                                          std::bind(&Oscilloscope::canChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
                                          ));

        channel1 = port.addChild(nds::DataAcquisition<double>("Ch1",
                                          nds::ai,
                                          1,
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

    void sqrThread(nds::DataAcquisition<double> dataAcquisition)
    {
        double hz = dataAcquisition.getFrequencyHz();

        const std::uint64_t secMultiplier(1000000000);
        const timespec startTime = dataAcquisition.getStartTimestamp();
        const std::uint64_t startTimeNsec(startTime.tv_sec * secMultiplier + startTime.tv_nsec);

        const std::uint64_t hzMultiplier = 1000000;
        const std::uint64_t periodNsec = (secMultiplier * hzMultiplier) / (std::uint64_t)(hz * (double)hzMultiplier);

        while(m_bSqrContinue)
        {
            timespec currentTime = dataAcquisition.getTimestamp();
            std::uint64_t currentTimeNsec(currentTime.tv_sec * secMultiplier + currentTime.tv_nsec);
            std::uint64_t difference(currentTimeNsec - startTimeNsec);

            if(difference % periodNsec > periodNsec / 2)
            {
                dataAcquisition.pushData(currentTime, 100);
            }
            else
            {
                dataAcquisition.pushData(currentTime, 0);
            }

            ::sleep(0);
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
    nds::DataAcquisition<double> channel1;

    std::thread m_sinThread;
    std::thread m_sqrThread;

    bool m_bSinContinue;
    bool m_bSqrContinue;

};


int main()
{
    nds::FactoryImpl::getInstance().registrationCommand("myIoc");

    Oscilloscope oscilloscope;


    iocshCmd("dbLoadDatabase ndsTest.dbd /home/codac-dev/Documents/m-nds-test/target/main/epics/dbd");
    iocshCmd("myIoc pdbbase");
    iocshCmd("dbLoadRecords auto_generated_Dev-MyPort.db");
    iocshCmd("iocInit");

    iocsh(0);
}

