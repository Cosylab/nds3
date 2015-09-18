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
    dataInt8,
    dataUint8,
    dataInt32,
    dataUint32,
    dataFloat64,
    dataInt8Array,
    dataUint8Array,
    dataInt32Array,
    dataFloat64Array,
    dataString
};

enum class scanType_t
{
    periodic,
    passive,
    interrupt
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
 * @brief Definition for the function executed to allocate a driver.
 *
 * Returns a pointer to the allocated driver: the pointer will be passed
 *  to the function defined by deallocateDriver_t.
 *
 */
typedef std::function<void*(const std::string& parameter)> allocateDriver_t;

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
 * @brief Definition for a function called to retrieve a time.
 *
 * The function should return the UNIX epoch (seconds and nanoseconds).
 */
typedef std::function<timespec ()> getTimestampPlugin_t;

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
