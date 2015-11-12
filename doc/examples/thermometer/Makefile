CXX = g++

# We enable the c++0x flag, plus we define the NDS3_DLL preprocessor macro.
CXXFLAGS = -std=c++0x -Wall -Wextra -pedantic -fPIC -pthread -DNDS3_DLL

# Flags passed to gcc during linking
LINK = -shared -fPIC -Wl,-as-needed

# Name of our device library
TARGET = libthermometer.so
 
# We specify that we depend on NDS3
LIBS = -lnds3
 
# We assume that all the .cpp files are in the src folder
SRCS = thermometer.cpp

OBJS = $(SRCS:.cpp=.o)
# Rules for building
$(TARGET): $(OBJS)
	$(CXX) $(LINK) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
