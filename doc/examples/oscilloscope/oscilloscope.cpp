#include <functional>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implement a fictional Oscilloscope device.
 *
 * @warning For the purpose of the example the declaration of the class is in the same
 *          file as its implementation. In a real life situation the declaration should be
 *          in a separate header file.
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few manadatory parameters and should register the root node via Node::initialize().
 */
class Oscilloscope
{
public:
    /**
     * @brief Constructor.
     *
     * @param factory    the control system factory that requested the creation of the device
     * @param device     the name given to the device
     * @param parameters optional parameters passed to the device
     */
    Oscilloscope(nds::Factory& factory, const std::string& device, const nds::namedParameters_t& parameters);

private:
    /**
     * @brief Amplitude of the sinusoidal wave
     */
    nds::PVVariableOut<std::int32_t> m_sinWaveAmplitude;

    /**
     * @brief Amplitude of the square wave
     */
    nds::PVVariableOut<std::int32_t> m_squareWaveAmplitude;

    /**
     * @brief Acquisition node for the sinusoidal wave
     */
    nds::DataAcquisition<std::vector<std::int32_t> > m_acquisitionSinWave;

    /**
     * @brief Acquisition node for the square wave
     */
    nds::DataAcquisition<std::vector<std::int32_t> > m_acquisitionSquareWave;

    void switchOnSinWave();  ///< Called to switch on the sinusoidal wave acquisition node.
    void switchOffSinWave(); ///< Called to switch off the sinusoidal wave acquisition node.
    void startSinWave();     ///< Called to start the data acquisition on the sinusoidal wave node.
    void stopSinWave();      ///< Called to stop the data acquisition on the sinusoidal wave node.
    void recoverSinWave();   ///< Called to recover the sinusoidal wave node from a failure.

    void switchOnSquareWave();  ///< Called to switch on the square wave acquisition node.
    void switchOffSquareWave(); ///< Called to switch off the square wave acquisition node
    void startSquareWave();     ///< Called to start the data acquisition on the square wave node.
    void stopSquareWave();      ///< Called to stop the data acquisition on the square wave node.
    void recoverSquareWave();   ///< Called to recover the square wave node from a failure.

    /**
     * @brief Called to verify if a state change is allowed
     * @return
     */
    bool allowChange(const nds::state_t, const nds::state_t, const nds::state_t);

    /**
     * @brief Function that continuously pushes a sinusoidal wave to m_acquisitionSinWave
     *        until the flag m_bStopAcquisitionSinWave is set to true.
     *
     * It is launched by startSinWave() in a separate thread.
     */
    void acquireSinusoidalWave();

    /**
     * @brief Function that continuously pushes a square wave to m_acquisitionThreadSquareWave
     *        until the flag m_bStopAcquisitionSquareWave is set to true.
     *
     * It is launched by startSinWave() in a separate thread.
     */
    void acquireSquareWave();

    /**
     * @brief A thread that runs acquireSinusoidalWave().
     */
    std::thread m_acquisitionThreadSinWave;

    /**
     * @brief A boolean flag that stop the acquisition loop in acquireSinusoidalWave()
     *        when true.
     */
    volatile bool m_bStopAcquisitionSinWave;

    /**
     * @brief A thread that runs acquireSquareWave().
     */
    std::thread m_acquisitionThreadSquareWave;

    /**
     * @brief A boolean flag that stop the acquisition loop in acquireSquareWave()
     *        when true.
     */
    volatile bool m_bStopAcquisitionSquareWave;
};

// THE IMPLEMENTATION FOR THE OSCILLOSCOPE CLASS FOLLOWS: IN A REALD LIFE
// SITUATION THIS WOULD BE IN A DIFFERENT FILE

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

NDS_DEFINE_DRIVER(Oscilloscope, Oscilloscope)
