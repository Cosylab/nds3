CXX = g++
RM = rm
INSTALL = install

CXXFLAGS = -std=c++0x -Wall -Wextra -pedantic -fPIC -pthread
CXXFLAGS += -fvisibility=hidden -fvisibility-inlines-hidden -DNDS3_DLL
CXXFLAGS += -DNDS3_DLL_EXPORTS -Iinclude

# make install PREFIX=/usr to override
PREFIX ?= /usr/local

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

# Rules for building library
$(TARGET): $(OBJS)
	$(CXX) $(LINK) -o $@ $^ $(LIBS)

INSTALL_SHLIB=$(addprefix $(PREFIX)/lib64/,$(TARGET))

$(INSTALL_SHLIB): $(TARGET)
	$(INSTALL) -D -m 0755 $^ $@
	ldconfig -n $(@D)

INSTALL_INCLUDES=$(addprefix $(PREFIX)/,$(wildcard include/nds3/*.h) $(wildcard include/nds3/impl/*.h))

$(PREFIX)/%.h: %.h
	install -D -m 0644 $^ $@

.PHONY: clean
clean:
	$(RM) -f $(TARGET) $(OBJS)
	$(RM) -rf doc/api/hlatex doc/api/latex doc/api/html
 
.PHONY: install
install: $(INSTALL_SHLIB) $(INSTALL_INCLUDES)

.PHONY: revision
revision:
	bumprev $(VERSION) $(TARGET)

.PHONY: doc
doc: doc/api/html doc/api/hlatex/refman.pdf

doc/api/html: doc/api/DoxyfileHTML
	cd doc/api && doxygen DoxyfileHTML

doc/api/hlatex/refman.pdf: doc/api/DoxyfilePDFH
	cd doc/api && doxygen DoxyfilePDFH
	make -C doc/api/hlatex refman.pdf
