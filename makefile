.PHONY:all run clean

# CPP := $(wildcard *.cpp)
# HPP := $(wildcard *.hpp)
CPP := $(wildcard *.cpp)
HPP := $(wildcard *.hpp)
CC  := g++


all: main run


main:$(CPP) $(HPP)
	@$(CC) -g --std=c++14 $(CPP) -o $@

run: main
	@./main

clean:
	rm -rf main