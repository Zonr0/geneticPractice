CXX=clang++
CXXFLAGS=-std=gnu++11 -g -Wall

all: genetic.out

debug: CXXFLAGS += -DDEBUG -g
debug: genetic.out

genetic.out: main.cpp Chromosome.o Postfix.o
	 $(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm *.o
