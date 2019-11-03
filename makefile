plugboard: plugboard.o utils.o
	g++ plugboard.o utils.o -o plugboard

reflector: reflector.o utils.o
	g++ reflector.o utils.o -o reflector

%.o: %.cpp
	g++ -Wall -g -c $<

plugboard.o: plugboard.hpp utils.hpp errors.h

reflector.o: reflector.hpp utils.hpp errors.h

utils.o: utils.hpp errors.h

clean:
	rm -f *.o *.exe

.PHONY: clean
