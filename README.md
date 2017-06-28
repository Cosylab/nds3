# NDS v3

## Introduction

NDS (Nominal Device Support) is a library that allows to write device supports
for a variety of control systems (CS) by hiding the details of the chosen CS
and focusing on device functionalities.

NDS provides the following features:

- organizes your device in a tree-like structure, containing one or more
  devices, channels, attributes (PVs), state machine, etc.
- provides the data pull mode (passive scanning on EPICS, polling on Tango) and
  the push mode (Interrupt on EPICS, push on Tango)
- your code uses standard C++ types and Unix EPOCH timing to communicate with
  the library
- on EPICS there is no need to supply separate db files (but you still can if
  you want)
- supplies an hierarchical state machine
- supplies a specialized data acquisition module
- guaranteed binary compatibility between minor versions allow to easily
  upgrade existing installations without recompiling the device support

[![Build Status](https://travis-ci.org/Cosylab/nds3.svg?branch=master)](https://travis-ci.org/Cosylab/nds3)

## Documentation

- [Online Reference](https://cosylab.github.io/nds3/)
- Build doxygen reference with `make doc`. Requires doxygen and latex. The
  generated documentation is then found at `doc/api/html/index.html` and
  `doc/api/hlatex/refman.pdf`.

## Developer Dependencies

cmake 2.8.2+ required to run unit tests. Included in RHEL 6.6, Debian Wheezy, Ubuntu Trusty.

GCC 4.9+ is recommended because of [GCC 57869](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=57869).

## Package Installation

Packages are not available yet.

## Manual Installation

### Using cmake

```
mkdir build
cd build
cmake ../CMake
make install
```

### Using make

```
make install
```

## Run unit tests

- Build NDS3 with CMake
- Build and run tests
    ```
    mkdir tests/build
    cd tests/build
    cmake ../CMake -DLIBRARY_LOCATION=../../build
    make
    ./nds3tests
    ```
## Build example drivers

- Build NDS3 with CMake
- Build examples
    ```
    mkdir doc/examples/build
    cd doc/examples/build
    cmake ../CMake -DLIBRARY_LOCATION=../../../build
    make
    ```
