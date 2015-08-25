TEMPLATE = app
QT -= qt
QT += console

QMAKE_CXXFLAGS += -std=c++0x -O0 -rdynamic -fvisibility=hidden -fvisibility-inlines-hidden

DEFINES += GOINGTHERE_DLL
DEFINES += GOINGTHERE_DLL_EXPORTS

LIBS += -L/opt/codac-5.0/epics/base/lib/linux-x86_64 \
    -L/opt/codac-5.0/epics/modules/asyn/lib/linux-x86_64 \
    -ldbCore -ldbRecStd -lgdd -lasyn \
    -lca -lcas -ldl -pthread

INCLUDEPATH += /opt/codac-5.0/epics/base/include \
    /opt/codac-5.0/epics/base/include/compiler/gcc \
    /opt/codac-5.0/epics/base/include/os/Linux/ \
    /opt/codac-5.0/epics/modules/asyn/include \


SOURCES += main.cpp \
    library/implementation/ndsbaseimpl.cpp \
    library/implementation/ndsnodeimpl.cpp \
    library/implementation/ndsbase.cpp \
    library/implementation/ndsnode.cpp \
    library/implementation/ndsport.cpp \
    library/implementation/ndsportimpl.cpp \
    library/implementation/ndsinterfaceimpl.cpp \
    library/implementation/ndspvbaseimpl.cpp \
    library/implementation/ndspvbase.cpp \
    library/implementation/ndsfactoryimpl.cpp \
    library/implementation/scansymbols.cpp \
    library/implementation/ndspvdelegateimpl.cpp \
    library/implementation/ndspvdelegate.cpp \
    library/implementation/ndsdelegate.cpp \
    library/implementation/ndspvholddelegateimpl.cpp \
    library/implementation/ndspvholddelegate.cpp \
    library/implementation/ndsfactory.cpp \
    library/implementation/ndsstatemachine.cpp \
    library/implementation/ndsstatemachineimpl.cpp \
    library/implementation/ndsexceptions.cpp

HEADERS += \
    library/implementation/ndsbaseimpl.h \
    library/implementation/ndsnodeimpl.h \
    library/implementation/ndsportImpl.h \
    library/implementation/ndspvbaseimpl.h \
    library/implementation/ndspvimpl.h \
    library/implementation/ndsinterfaceimpl.h \
    library/include/nds3/ndsbase.h \
    library/include/nds3/ndsnode.h \
    library/include/nds3/ndsport.h \
    library/include/nds3/ndspvbase.h \
    library/include/nds3/ndspvdelegate.h \
    library/implementation/ndsfactoryimpl.h \
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
    library/include/nds3/ndsexceptions.h

