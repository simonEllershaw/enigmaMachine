enigma: enigma.o rotor.o reflector.o plugboard.o utils.o
	g++ enigma.o rotor.o reflector.o utils.o plugboard.o -o enigma

test: test.o enigma.o rotor.o reflector.o plugboard.o utils.o
	g++ test.o enigma.o rotor.o reflector.o utils.o plugboard.o -o test.exe





%.o: %.cpp
	g++ -Wall -g -c $<

plugboard.o: plugboard.hpp utils.hpp errors.h

rotor.o: rotor.hpp utils.hpp errors.h

reflector.o: reflector.hpp utils.hpp errors.h

utils.o: utils.hpp errors.h

test.o: test.cpp utils.hpp

enigma.o: enigma.hpp

clean:
	rm -f *.o *.exe

.PHONY: clean
