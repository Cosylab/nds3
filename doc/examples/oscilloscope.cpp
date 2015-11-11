#include "../include/oscilloscope.h"
#include <functional>
#include <math.h>
#include <unistd.h>
#include <iostream>

void Oscilloscope::callback(const timespec& time, const std::vector<double>& data)
{
    std::cout << data.size();
}

////////////////////////////////////////////////////////////////////////////////
//
// Constructor for our Oscilloscope device.
// It declares all the nodes and PVs in the device, then register the root node
//  (which in turn register all its children).
//
////////////////////////////////////////////////////////////////////////////////
Oscilloscope::Oscilloscope(nds::Factory &factory, const std::string &deviceName, const nds::namedParameters_t &parameters)
{
    // Here we declare the root node.
    // It is a good practice to name it with the device name.
    //
    // Also, for simplicity we declare it as a "Port": this means that
    //  the root node will be responsible for the communication with
    //  the underlying control system.
    //
    // It is possible to have the root node as a simple Node and promote one or
    //  more of its children to "Port": each port will interface with a different
    //  control system thread.
    ///////////////////////////////////////////////////////////////////////////////
    nds::Port rootNode(deviceName);

    nds::PVBaseOut nodeOut = rootNode.addChild(nds::PVDelegateOut<std::vector<double> >("Output", std::bind(&Oscilloscope::callback, this, std::placeholders::_1, std::placeholders::_2)));
    nodeOut.setMaxElements(100);

    // We add a child to the root node: a variable that the user can set with the
    //  max. amplitude of the sinusoidal wave before the data acquisition starts.
    //
    // We save the variable in a class' member so later we can access
    //  it to retrieve its value
    ///////////////////////////////////////////////////////////////////////////////
    m_sinWaveAmplitude = rootNode.addChild(nds::PVVariableOut<std::int32_t>("maxSinAmplitude"));

    // Another child is used to declare the maximum square wave amplitude
    ////////////////////////////////////////////////////////////////////////////////
    m_squareWaveAmplitude = rootNode.addChild(nds::PVVariableOut<std::int32_t>("maxSquareAmplitude"));

    // Add an acquisition node: it supplies an input PV on which we can push the
    //  acquired data and a state machine that allows to start and stop the
    //  acquisition.
    // This node is for the sinusoidal wave....
    ////////////////////////////////////////////////////////////////////////////////
    m_acquisitionSinWave = rootNode.addChild(nds::DataAcquisition<std::vector<std::int32_t> >(
                                                 "SinWave",
                                                 100,
                                                 std::bind(&Oscilloscope::switchOnSinWave, this),
                                                 std::bind(&Oscilloscope::switchOffSinWave, this),
                                                 std::bind(&Oscilloscope::startSinWave, this),
                                                 std::bind(&Oscilloscope::stopSinWave, this),
                                                 std::bind(&Oscilloscope::recoverSinWave, this),
                                                 std::bind(&Oscilloscope::allowChange,
                                                           this,
                                                           std::placeholders::_1,
                                                           std::placeholders::_2,
                                                           std::placeholders::_3)
                                                 ));

    // ...and this node is for the square wave
    ////////////////////////////////////////////////////////////////////////////////
    m_acquisitionSquareWave = rootNode.addChild(nds::DataAcquisition<std::vector<std::int32_t> >(
                                                    "SquareWave",
                                                    100,
                                                    std::bind(&Oscilloscope::switchOnSquareWave, this),
                                                    std::bind(&Oscilloscope::switchOffSquareWave, this),
                                                    std::bind(&Oscilloscope::startSquareWave, this),
                                                    std::bind(&Oscilloscope::stopSquareWave, this),
                                                    std::bind(&Oscilloscope::recoverSquareWave, this),
                                                    std::bind(&Oscilloscope::allowChange,
                                                              this,
                                                              std::placeholders::_1,
                                                              std::placeholders::_2,
                                                              std::placeholders::_3)
                                                    ));

    // We have declared all the nodes and PVs in our device: now we register them
    //  with the control system that called this constructor.
    ////////////////////////////////////////////////////////////////////////////////
    rootNode.initialize(this, factory);
}


////////////////////////////////////////////////////////////////////////////////
//
// Called when the "SinWave" PV has to be switched on.
// Here we do nothing (in our case no special operations are needed to switch
//  on the node).
//
////////////////////////////////////////////////////////////////////////////////
void Oscilloscope::switchOnSinWave()
{
}


////////////////////////////////////////////////////////////////////////////////
//
// Called when the "SinWave" PV has to be switched off.
// Here we do nothing (in our case no special operations are needed to switch
//  off the node).
//
////////////////////////////////////////////////////////////////////////////////
void Oscilloscope::switchOffSinWave()
{
}


////////////////////////////////////////////////////////////////////////////////
//
// Called when the "SinWave" PV has to start acquiring.
// We start the data acquisition thread for the sinusoidal wave
//
////////////////////////////////////////////////////////////////////////////////
void Oscilloscope::startSinWave()
{
    m_bStopAcquisitionSinWave = false; //< We will set to true to stop the acquisition thread

    // Start the acquisition thread.
    // We don't need to check if the thread was already started because the state
    //  machine guarantees that the start handler is called only while the state
    //  is ON.
    ////////////////////////////////////////////////////////////////////////////////
    m_acquisitionThreadSinWave = std::thread(std::bind(&Oscilloscope::acquireSinusoidalWave, this));
}


////////////////////////////////////////////////////////////////////////////////
//
// Stop the sinusoidan wave acquisition thread
//
////////////////////////////////////////////////////////////////////////////////
void Oscilloscope::stopSinWave()
{
    m_bStopAcquisitionSinWave = true;
    m_acquisitionThreadSinWave.join();
}


////////////////////////////////////////////////////////////////////////////////
//
// A failure during a state transition will cause the state machine to switch
//  to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force
//  the state machine to stay on the failure state.
//
////////////////////////////////////////////////////////////////////////////////
void Oscilloscope::recoverSinWave()
{
    throw nds::StateMachineRollBack("Cannot recover");
}


////////////////////////////////////////////////////////////////////////////////
//
// Called when the "SquareWave" PV has to be switched on.
// Here we do nothing (in our case no special operations are needed to switch
//  on the node).
//
////////////////////////////////////////////////////////////////////////////////
void Oscilloscope::switchOnSquareWave()
{
}


////////////////////////////////////////////////////////////////////////////////
//
// Called when the "SquareWave" PV has to be switched off.
// Here we do nothing (in our case no special operations are needed to switch
//  off the node).
//
////////////////////////////////////////////////////////////////////////////////
void Oscilloscope::switchOffSquareWave()
{

}


////////////////////////////////////////////////////////////////////////////////
//
// Called when the "SquareWave" PV has to start acquiring.
// We start the data acquisition thread for the square wave
//
////////////////////////////////////////////////////////////////////////////////
void Oscilloscope::startSquareWave()
{
    m_bStopAcquisitionSquareWave = false; //< We will set to true to stop the acquisition thread

    // Start the acquisition thread.
    // We don't need to check if the thread was already started because the state
    //  machine guarantees that the start handler is called only while the state
    //  is ON.
    ////////////////////////////////////////////////////////////////////////////////
    m_acquisitionThreadSquareWave = std::thread(std::bind(&Oscilloscope::acquireSquareWave, this));
}


////////////////////////////////////////////////////////////////////////////////
//
// Stop the square wave acquisition thread
//
////////////////////////////////////////////////////////////////////////////////
void Oscilloscope::stopSquareWave()
{
    m_bStopAcquisitionSquareWave = true;
    m_acquisitionThreadSquareWave.join();
}


////////////////////////////////////////////////////////////////////////////////
//
// A failure during a state transition will cause the state machine to switch
//  to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force
//  the state machine to stay on the failure state.
//
////////////////////////////////////////////////////////////////////////////////
void Oscilloscope::recoverSquareWave()
{
    throw nds::StateMachineRollBack("Cannot recover");
}


////////////////////////////////////////////////////////////////////////////////
//
// We always allow the state machine to switch state.
// Before calling this function the state machine has already verified that the
//  requested state transition is legal.
//
////////////////////////////////////////////////////////////////////////////////
bool Oscilloscope::allowChange(const nds::state_t, const nds::state_t, const nds::state_t)
{
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//
// Acquisition function for the sinusoidal wave. It is launched in a separate
//  thread by startSinWave().
//
////////////////////////////////////////////////////////////////////////////////
void Oscilloscope::acquireSinusoidalWave()
{
    // Let's allocate a vector that will contain the data that we will push to the
    //  control system
    ////////////////////////////////////////////////////////////////////////////////
    std::vector<std::int32_t> outputData(m_acquisitionSinWave.getMaxElements());

    // A counter for the angle in the sin() operation
    ////////////////////////////////////////////////////////////////////////////////
    std::int64_t angle(0);

    // Run until the state machine stops us
    ////////////////////////////////////////////////////////////////////////////////
    while(!m_bStopAcquisitionSinWave)
    {
        // Fill the vector with a sin wave
        ////////////////////////////////////////////////////////////////////////////////
        size_t maxAmplitude = m_sinWaveAmplitude.getValue(); // PVVariables are thread safe
        for(size_t scanVector(0); scanVector != outputData.size(); ++scanVector)
        {
            outputData[scanVector] = (double)maxAmplitude * sin((double)(angle++) / 10.0f);
        }

        // Push the vector to the control system
        ////////////////////////////////////////////////////////////////////////////////
        m_acquisitionSinWave.push(m_acquisitionSinWave.getTimestamp(), outputData);

        // Rest for a while
        ////////////////////////////////////////////////////////////////////////////////
        ::usleep(100000);
    }
}


////////////////////////////////////////////////////////////////////////////////
//
// Acquisition function for the square wave. It is launched in a separate
//  thread by startSquareWave().
//
////////////////////////////////////////////////////////////////////////////////
void Oscilloscope::acquireSquareWave()
{
    // Let's allocate a vector that will contain the data that we will push to the
    //  control system
    ////////////////////////////////////////////////////////////////////////////////
    std::vector<std::int32_t> outputData(m_acquisitionSquareWave.getMaxElements());

    // A counter for the angle of the square wave
    ////////////////////////////////////////////////////////////////////////////////
    std::int64_t angle(0);

    // Run until the state machine stops us
    ////////////////////////////////////////////////////////////////////////////////
    while(!m_bStopAcquisitionSquareWave)
    {
        // Fill the vector with a square wave
        ////////////////////////////////////////////////////////////////////////////////
        size_t maxAmplitude = m_squareWaveAmplitude.getValue();
        for(size_t scanVector(0); scanVector != outputData.size(); ++scanVector)
        {
            outputData[scanVector] = (angle & 0xff < 128) ? maxAmplitude : - maxAmplitude;
        }

        // Push the vector to the control system
        ////////////////////////////////////////////////////////////////////////////////
        m_acquisitionSquareWave.push(m_acquisitionSquareWave.getTimestamp(), outputData);

        // Rest for a while
        ////////////////////////////////////////////////////////////////////////////////
        ::usleep(100000);
    }
}

NDS_DEFINE_DRIVER("Oscilloscope", Oscilloscope);
