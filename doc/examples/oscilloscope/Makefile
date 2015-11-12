CXX = g++
 
CXXFLAGS = -std=c++0x -Wall -Wextra -pedantic -fPIC -pthread -DNDS3_DLL
 
# Flags passed to gcc during linking
LINK = -shared -fPIC -Wl,-as-needed
 
# Name of the nds library
TARGET = liboscilloscope.so
 
# Additional linker libraries
LIBS = -lnds3
 
# Source code files used in this project
SRCS = oscilloscope.cpp 
 
OBJS = $(SRCS:.cpp=.o)
# Rules for building
$(TARGET): $(OBJS)
	$(CXX) $(LINK) -o $@ $^ $(LIBS)
 
.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
 
