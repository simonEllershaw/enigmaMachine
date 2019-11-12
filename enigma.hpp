#ifndef ENIGMA_H
#define ENIGMA_H

#include "rotor.hpp"
#include "plugboard.hpp"
#include "reflector.hpp"
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class Enigma{
  vector<Rotor*> rotorVector;
  Plugboard* plugboard = nullptr;
  Reflector* reflector = nullptr;

  /* Creates rotors using cmd line config files and adds to rotorVector */
  void setRotorVector(string rotorPositionsFname,
                    vector<string> rotorFnames);


   /* Rotates leftmost rotor 1 position, updates rotors to the right if notch at
      the top */
  void rotateRotors();

  /* Returns a single encoded char, also updates internal state of the Enigma
    machine e.g. rotates rotors */
  char encodeChar(char input);

  public:

  /* Creates Enigma object using files given on cmd line */
  Enigma(string plugboardFname, string reflectorFname,
        vector<string> rotorFnames, string rotorPositionsFname);

  /* Takes a line from cin and outputs encrypted message to cout */
  void IOInterface();

  /* Outputs text representation of the state of the enigma machine to cout*/
  void print();

  ~Enigma();
};

/* Updates rotorFnames vector with the rotor filenames supplied on the cmd line*/
void getRotorFnamesFromCmdLine(vector<string>& rotorFnames, int argc,
                              char**argv);

#endif
