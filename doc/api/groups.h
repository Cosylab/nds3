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




*/

}
