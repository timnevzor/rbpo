CXX=g++

CFLAGS=-c -std=c++11 -Wall
OFLAGS=-O2
LDFLAGS=
DFLAGS=-DNDEBUG
INCLUDES=
SOURCES=client.cpp getflag.cpp decryptflag.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=pervak

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
# Linking
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@
.cpp.o:
# Build
	$(CXX) $(CFLAGS) $< $(DFLAGS) $(INCLUDES) $(OFLAGS) -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
