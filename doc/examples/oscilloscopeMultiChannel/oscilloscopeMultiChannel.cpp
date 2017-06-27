#include <functional>
#include <sstream>
#include <math.h>
#include <unistd.h>

#include <nds3/nds.h>

class Channel; // Forward declaration

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
    std::vector<std::shared_ptr<Channel> > m_channels;
};


/**
 * @brief We use this class to represent a single channel in the Oscilloscope.
 *
 * The class just take the nds::Node to which the acquisition node has to be
 * added and then defines the acquisition node's PVs.
 *
 */
class Channel
{
public:
    /**
     * @brief Construct the channel.
     *
     * @param name       the name to be given to the channel
     * @param parentNode the node to which we add the channel's node
     */
    Channel(const std::string& name, nds::Node& parentNode);

    /**
     * @brief We keep a reference to the acquisition node.
     */
    nds::DataAcquisition<std::vector<std::int32_t> > m_acquisition;

    /*
     * @brief A variable PV that stores the amplitude of the wave.
     */
    nds::PVVariableOut<std::int32_t> m_amplitude;

    void switchOn();  ///< Called to switch on the sinusoidal wave acquisition node.
    void switchOff(); ///< Called to switch off the sinusoidal wave acquisition node.
    void start();     ///< Called to start the data acquisition on the sinusoidal wave node.
    void stop();      ///< Called to stop the data acquisition on the sinusoidal wave node.
    void recover();   ///< Called to recover the sinusoidal wave node from a failure.

    /**
     * @brief Called by NDS when it wants to know if we allos a state change.
     * @return
     */
    bool allowChange(const nds::state_t, const nds::state_t, const nds::state_t);

    /**
     * @brief Loop that performs the data acquisition.
     */
    void acquisitionLoop();

    /**
     * @brief A boolean flag that stop the acquisition loop in acquisitionLoop()
     *        when true.
     */
    volatile bool m_bStopAcquisition;

    /**
     * @brief A thread that runs acquisitionLoop().
     */
    nds::Thread m_acquisitionThread;
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
Oscilloscope::Oscilloscope(nds::Factory &factory, const std::string &deviceName, const nds::namedParameters_t & /* parameters */)
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

    // Let's add 32 acquisition channels to the root node
    /////////////////////////////////////////////////////
    for(size_t numChannel(0); numChannel != 16; ++numChannel)
    {
        std::ostringstream channelName;
        channelName << "Ch" << numChannel;
        m_channels.push_back(std::make_shared<Channel>(channelName.str(), rootNode));
    }

    // We have declared all the nodes and PVs in our device: now we register them
    //  with the control system that called this constructor.
    ////////////////////////////////////////////////////////////////////////////////
    rootNode.initialize(this, factory);
}


////////////////////////////////////////////////////////////////////////////////
//
// Constructor for an acquisition channel
//
////////////////////////////////////////////////////////////////////////////////
Channel::Channel(const std::string &name, nds::Node &parentNode)
{
    nds::Node channel = parentNode.addChild(nds::Node(name));

    // We create an acquisition node with the requested name...
    ////////////////////////////////////////////////////////////////////////////////
    m_acquisition = nds::DataAcquisition<std::vector<std::int32_t> >("Acquisition",
                                                                     100,
                                                                     std::bind(&Channel::switchOn, this),
                                                                     std::bind(&Channel::switchOff, this),
                                                                     std::bind(&Channel::start, this),
                                                                     std::bind(&Channel::stop, this),
                                                                     std::bind(&Channel::recover, this),
                                                                     std::bind(&Channel::allowChange, this,
                                                                               std::placeholders::_1,
                                                                               std::placeholders::_2,
                                                                               std::placeholders::_3));

    // ...and we add it to the root
    ////////////////////////////////////////////////////////////////////////////////
    channel.addChild(m_acquisition);

    // We also add to the acquisition node a PV that specifies the amplitude of the
    //  generated wave
    ////////////////////////////////////////////////////////////////////////////////
    m_amplitude = nds::PVVariableOut<std::int32_t>("Amplitude");
    channel.addChild(m_amplitude);
}

////////////////////////////////////////////////////////////////////////////////
//
// Called when the acquisition node has to be switched on.
// Here we do nothing (in our case no special operations are needed to switch
//  on the node).
//
////////////////////////////////////////////////////////////////////////////////
void Channel::switchOn()
{
}


////////////////////////////////////////////////////////////////////////////////
//
// Called when the acquisition node has to be switched off.
// Here we do nothing (in our case no special operations are needed to switch
//  off the node).
//
////////////////////////////////////////////////////////////////////////////////
void Channel::switchOff()
{
}


////////////////////////////////////////////////////////////////////////////////
//
// Called when the acquisition node has to start acquiring.
// We start the data acquisition thread for the sinusoidal wave
//
////////////////////////////////////////////////////////////////////////////////
void Channel::start()
{
    m_bStopAcquisition = false; //< We will set to true to stop the acquisition thread

    // Start the acquisition thread.
    // We don't need to check if the thread was already started because the state
    //  machine guarantees that the start handler is called only while the state
    //  is ON.
    ////////////////////////////////////////////////////////////////////////////////
    m_acquisitionThread = m_acquisition.runInThread("Acquisition", std::bind(&Channel::acquisitionLoop, this));
}


////////////////////////////////////////////////////////////////////////////////
//
// Stop the acquisition thread
//
////////////////////////////////////////////////////////////////////////////////
void Channel::stop()
{
    m_bStopAcquisition = true;
    m_acquisitionThread.join();
}


////////////////////////////////////////////////////////////////////////////////
//
// A failure during a state transition will cause the state machine to switch
//  to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force
//  the state machine to stay on the failure state.
//
////////////////////////////////////////////////////////////////////////////////
void Channel::recover()
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
bool Channel::allowChange(const nds::state_t, const nds::state_t, const nds::state_t)
{
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//
// Acquisition function. It is launched in a separate thread by start().
//
////////////////////////////////////////////////////////////////////////////////
void Channel::acquisitionLoop()
{
    // Let's allocate a vector that will contain the data that we will push to the
    //  control system
    ////////////////////////////////////////////////////////////////////////////////
    std::vector<std::int32_t> outputData(m_acquisition.getMaxElements());

    // A counter for the angle in the sin() operation
    ////////////////////////////////////////////////////////////////////////////////
    std::int64_t angle(0);

    // Run until the state machine stops us
    ////////////////////////////////////////////////////////////////////////////////
    while(!m_bStopAcquisition)
    {
        // Fill the vector with a sin wave
        ////////////////////////////////////////////////////////////////////////////////
        size_t maxAmplitude = m_amplitude.getValue(); // PVVariables are thread safe
        for(size_t scanVector(0); scanVector != outputData.size(); ++scanVector)
        {
            outputData[scanVector] = (double)maxAmplitude * sin((double)(angle++) / 10.0f);
        }

        // Push the vector to the control system
        ////////////////////////////////////////////////////////////////////////////////
        m_acquisition.push(m_acquisition.getTimestamp(), outputData);

        // Rest for a while
        ////////////////////////////////////////////////////////////////////////////////
        ::usleep(100000);
    }
}

// The following MACRO defines the function to be exported in order
//  to allow the dynamic loading of the shared module
///////////////////////////////////////////////////////////////////
NDS_DEFINE_DRIVER(OscilloscopeMultiChannel, Oscilloscope)
