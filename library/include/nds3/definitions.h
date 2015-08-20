#ifndef NDS3_DEFINITIONS_H
#define NDS3_DEFINITIONS_H

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


// GOINGTHERE_API is used for the public API symbols
// GOINGTHERE_LOCAL is used for non-api symbols

#ifdef NDS3_DLL // defined if GoingThere is compiled as a DLL
  #ifdef NDS3_DLL_EXPORTS // defined if we are building the GoingThere DLL (instead of using it)
    #define NDS3_API NDS3_HELPER_DLL_EXPORT
  #else
    #define NDS3_API NDS3_HELPER_DLL_IMPORT
  #endif // GOINGTHERE_DLL_EXPORTS
#else // GOINGTHERE_DLL is not defined: this means GoingThere is a static lib.
  #define NDS3_API
#endif // GOINGTHERE_DLL

#endif // NDS3_DEFINITIONS_H
