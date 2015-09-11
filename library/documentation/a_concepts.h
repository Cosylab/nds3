/**

@page introduction_concepts Concepts

NDS tries to hide away the details of the control system for which you are developing the device support.

In order to do so NDS performs several operations under the hood, like converting timestamps from/to
 EPICS epoch, converting pointers to raw data from/to std::vector objects, taking care of registering the
 device attributes or PVs with the control system.

On the other hand, you will have to get some familiarity with few new concepts not present in any of the
 supported control system.

In particular, NDS maps the device to a tree-like structure that you must define in your code. It also
 supplies a hierarchical state machine that has no equivalent in EPICS nor Tango.


@section introduction_tree_like_structure The tree structure

You should imagine your device as a tree-like structure, in which the leaves are the device attributes
 (or PVs in EPICS parlance), and the root could be the device itself.

In NDS terminology the leaves (device attributes or PVs) are called PVs while all the other parts of the
 structure are called Nodes. Nodes can have children (other Nodes or PVs) while PVs cannot have any
 child.\n
One or more Nodes can be given the task to communicate directly with the control system: those nodes are
 called Ports. All the children of a Port (and children of the children) will use the parent Port to communicate
 with the control system. Ports also define the thread in which the data communication is executed: all the PVs that
 have the same parent Port will be written or polled in the same thread.


Now lets try to map a simple acquisition device, an oscilloscope, into a tree structure ready to be
 used in NDS.\n
We assume that our oscilloscope has 2 channels and each channel provides:
- a data acquisition PV which acquires the data
- a frequency PV with which we set the acquisition frequency

We could structure our oscilloscope like this:
- a Port named "Oscilloscope" as the root node. This node will take care of communicating with the control system
- the port will contain 2 children Nodes: "channel1" and "channel2"
- both channel1 and channel2 will contain 2 children PVs: "dataAcquisition" and "frequency"

@image html Oscilloscope_structure_1.png
@image latex Oscilloscope_structure_1.png

Alternatively we could have chosen to have a separate port for each channel so both of them get a dedicated
 thread for polling:
- a root Node named "Oscilloscope"
- the root node will contain two children Ports: "channel1" and "channel2"
- both channel1 and channel2 will contain 2 children PVs: "dataAcquisition" and "frequency"

When using the first structure then on the control system side we will see a single device named "Oscilloscope"
 containing four PVs:
- "channel0-dataAcquisition"
- "channel0-frequency"
- "channel1-dataAcquisition"
- "channel1-frequency"

@image html Oscilloscope_structure_2.png
@image latex Oscilloscope_structure_2.png

When using the second structure then on the control system side we will see two separate devices:
- "Oscilloscope-channel0" with two PVs: "dataAcquisition" and "frequency"
- "Oscilloscope-channel1" with two PVs: "dataAcquisition" and "frequency"

On EPICS, in all the cases the PVs will present the same names but the way they are polled will differ.
The PV names in EPICS will be:
- "Oscilloscope-channel0-dataAcquisition"
- "Oscilloscope-channel0-frequency"
- "Oscilloscope-channel1-dataAcquisition"
- "Oscilloscope-channel1-frequency"


@section introduction_tree_nodes The Nodes

Nodes in the device tree-like structure are objects that can have children.

Nodes have a name: the node's name is linked to all the parent nodes' names to form the fully qualified
 node name. For instance, the node "channel1" child of the root node "Oscilloscope" will have a fully
 qualified name "Oscilloscope-channel1".

NDS provides also specialized nodes:
- a state machine: this node declares its own children PVs that allow to monitor and/or set the state of the
  node that owns the state machine. It also provides validation of the requested states and functions that
  initiate the state transitions
- a data acquisition node: this node declares its own children PVs for data acquisition purposes and provides
  few helper functions that can launch and stop a separate acquisition loop.\n
  The data acquisition node also has its own state machine node, used to launch and stop the data acquisition


@section introduction_tree_pvs The PVs

PVs are the leaves of the device tree-like structure: they cannot have children nodes nor PVs and they are used
 to set or retrieve the device's internal values (PV in EPICS terminology, attribute in Tango terminology).

PVs have a name: the PV name is linked to all the parent nodes' names to form the fully qualified PV name.
 For instance, the PV "dataAcquisition" child of the node "channel1" child of "Oscilloscope" will have a fully
 qualified name "Oscilloscope-channel1-dataAcquisition".

In EPICS, the NDS fully qualified PV name is also the name used to declare the ChannelAccess PV name. In Tango
 the qualified name AS SEEN BY THE PORT NODE is the final attribute name.

NDS provides several types of PVs:
- a Variable PV (nds::PVVariable). This PV stores the data internally: When the control system writes or reads
  a value into/from a Variable PV then the user source code is not called and NDS will carry out the operation
  by itself. The user code may query or set the Variable PV at any moment but this will not cause any immediate
  interaction with the control system; the control system will be able to read back the new value the next
  time it will poll the PV
- a Delegate PV (nds::PVDelegate). This PV does not store the data internally: when the control system writes
  or reads a PV value then the delegate PV will call the user code supplied methods, which will be responsible
  for retrieving or setting the proper value and timestamp
- a Hold Delegate PV (nds::PVHoldDelegate). This PV is similar to the delegate PV, but calls delegated methods
  from a Delegate class, which lifetime is also controlled by the Hold Delegate PV. This PV is mostly used with
  Python bindings and the normal Delegate PV should be used for normal C++ applications

All the PV supply a push method which can push data immediately to the control system, without waiting for the
 next polling operation.


@section introduction_references_to_objects Nodes and PVs lifetime (instatiation, copy, deletion, usage)

When instatiating a PV, Node or any other object derived from nds::Base then NDS creates the desidered object but
 returns to your code just a variable holding a reference to the allocated object.\n
The returned variable is just a proxy that supplies the public functions of the referenced object: you can
 call directly the functions exposed by the returned proxy, which in turn will forward the calls to the
 referenced object (the implementation).

NDS uses reference counters to decide when to remove an object from memory, so your application should not worry
 about freeing nds::Base derived object.

Also, when copying NDS objects with the copy operator you are just copying the object reference. For instance:

@code
nds::Port oscilloscope("Oscilloscope");
nds::Node channel1 = oscilloscope.addChild(nds::Node("Channel1"));
nds::PVBase dataAcquisition = channel1.addChild(nds::PVVariable<std::int32_t>("dataAcquisition"));

// Here we register all the PVs. After this operation
//  NDS will hold a reference to our root node, so we can
//  let it go out of scope and it will still exist in memory
//  until NDS lets it go
oscilloscope.initialize(); // Register all the declared PVs

// Let's write a value in a copy of dataAcquisition
nds::PVBase copyOfDataAcquisition = dataAcquisition;
copyOfDataAcquisition.write(copyOfDataAcquisition.getTimestamp(), (std::int32_t)10);

// We can read back the written value from dataAcquisition, because
// copyOfDataAcquisition actually manages the same object
std::int32_t readback;
timespec timestamp;
dataAcquisition.read(&timestamp, readback);
@endcode

The nds::PVBase::initialize() call performed on the root node causes NDS to scan
 all the children and register the PV in the control system: during the initialization
 NDS will also store a reference to the root node, preventing it to be deleted until
 it is needed by NDS.


*/
