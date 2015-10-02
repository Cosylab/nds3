#ifndef NDS3_DEFINITIONS_H
#define NDS3_DEFINITIONS_H


/**
 * @file definitions.h
 * @brief Defines all the enumeration and common types used across the NDS library.
 *
 * Include nds3.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include <cstdint>
#include <functional>
#include <time.h>
#include <string>
#include <list>
#include <vector>

namespace nds
{

/**
 * The type of control system that NDS must interface with.
 */
enum class controlSystem_t
{
    defaultSystem,  ///< The control system defined with setDefaultControlSystem()
    test,           ///< A Mock control system used by the test units
    epics,          ///< EPICS (http://www.aps.anl.gov/epics)
    tango           ///< Tango (http://tango-controls.org)

};

/**
 * @brief Available states, ordered by priority (lowest to higher).
 */
enum class state_t
{
    unknown,      ///< The state is Unknown
    off,          ///< The device is switched off
    switchingOff, ///< The device is switched on but is in the process of switching off
    initializing, ///< The device is switched off but is in the process of switching on
    on,           ///< The device is switched on
    stopping,     ///< The device is acquiring data but is in the process of stopping the operations
    starting,     ///< The device is switched on and is starting the operations
    running,      ///< The device is operating
    fault,        ///< An error caused the device to go to FAULT mode
    MAX_STATE_NUM ///< Indicate the number of states in the enumeration
};

/**
 * @brief PV data types
 */
enum class dataType_t
{
    /* The value zero is reserved to static asserts to verify that the correct cpp types are being used */

    dataInt8 = 1,     ///< Signed integer, 8 bits
    dataUint8,        ///< Unsigned integer, 8 bits
    dataInt32,        ///< Signed integer, 32 bits
    dataUint32,       ///< Unsigned integer, 32 bits
    dataFloat64,      ///< Float, 64 bits
    dataInt8Array,    ///< Array of signed 8 bit integers
    dataUint8Array,   ///< Array of unsigned 8 bit integers
    dataInt32Array,   ///< Array of signed 32 bit integers
    dataFloat64Array, ///< Array of 64 bit floats
    dataString        ///< String
};

/**
 * @brief Specify how to get the PV value.
 */
enum class scanType_t
{
    periodic, ///< The control system polls the value with a specified frequency
    passive,  ///< The control system polls the value only when needed
    interrupt ///< The device server pushes the value to the control system when it changes
};

/**
 * @brief Possible record types.
 *
 * The record types are mostly used by EPICS to auto-generate the db file.
 *
 * PVs that have the record type set to notSpecified will not be included in the
 *  auto generated db file.
 *
 * Input records have the bit 7 set to 1, output records have the bit 6 set to
 *  1.
 */
enum class recordType_t
{
    notSpecified = 0,  ///< The PV's type is not specified
    aao         = 64,  ///< Array analog output
    ao          = 65,  ///< Analog output
    bo          = 66,  ///< Binary output
    mbbo        = 68,  ///< Multi-bit binary output
    longout     = 67,  ///< Long output
    mbboDirect  = 69,  ///< Multi-bit binary output direct
    stringOut   = 70,  ///< String output
    waveformOut = 71,  ///< Waveform output. On EPICS db files "waveform" together with an output array type will be used
    aai         = 128, ///< Array analog input
    ai          = 129, ///< Analog input
    bi          = 130, ///< Binary input
    longin      = 131, ///< Long input
    mbbi        = 132, ///< Multi-bit binary input
    mbbiDirect  = 133, ///< Multi-bit binary input direct
    stringIn    = 134, ///< String input
    waveformIn  = 135  ///< Waveworm input. On EPICS db files "waveform" together with an input array type will be used
};


/**
 * @ingroup logging
 * @brief Defines the severities of the log information.
 */
enum class logLevel_t: std::uint8_t
{
    debug,   ///< Debug information
    info,    ///< Non critical information
    warning, ///< Warning
    error,   ///< Error
    none     ///< Nothing is logger
};

/**
 * @ingroup logging
 * @brief If the logging is enabled for the specified severity level then the
 *        macro returns the proper logging stream, otherwise it skips the logging
 *        operation.
 */
#define ndsLogStream(node, logLevel) \
    if(!(node).isLogLevelEnabled(logLevel)) {} else (node).getLogger(logLevel)

/**
 * @ingroup logging
 * @brief Log to the debug stream if the debug log level is enabled on the
 *        selected node.
 *
 * The last logged element must be std::endl.
 *
 * Example:
 * @code
 * ndsDebugStream(node) << "The value has been changed from " << value0 << " to value " << value1 << std::endl
 * @endcode
 */
#define ndsDebugStream(node) ndsLogStream(node, nds::logLevel_t::debug)

/**
 * @ingroup logging
 * @brief Log to the info stream if the info log level is enabled on the
 *        selected node.
 *
 * The last logged element must be std::endl.
 *
 * Example:
 * @code
 * ndsInfoStream(node) << "The value has been changed from " << value0 << " to value " << value1 << std::endl
 * @endcode
 */
#define ndsInfoStream(node) ndsLogStream(node, nds::logLevel_t::info)
#define ndsWarningStream(node) ndsLogStream(node, nds::logLevel_t::warning)
#define ndsErrorStream(node) ndsLogStream(node, nds::logLevel_t::error)



/**
 * @brief Definition for the function executed to allocate a driver.
 *
 * Returns a pointer to the allocated driver: the pointer will be passed
 *  to the function defined by deallocateDriver_t.
 *
 */
typedef std::function<void*(const std::string& parameter)> allocateDriver_t;


/**
 * @brief Definition for the function executed to deallocate a driver.
 *
 * Takes a pointer to the allocated driver.
 *
 */
typedef std::function<void (void*)> deallocateDriver_t;


/**
 * @brief Definition for the function executed during the state transition.
 *
 * An exception thrown during a state transition will cause the StateMachine to
 *  switch to the state_t::fault state, unless the exception StateMachineRollBack
 *  is thrown which cause a rollback to the previous state.
 */
typedef std::function<void ()> stateChange_t;


/**
 * @brief Definition for a function called to allow or deny a state transition.
 *
 * The function receives 3 parameters:
 * - the first one represents the current state
 * - the secont one represents the current global state
 * - the last one represents the desidered state
 *
 * The function must return true if the transition from the current state to
 *  the desidered state is allowed, or false otherwise.
 *
 * This function is called only after the state machine has verified that
 *  the requested transition is legal.
 */
typedef std::function<bool (const state_t, const state_t, const state_t)> allowChange_t;

/**
 * @ingroup timing
 * @brief Definition for a function called to retrieve a time.
 *
 * The function should return the UNIX epoch (seconds and nanoseconds).
 */
typedef std::function<timespec ()> getTimestampPlugin_t;

typedef std::function<void ()> threadFunction_t;


/**
 * @brief List of strings used for enumeration in PVs that support
 *        the enumeration field.
 */
typedef std::list<std::string> enumerationStrings_t;
}

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define NDS3_HELPER_DLL_IMPORT __declspec(dllimport)
  #define NDS3_HELPER_DLL_EXPORT __declspec(dllexport)
#else
  #if __GNUC__ >= 4
    #define NDS3_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define NDS3_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
  #else
    #define NDS3_HELPER_DLL_IMPORT
    #define NDS3_HELPER_DLL_EXPORT
  #endif
#endif


// NDS3_API is used for the public API symbols

#ifdef NDS3_DLL // defined if NDS3 is compiled as a DLL
  #ifdef NDS3_DLL_EXPORTS // defined if we are building the NDS3 DLL (instead of using it)
    #define NDS3_API NDS3_HELPER_DLL_EXPORT
  #else
    #define NDS3_API NDS3_HELPER_DLL_IMPORT
  #endif // NDS3_DLL_EXPORTS
#else // NDS3_DLL is not defined: this means NDS3 is a static lib.
  #define NDS3_API
#endif // NDS3_DLL

#endif // NDS3_DEFINITIONS_H
