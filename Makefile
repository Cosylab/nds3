CXX = g++
RM = rm
CP = cp
 
CXXFLAGS = -std=c++0x -Wall -Wextra -pedantic -fPIC -pthread -fvisibility=hidden -fvisibility-inlines-hidden -DNDS3_DLL -DNDS3_DLL_EXPORTS
CXXFLAGS += -Iinclude

debug: CXXFLAGS += -DDEBUG -g
debug: libnds3.so

# Flags passed to gcc during linking
LINK = -shared -fPIC -Wl,-as-needed
 
# Name of the nds library
TARGET = libnds3.so
 
# Additional linker libraries
LIBS = -lrt -ldl -lpthread
 
# Version of the library to build
VERSION = 3
 
# Source code files used in this project
SRCS = $(wildcard src/*.cpp)
 
OBJS = $(SRCS:.cpp=.o)

# Rules for building
$(TARGET): $(OBJS)
	$(CXX) $(LINK) -o $@ $^ $(LIBS)
 
.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
 
.PHONY: install
install: $(TARGET)
	$(CP) $(TARGET) /usr/lib64
	chmod -R 0755 /usr/lib64/libnds3.so
	$(CP) -r include/nds3 /usr/include
	chmod -R 0755 /usr/include/nds3
	$(CP) -r include/nds3impl /usr/include
	chmod -R 0755 /usr/include/nds3impl
	ldconfig -n /usr/lib

.PHONY: revision
revision:
	bumprev $(VERSION) $(TARGET)

.PHONY: doc
doc: doc/api/html

doc/api/html:
	cd doc/api && doxygen DoxyfileHTML
