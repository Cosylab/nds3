CXX = g++
RM = rm
CP = cp
 
CXXFLAGS = -std=c++0x -Wall -Wextra -pedantic -fPIC -pthread -fvisibility=hidden -fvisibility-inlines-hidden
 
# Flags passed to gcc during linking
LINK = -shared -fPIC -Wl,-as-needed
 
# Name of the nds library
TARGET = nds3.so
 
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
	$(CP) $(TARGET) /usr/lib
	$(CP) -r include/nds3 /usr/include
	$(CP) -r include/nds3impl /usr/include

.PHONY: revision
revision:
	bumprev $(VERSION) $(TARGET)
