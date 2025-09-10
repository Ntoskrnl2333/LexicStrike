CXX = g++
CXXFLAGS = -std=c++11 -O2

all: lexic-strike

clean:
	rm lexic-strike

lexic-strike: main.cpp
	$(CXX) $^ -o $@ $(CXXFLAGS) 

install: lexic-strike
	sudo cp lexic-strike /usr/local/bin/

uninstall:
	sudo rm /usr/local/bin/lexic-strike

.PHONY: install uninstall clean all
