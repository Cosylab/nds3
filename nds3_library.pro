TEMPLATE = lib
QT -= qt
CONFIG += dll
TARGET = nds3

# Library flags
#--------------
QMAKE_CXXFLAGS += -std=c++0x -Wall -Wextra -pedantic -fPIC -pthread -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -fPIC -Wl,-as-needed

# We are building the library, export symbols
#--------------------------------------------
DEFINES += NDS3_DLL
DEFINES += NDS3_DLL_EXPORTS

LIBS += -lrt -ldl

tango:LIBS += -L/usr/local/lib -ltango -llog4tango -lomniORB4 -lomnithread -lomniDynamic4

tango:INCLUDEPATH += /usr/local/include/tango /usr/include/omniORB4

SOURCES +=\
    src/scansymbols.cpp \
    src/base.cpp \
    src/baseImpl.cpp \
    src/dataAcquisition.cpp \
    src/dataAcquisitionImpl.cpp \
    src/delegate.cpp \
    #src/epicsFactoryImpl.cpp \
    #src/epicsInterfaceImpl.cpp \
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
    src/tangoFactoryImpl.cpp \
    src/tangoInterfaceImpl.cpp \
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
    src/scansymbols.h \
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
    src/epicsFactoryImpl.h \
    src/dataAcquisitionImpl.h \
    src/epicsInterfaceImpl.h \
    src/factoryBaseImpl.h \
    src/interfaceBaseImpl.h \
    src/nodeImpl.h \
    src/portImpl.h \
    src/pvBaseImpl.h \
    src/pvHoldDelegateImpl.h \
    src/stateMachineImpl.h \
    src/tangoFactoryImpl.h \
    src/tangoInterfaceImpl.h \
    src/baseImpl.h \
    src/logStreamGetterImpl.h \
    doc/groups.h \
    include/nds3/pvBaseIn.h \
    include/nds3/pvBaseOut.h \
    src/pvBaseInImpl.h \
    src/pvBaseOutImpl.h \
    include/nds3/pvDelegateIn.h \
    include/nds3/pvDelegateOut.h \
    src/pvDelegateInImpl.h \
    src/pvDelegateOutImpl.h \
    include/nds3/pvVariableIn.h \
    include/nds3/pvVariableOut.h \
    src/pvVariableInImpl.h \
    src/pvVariableOutImpl.h \
    src/ndsFactoryImpl.h

