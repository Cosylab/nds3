TEMPLATE = app
CONFIG += console
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x -Wall -Wextra -pedantic -pthread
debug:QMAKE_CXXFLAGS += --coverage

DEFINES += NDS3_DLL
DEFINES += NDS3_DLL_IMPORT


LIBS +=  -lgtest -lnds3 -lpthread
debug:LIBS += -lgcov

SOURCES += \
    src/testDevice.cpp \
    src/testDeviceAllocation.cpp \
    src/testDataAcquisition.cpp \
    src/ndsTestInterface.cpp \
    src/ndsTestFactory.cpp \
    src/main.cpp \
    src/testStateMachine.cpp


HEADERS += \
    include/testDevice.h \
    include/ndsTestInterface.h \
    include/ndsTestFactory.h
