## Getting started

This section will guide you through the development of a simple Device Support based on the NDS3 framework

Here we assume that you already installed the NDS3 library and the NDS3 control system layer of your choice.


### NDS3 concepts

Device Supports for the NDS3 framework must declare the device functionalities using a tree-like structure: the tree 
structure can be composed by:

- Nodes
- Process Variables (PVs)

#### Nodes

A node can contain children nodes or PVs. There are several kind of nodes:

- plain Node: it is just used to organize the device's component
- Port: it is a node that is able to communicate with an instance of the underlying control system. All the children
  of a port will communicate with the control system using their parent port
- StateMachine: a node that comes with few pre-defined children representing the node's state and supplies few
  functionalities to manage the states
- DataAcquisition: a node that comes with few pre-defined children PVs and a state machine useful for controlling
  data acquisition.

The responsability of defining the device structure falls on the constructor of the class that the developer decides
to use as representation of the device: when a specific device support is needed then NDS3 will allocate and
construct the class that represents the device and expects that the constructor of the class will specify all the nodes
and PVs that are needed.


#### Process variables

PVs, or Process Variables, are variables that are visible from both the Device Support and from the clients connected to
the control system.

NDS3 differentiates between input PVs and output PVs;

- input PVs are WRITTEN by the Device Support and READ by the control system and its clients
- output PVs are READ by the Device Suppoer and WRITTEN by the control system and its clients

For instance, a device representing a temperature sensor will have one *input* PV that it will update regularly with the
detected temperature, while a power supply could have an *output* PV that set the desidered output voltage.

NDS3 provides 2 kinds of input and output PVs:

- PVVariableIn and PVVariableOut: the Variable PVs are responsible for storing the value written by the Device Support or
  by the Control System and are able to supply the internal value on demand to both the Device Support and the Control System.
  This means that the Device Support can just read or write a value into them when it is convenient, without worrying about
  the mechanism and the timing that the Control System will use to read/write the PV
- PVDelegateIn and PVDelegateOut: the Delegate PVs will call a predefined Device Support's function when the Control System
  wants to read or write some data from/to the PV

All the input PVs also support the push model, which allows the Device Support to forcefully push data to the Control System
at any moment.


### A simple Device Support: a thermometer

The first device that we are going to build is a really simple thermometer: It has a single input PV which supplies the
temperature in Kelvin degrees. 
  
The structure of the termomether device is really simple: it provides a root node (a Port, so it can communicate with the
Control System) that represents the device and an input PV that the control system will read to get the current temperature.

Let's put this in C++ form:

    #include <nds3/nds.h>

    // This class declares our device in the contructor and supplies the functionalities to support it
    //////////////////////////////////////////////////////////////////////////////////////////////////
    class Thermometer
    {
        public:

        // Constructor. Here we setup the device structure
        //////////////////////////////////////////////////
        Thermometer(nds::Factory &factory,
                    const std::string &deviceName,
                    const nds::namedParameters_t &parameters)
        {
            nds::Port rootNode(deviceName);
            rootNode.addChild(nds::PVDelegateIn<double>("Temperature", std::bind(&Thermometer::getTemperature, 
                                                                                 this, 
                                                                                 std::placeholders::_1, 
                                                                                 std::placeholders::_2));

            rootNode.initialize(this, factory);
        }

        void getTemperature(timespec* pTimestamp, double* pValue)
        {
            *pValue = 10; // It is always cold in here
        }
    };

    // If we want to load our device support dynamically then we have to compile it
    //  as a dynamic module and we have to provide the functions to allocate it,
    //  deallocate it and retrieve its name
    ////////////////////////////////////////////////////////////////////////////////
    NDS_DEFINE_DRIVER("Thermometer", Thermometer);

What did we do with this code?

The constructor takes three parameters passed by NDS3 when it allocates the device:

- factory: this parameter contains an interface to the Control System that requested the creation of the device
- deviceName: the name with which the device should be presented to the control system. Usually it is used as name of the root node
- parameters: a map of named parameters that are specific for each Device Support. The implementor of the Device Support can decide
  how to use the parameters

We allocate a Port as root node: a Port node holds a reference to the underlying control system. PVs can be added only to Ports or to
nodes that have a Port up in the hierarchy.

The variable rootNode contains a shared pointer to the acual implementation of the Port: when we will register rootNode with the
Control System then NDS will take care of storing the shared pointer internally and therefore the Port will continue to exists
even after rootNode goes out of scope.
*In NDS3, all the device components just hold shared pointers to the actual components implementation*.

After we declare the root node then we call addChild() on it to add a child component, in our case a PVDelegateIn that handles double
floating point values.
PVDelegateIN PVs are not able to store the value internally but rely on a function that is called when the control system wants
to read the value. In our case we provide a callback to getTemperature() which always return 10 degrees Kelvin.

Finally we call initialize() which registers the root node and its children with the Control System.

If we are running our Device Support using EPICS as control system then the following PV will be available:

    rootNodeName-Temperature

Issuing a dbpf rootNodeName-Temperature.PROC 1 will cause the framework to call getTemperature() and fill the PV with the value 10.


    
