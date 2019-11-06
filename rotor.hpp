#ifndef ROTOR_H
#define ROTOR_H

#include "utils.hpp"
#include <fstream>

class Rotor{
  int mappings[NUM_LETTERS_IN_ALPHABET];
  bool notches[NUM_LETTERS_IN_ALPHABET] = {0};
  int positionAtOrigin = VALUE_NOT_SET;

  /* Set all mappings values to have a INDEX_NOT_SET */
  void setMappingsToNotSet();

  /* Set mapping from config filestream */
  void setMappingsFromFile(std::ifstream& inputStream);

  /* Set notch values from config filestream */
  void setNotchesFromFile(std::ifstream& inputStream);


public:
  /*Rotor contructor- takes config.rot filename as a parameter
  * Config file should be of the form: index mapping x13 notch1 notch2 .. notchN
  * See rotors/I.rot for an example
  */
  Rotor(const char* configFname, const int startingPosition);

  /* Returns rotor mapping of an index */
  int getFowardMapping(const int index);

  int getBackwardMapping(const int mapping);


  /* Returns true if a notch is at the origin position, otherwise false */
  bool aNotchIsAtOrigin();

  /* Implements rotor rotation by updating indexAtOrigin */
  void rotateRotor();
};

#endif
