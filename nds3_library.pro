TEMPLATE = lib
QT -= qt
CONFIG += dll
TARGET = nds3

# Target that includes the dbd file into a cpp file
# Define how to create dbd.h
dbd.target = dbd.h
dbd.commands = echo \'static const char dbdfile[] = {\' > $$PWD/library/dbd/dbdfile.h && \
               xxd -i < $$PWD/library/dbd/dbdfile.dbd >> $$PWD/library/dbd/dbdfile.h && \
               echo \'};\' >> $$PWD/library/dbd/dbdfile.h
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
    library/src/ndsbaseimpl.cpp \
    library/src/ndsnodeimpl.cpp \
    library/src/ndsbase.cpp \
    library/src/ndsnode.cpp \
    library/src/ndsport.cpp \
    library/src/ndsportimpl.cpp \
    library/src/ndspvbaseimpl.cpp \
    library/src/ndspvbase.cpp \
    library/src/scansymbols.cpp \
    library/src/ndspvdelegateimpl.cpp \
    library/src/ndspvdelegate.cpp \
    library/src/ndsdelegate.cpp \
    library/src/ndspvholddelegateimpl.cpp \
    library/src/ndspvholddelegate.cpp \
    library/src/ndsfactory.cpp \
    library/src/ndsstatemachine.cpp \
    library/src/ndsstatemachineimpl.cpp \
    library/src/ndsexceptions.cpp \
    library/src/ndsepicsinterfaceimpl.cpp \
    library/src/ndsinterfacebaseimpl.cpp \
    library/src/ndsdataacquisitionimpl.cpp \
    library/src/ndspvvariableimpl.cpp \
    library/src/ndspvvariable.cpp \
    library/src/ndsdataacquisition.cpp \
    library/src/ndsepicsfactoryimpl.cpp \
    library/src/ndsfactorybaseimpl.cpp \
    library/src/ndstangofactoryimpl.cpp \
    library/src/ndstangointerfaceimpl.cpp

HEADERS += \
    library/src/scansymbols.h \
    library/include/nds3/definitions.h \
    library/include/nds3/nds.h \
    library/include/nds3/base.h \
    library/include/nds3/dataAcquisition.h \
    library/include/nds3/delegate.h \
    library/include/nds3/exceptions.h \
    library/include/nds3/factory.h \
    library/include/nds3/node.h \
    library/include/nds3/port.h \
    library/include/nds3/pvBase.h \
    library/include/nds3/pvDelegate.h \
    library/include/nds3/pvHoldDelegate.h \
    library/include/nds3/pvVariable.h \
    library/include/nds3/stateMachine.h \
    library/src/epicsFactoryImpl.h \
    library/src/dataAcquisitionImpl.h \
    library/src/epicsInterfaceImpl.h \
    library/src/factoryBaseImpl.h \
    library/src/interfaceBaseImpl.h \
    library/src/nodeImpl.h \
    library/src/portImpl.h \
    library/src/pvBaseImpl.h \
    library/src/pvDelegateImpl.h \
    library/src/pvHoldDelegateImpl.h \
    library/src/pvVariableImpl.h \
    library/src/stateMachineImpl.h \
    library/src/tangoFactoryImpl.h \
    library/src/tangoInterfaceImpl.h \
    library/src/baseImpl.h

