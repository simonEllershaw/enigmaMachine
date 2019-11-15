#ifndef ENIGMA_H
#define ENIGMA_H

#include "rotor.hpp"
#include "plugboard.hpp"
#include "reflector.hpp"
#include <vector>
#include <fstream>
#include <string>

class Enigma{
  std::vector<Rotor*> rotorVector;
  Plugboard* plugboard = nullptr;
  Reflector* reflector = nullptr;

  /* Creates rotors using cmd line config files and adds to rotorVector */
  void setRotorVector(std::string rotorPositionsFname,
                    std::vector<std::string> rotorFnames);


   /* Rotates leftmost rotor 1 position, updates rotors to the right if notch at
      the top */
  void rotateRotors();

  /* Returns a single encoded char, also updates internal state of the Enigma
    machine e.g. rotates rotors */
  char encodeChar(char input);

  public:

  /* Creates Enigma object using files given on cmd line */
  Enigma(std::string plugboardFname, std::string reflectorFname,
        std::vector<std::string> rotorFnames, std::string rotorPositionsFname);

  /* Takes a line from cin and outputs encrypted message to cout */
  void IOInterface();

  /* Outputs text representation of the state of the enigma machine to cout*/
  void print();

  ~Enigma();
};

/* Updates rotorFnames std::vector with the rotor filenames supplied on the cmd line*/
void getRotorFnamesFromCmdLine(std::vector<std::string>& rotorFnames, int argc,
                              char**argv);

#endif
