TEMPLATE = app
CONFIG += console
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x -Wall -Wextra -pedantic -pthread

DEFINES += NDS3_DLL
DEFINES += NDS3_DLL_IMPORT


INCLUDEPATH += \
    /opt/codac-5.0/common/gtest/include \
    /opt/codac-5.0/common/gtest/

LIBS +=  -L/opt/codac-5.0/lib -lnds3

SOURCES += \
    /opt/codac-5.0/common/gtest/src/gtest-typed-test.cc \
    /opt/codac-5.0/common/gtest/src/gtest-test-part.cc \
    /opt/codac-5.0/common/gtest/src/gtest-port.cc \
    /opt/codac-5.0/common/gtest/src/gtest-filepath.cc \
    /opt/codac-5.0/common/gtest/src/gtest-death-test.cc \
    /opt/codac-5.0/common/gtest/src/gtest.cc \
    src/testDevice.cpp \
    src/testDeviceAllocation.cpp \
    src/testDataAcquisition.cpp \
    src/ndsTestInterface.cpp \
    src/ndsTestFactory.cpp \
    src/main.cpp


HEADERS += \
    include/testDevice.h \
    include/ndsTestInterface.h \
    include/ndsTestFactory.h
