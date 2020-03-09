.PHONY: all clean debug release

all: debug

release: longnums

debug: longnums-dbg

longnums: main.cpp superlong.h
	g++ main.cpp -O2 -Wall -std=c++11 -DNDEBUG -o longnums

longnums-dbg:  main.cpp superlong.h
	g++ main.cpp -g -O0 -Wall -std=c++11 -o longnums-dbg
	
clean:
	rm -f longnums longnums-dbg 
	