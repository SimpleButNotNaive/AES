.PHONY:all run clean

# CPP := $(wildcard *.cpp)
# HPP := $(wildcard *.hpp)
CPP := aes.cpp main.cpp
HPP := $(wildcard *.hpp)
CC  := g++


all: run


main:$(CPP) $(HPP)
	@$(CC) -g --std=c++14 $(CPP) -o $@

run: main
	@./main

clean:
	rm -rf main