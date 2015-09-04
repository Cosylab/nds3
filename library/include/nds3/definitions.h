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

namespace nds
{

/**
 * @brief Available states, ordered by priority (lowest to higher).
 */
enum state_t
{
    unknownState,
    off,
    switchingOff,
    initializing,
    on,
    stopping,
    starting,
    running,
    fault
};

/**
 * @brief PV data types
 */
enum dataType_t
{
    dataInt8,
    dataUint8,
    dataInt32,
    dataUint32,
    dataFloat64,
    dataInt8Array,
    dataUint8Array,
    dataInt32Array,
    dataUint32Array,
    dataFloat64Array,
    dataString
};

enum scanType_t
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
 */
enum recordType_t
{
    notSpecified, ///< The PV's type is not specified
    aai,          ///< Array analog input
    aao,          ///< Array analog output
    ai,           ///< Analog input
    ao,           ///< Analog output
    bi,           ///< Binary input
    bo,           ///< Binary output
    longin,       ///< Long input
    longout,      ///< Long output
    mbbi,         ///< Multi-bit binary input
    mbbo,         ///< Multi-bit binary output
    mbbiDirect,   ///< Multi-bit binary input direct
    mbboDirect,   ///< Multi-bit binary output direct
    stringIn,     ///< String input
    stringOut,    ///< String output
    waveformIn,   ///< Waveworm input. On EPICS db files "waveform" together with an input array type will be used
    waveformOut   ///< Waveform output. On EPICS db files "waveform" together with an output array type will be used
};



typedef std::function<void ()> stateChange_t;
typedef std::function<bool (const state_t, const state_t, const state_t)> allowChange_t;

typedef std::function<timespec ()> getTimestampPlugin_t;



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
