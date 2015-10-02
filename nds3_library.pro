TEMPLATE = lib
QT -= qt
CONFIG += dll
TARGET = nds3

# Target that includes the dbd file into a cpp file
# Define how to create dbd.h
dbd.target = dbd.h
dbd.commands = echo \'static const char dbdfile[] = {\' > $$PWD/dbd/dbdfile.h && \
               xxd -i < $$PWD/dbd/dbdfile.dbd >> $$PWD/dbd/dbdfile.h && \
               echo \'};\' >> $$PWD/dbd/dbdfile.h
dbd.depends = FORCE

QMAKE_EXTRA_TARGETS += dbd

PRE_TARGETDEPS += dbd.h

# Find EPICS libraries position
#------------------------------
epics:EPICS_BASE = $$(EPICS_BASE)
epics:EPICS_HOST_ARCH = $$(EPICS_HOST_ARCH)

epics:LIBS_BASE = $$EPICS_BASE/lib/$$EPICS_HOST_ARCH
epics:LIBS_ASYN = $$EPICS_BASE/../modules/asyn/lib/$$EPICS_HOST_ARCH

# Library flags
#--------------
QMAKE_CXXFLAGS += -std=c++0x -Wall -Wextra -pedantic -fPIC -pthread -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -fPIC -Wl,-as-needed

# Epics only library flags
#-------------------------
epics: QMAKE_LFLAGS += -Wl,-rpath,$$LIBS_BASE
epics: QMAKE_LFLAGS += -Wl,-rpath,$$LIBS_ASYN

# We are building the library, export symbols
#--------------------------------------------
DEFINES += NDS3_DLL
DEFINES += NDS3_DLL_EXPORTS

epics:DEFINES += NDS3_EPICS
tango:DEFINES += NDS3_TANGO

epics:LIBS += -L$$LIBS_BASE \
    -L$$LIBS_ASYN \
    -ldbCore -ldbRecStd -lgdd -lasyn \
    -lca -lcas -lCom -ldl

tango:LIBS += -L/usr/local/lib -ltango -llog4tango -lomniORB4 -lomnithread -lomniDynamic4

epics:INCLUDEPATH += $$EPICS_BASE/include \
    $$EPICS_BASE/include/compiler/gcc \
    $$EPICS_BASE/include/os/Linux/ \
    $$EPICS_BASE/../modules/asyn/include \

tango:INCLUDEPATH += /usr/local/include/tango /usr/include/omniORB4

SOURCES +=\
    src/scansymbols.cpp \
    src/base.cpp \
    src/baseImpl.cpp \
    src/dataAcquisition.cpp \
    src/dataAcquisitionImpl.cpp \
    src/delegate.cpp \
    src/epicsFactoryImpl.cpp \
    src/epicsInterfaceImpl.cpp \
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
    src/pvDelegate.cpp \
    src/pvDelegateImpl.cpp \
    src/pvHoldDelegate.cpp \
    src/pvHoldDelegateImpl.cpp \
    src/pvVariable.cpp \
    src/pvVariableImpl.cpp \
    src/stateMachine.cpp \
    src/stateMachineImpl.cpp \
    src/tangoFactoryImpl.cpp \
    src/tangoInterfaceImpl.cpp \
    src/logStreamGetterImpl.cpp

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
    include/nds3/pvDelegate.h \
    include/nds3/pvHoldDelegate.h \
    include/nds3/pvVariable.h \
    include/nds3/stateMachine.h \
    src/epicsFactoryImpl.h \
    src/dataAcquisitionImpl.h \
    src/epicsInterfaceImpl.h \
    src/factoryBaseImpl.h \
    src/interfaceBaseImpl.h \
    src/nodeImpl.h \
    src/portImpl.h \
    src/pvBaseImpl.h \
    src/pvDelegateImpl.h \
    src/pvHoldDelegateImpl.h \
    src/pvVariableImpl.h \
    src/stateMachineImpl.h \
    src/tangoFactoryImpl.h \
    src/tangoInterfaceImpl.h \
    src/baseImpl.h \
    src/logStreamGetterImpl.h \
    doc/groups.h

