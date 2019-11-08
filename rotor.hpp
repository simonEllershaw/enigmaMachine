#ifndef ROTOR_H
#define ROTOR_H

#include "utils.hpp"
#include <fstream>
#include <string>

class Rotor{
  int mappings[NUM_LETTERS_IN_ALPHABET];
  bool notches[NUM_LETTERS_IN_ALPHABET] = {0};
  int positionAtOrigin = VALUE_NOT_SET;

  /* Set all mappings values to have a INDEX_NOT_SET */
  void setMappingsToNotSet();

  /* Set mapping from config filestream */
  void setMappingsFromFStream(std::ifstream& inputStream,
                              std::string errorLocation);

  /* Set notch values from config filestream */
  void setNotchesFromFStream(std::ifstream& inputStream,
                            std::string errorLocation);

  /* Returns the indexed accounting for rotor overflow e.g. -1->25 and 26->0 */
  int mapOverflow(int index);


public:
  /*Rotor contructor- takes config.rot filename as a parameter
  * Config file should be of the form: index mapping x13 notch1 notch2 .. notchN
  * See rotors/I.rot for an example
  */
  Rotor(std::string configFname, const int startingPosition);

  /* Returns rotor mapping of an index */
  int getForwardMapping(const int index);

  int getBackwardMapping(const int mapping);


  /* Returns true if a notch is at the origin position, otherwise false */
  bool aNotchIsAtOrigin();

  /* Implements rotor rotation by updating indexAtOrigin */
  void rotateRotor();

  void print();
};

#endif
