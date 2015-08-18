TEMPLATE = app
QT -= qt
QT += console

QMAKE_CXXFLAGS += -std=c++0x -O0 -rdynamic

LIBS += -L/opt/codac-5.0/epics/base/lib/linux-x86_64 \
    -L/opt/codac-5.0/epics/modules/asyn/lib/linux-x86_64 \
    -ldbCore -ldbRecStd -lgdd -lasyn \
    -lca -lcas -ldl -pthread

LIBS += -L/opt/codac-5.0/epics/base/lib/linux-x86_64 \
    -L/opt/codac-5.0/epics/modules/asyn/lib/linux-x86_64 \
    -lca -lcas -lCom -ldbCore -ldbRecStd -lgdd -lasyn -lxml2 -lcurl \
    -lboost_system -lboost_filesystem \
    -L/opt/codac-5.0/epics/modules/nds/lib/linux-x86_64 \
    -lnisync -L/opt/codac-5.0/lib

INCLUDEPATH += /opt/codac-5.0/epics/base/include \
    /opt/codac-5.0/epics/base/include/compiler/gcc \
    /opt/codac-5.0/epics/base/include/os/Linux/ \
    /opt/codac-5.0/epics/modules/asyn/include \


#INCLUDEPATH += /opt/codac-5.0/epics/base/include \
#    /opt/codac-5.0/epics/base/include/compiler/gcc \
#    /opt/codac-5.0/epics/base/include/os/Linux/ \
#    /opt/codac-5.0/epics/modules/asyn/include \

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
    library/implementation/scansymbols.cpp

HEADERS += \
    library/implementation/ndsbaseimpl.h \
    library/implementation/ndsnodeimpl.h \
    library/implementation/ndsportImpl.h \
    library/implementation/ndspvbaseimpl.h \
    library/implementation/ndspvimpl.h \
    library/implementation/ndsdelegatepvimpl.h \
    library/implementation/ndsinterfaceimpl.h \
    library/include/nds3/ndsbase.h \
    library/include/nds3/ndsnode.h \
    library/include/nds3/ndsport.h \
    library/include/nds3/ndspvbase.h \
    library/include/nds3/ndspvdelegate.h \
    library/implementation/ndsfactoryimpl.h \
    library/implementation/scansymbols.h

