namespace nds
{

/**

@defgroup logging Logging

The Logging methods can be used to send debug, information, warning or errors to the control system.

The logging interface is control-system agnostic.

Each logged information is tied to a node, and the log level of each node can be set separately.
For instance, it is possible to have the whole device (root node) with a log level set to "warning"
 and only a specific state machine set to "debug".

The logging level of each node can be set with Base::setLogLevel().

Each node provides a different std::ostream object for each severity level and for each thread.
Your application can retrieve the std::ostream object by using Base::getLogger().

@warning Altought it is possible to use directly Base::getLogger() in order to perform the log
         operations, the suggested method is the usage of the logging macros ndsDebugStream,
         ndsInfoStream, ndsWarningStream and ndsErrorStream.\n
         \n
         The loggin macros boost the performances by avoiding the log altogheter when it is not
         necessary



@defgroup datareadwrite Data input/output

The control system can access the data via PVs (process variables). The base PV class is PVBase,
 from which PVDelegate and PVVariable are derived.

Both PVDelegate and PVVariable provide the read and write methods (inherited from PVBase), but
 while PVDelegate calls two user-defined delegate functions to perform the read and write operation,
 PVVariable takes care of storing the value so it does not need to refer to delegate functions
 and can carry out the read and write operations by itself.

PVBase also provides a PVBase::push() method that allow to push data directly to the control system
 without waiting for it to poll it.



@defgroup naming Naming policies

Each node or PV of a device has a "component name" that identifies the node within its siblings with the same
parent, and a "full name" that uniquely identifies the node in the device.

The full name is calculated by prepending all the parents' names to the component name.

Let's consider the following structure of nodes (each line represents a node, the indentation shows the
 position in the tree structure):

- "rootNode"
  - "AcquisitionNode0"
    - "Data"
    - "Frequency"
  - "AcquisitionNode1"
    - "Data"
    - "Frequency"

The structure shows that there are two nodes with the name "Data", but their full name differs: the first
node has the full name "rootNode-AcquisitionNode0-Data" while the second node has the full name
"rootNode-AcquisitionNode1-Data".

Internally NDS always separates the component names with a dash ("-").

@warning It is not possible to have two nodes with the same full name.

Each node can be exposed to the control system by using a name that is different from the internal name.

For each node you can set the external name via nds::Base::setExternalName(). The default value of the
external name is the same as the component name.

The external name is also processed by the naming rules in order to calculate the full name with which the
node is exposed to the control system.





*/

}
