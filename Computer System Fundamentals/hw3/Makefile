CPP_SRCS = main.cpp Cache.cpp Block.cpp Set.cpp
CFLAGS = -g -Wall -Wextra -pedantic -std=c++11


%.o : %.cpp
	g++ $(CFLAGS) -c $<

all : csim

csim : main.o Cache.o Set.o Block.o
	g++ -o $@ main.o Cache.o Set.o Block.o

.PHONY: solution.zip
solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile *.h *.cpp README.md

clean :
	rm -f *.o csim depend.mak solution.zip

depend.mak :
	touch $@

depend :
	g++ -M $(CPP_SRCS) > depend.mak

include depend.mak
