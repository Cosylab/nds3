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
    library/implementation/ndsbaseimpl.cpp \
    library/implementation/ndsnodeimpl.cpp \
    library/implementation/ndsbase.cpp \
    library/implementation/ndsnode.cpp \
    library/implementation/ndsport.cpp \
    library/implementation/ndsportimpl.cpp \
    library/implementation/ndspvbaseimpl.cpp \
    library/implementation/ndspvbase.cpp \
    library/implementation/scansymbols.cpp \
    library/implementation/ndspvdelegateimpl.cpp \
    library/implementation/ndspvdelegate.cpp \
    library/implementation/ndsdelegate.cpp \
    library/implementation/ndspvholddelegateimpl.cpp \
    library/implementation/ndspvholddelegate.cpp \
    library/implementation/ndsfactory.cpp \
    library/implementation/ndsstatemachine.cpp \
    library/implementation/ndsstatemachineimpl.cpp \
    library/implementation/ndsexceptions.cpp \
    library/implementation/ndsepicsinterfaceimpl.cpp \
    library/implementation/ndsinterfacebaseimpl.cpp \
    library/implementation/ndsdataacquisitionimpl.cpp \
    library/implementation/ndspvvariableimpl.cpp \
    library/implementation/ndspvvariable.cpp \
    library/implementation/ndsdataacquisition.cpp \
    library/implementation/ndsepicsfactoryimpl.cpp \
    library/implementation/ndsfactorybaseimpl.cpp \
    library/implementation/ndstangofactoryimpl.cpp \
    library/implementation/ndstangointerfaceimpl.cpp

HEADERS += \
    library/implementation/ndsbaseimpl.h \
    library/implementation/ndsnodeimpl.h \
    library/implementation/ndsportImpl.h \
    library/implementation/ndspvbaseimpl.h \
    library/include/nds3/ndsbase.h \
    library/include/nds3/ndsnode.h \
    library/include/nds3/ndsport.h \
    library/include/nds3/ndspvbase.h \
    library/include/nds3/ndspvdelegate.h \
    library/implementation/scansymbols.h \
    library/implementation/ndspvdelegateimpl.h \
    library/implementation/ndspvdelegateimpl.h \
    library/include/nds3/nds3.h \
    library/include/nds3/definitions.h \
    library/include/nds3/ndsdelegate.h \
    library/include/nds3/ndspvholddelegate.h \
    library/implementation/ndspvholddelegateimpl.h \
    library/include/nds3/ndsfactory.h \
    library/include/nds3/ndsstatemachine.h \
    library/implementation/ndsstatemachineimpl.h \
    library/include/nds3/ndsexceptions.h \
    library/implementation/ndsepicsinterfaceimpl.h \
    library/implementation/ndsinterfacebaseimpl.h \
    library/include/nds3/ndsdataacquisition.h \
    library/implementation/ndsdataacquisitionimpl.h \
    library/implementation/ndspvvariableimpl.h \
    library/include/nds3/ndspvvariable.h \
    library/implementation/ndsepicsfactoryimpl.h \
    library/implementation/ndsfactorybaseimpl.h \
    library/implementation/ndstangofactoryimpl.h \
    library/implementation/ndstangointerfaceimpl.h

