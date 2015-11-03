TEMPLATE = lib
QT = ""
CONFIG += dll
TARGET = nds3

VERSION = 3.0.0

# Library flags
#--------------
QMAKE_CXXFLAGS += -std=c++0x -Wall -Wextra -pedantic -fPIC -pthread -fvisibility=hidden -fvisibility-inlines-hidden
debug:QMAKE_CXXFLAGS += --coverage

QMAKE_LFLAGS += -fPIC -Wl,-as-needed

# We are building the library, export symbols
#--------------------------------------------
DEFINES += NDS3_DLL
DEFINES += NDS3_DLL_EXPORTS

LIBS += -lrt -ldl
debug:LIBS += -lgcov

SOURCES +=\
    src/base.cpp \
    src/baseImpl.cpp \
    src/dataAcquisition.cpp \
    src/dataAcquisitionImpl.cpp \
    src/delegate.cpp \
    src/exceptions.cpp \
    src/factory.cpp \
    src/factoryBaseImpl.cpp \
    src/interfaceBaseImpl.cpp \
    src/node.cpp \
    src/nodeImpl.cpp \
    src/port.cpp \
    src/portImpl.cpp \
    src/pvBase.cpp \
    src/pvBaseImpl.cpp \
    src/pvHoldDelegate.cpp \
    src/pvHoldDelegateImpl.cpp \
    src/stateMachine.cpp \
    src/stateMachineImpl.cpp \
    src/logStreamGetterImpl.cpp \
    src/pvBaseIn.cpp \
    src/pvBaseOut.cpp \
    src/pvBaseOutImpl.cpp \
    src/PVBaseInImpl.cpp \
    src/pvDelegateIn.cpp \
    src/pvDelegateOut.cpp \
    src/pvDelegateOutImpl.cpp \
    src/pvDelegateInImpl.cpp \
    src/pvVariableOut.cpp \
    src/pvVariableIn.cpp \
    src/pvVariableInImpl.cpp \
    src/pvVariableOutImpl.cpp \
    src/ndsFactoryImpl.cpp

HEADERS += \
    include/nds3/definitions.h \
    include/nds3/nds.h \
    include/nds3/base.h \
    include/nds3/dataAcquisition.h \
    include/nds3/delegate.h \
    include/nds3/exceptions.h \
    include/nds3/factory.h \
    include/nds3/node.h \
    include/nds3/port.h \
    include/nds3/pvBase.h \
    include/nds3/pvHoldDelegate.h \
    include/nds3/stateMachine.h \
    include/nds3impl/dataAcquisitionImpl.h \
    include/nds3impl/factoryBaseImpl.h \
    include/nds3impl/interfaceBaseImpl.h \
    include/nds3impl/nodeImpl.h \
    include/nds3impl/portImpl.h \
    include/nds3impl/pvBaseImpl.h \
    include/nds3impl/pvHoldDelegateImpl.h \
    include/nds3impl/stateMachineImpl.h \
    include/nds3impl/baseImpl.h \
    include/nds3impl/logStreamGetterImpl.h \
    include/nds3/pvBaseIn.h \
    include/nds3/pvBaseOut.h \
    include/nds3impl/pvBaseInImpl.h \
    include/nds3impl/pvBaseOutImpl.h \
    include/nds3/pvDelegateIn.h \
    include/nds3/pvDelegateOut.h \
    include/nds3impl/pvDelegateInImpl.h \
    include/nds3impl/pvDelegateOutImpl.h \
    include/nds3/pvVariableIn.h \
    include/nds3/pvVariableOut.h \
    include/nds3impl/pvVariableInImpl.h \
    include/nds3impl/pvVariableOutImpl.h \
    include/nds3impl/ndsFactoryImpl.h \
    include/nds3impl/threadImpl.h

