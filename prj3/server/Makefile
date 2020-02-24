CXX=g++

CFLAGS=-c -Wall -std=c++14
OFLAGS=-O2
LDFLAGS=-lpthread
DFLAGS=-DNDEBUG
INCLUDES=
SOURCES=server.cpp sampo.cpp netfunc.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=server-pervak

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
# Linking
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@
.cpp.o:
# Build
	$(CXX) $(CFLAGS) $<  $(DFLAGS) $(INCLUDES) $(OFLAGS) -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
