#ifndef REFLECTOR_H
#define REFLECTOR_H

#include "utils.hpp"
#include <string>

using namespace std;

class Reflector{
  int mappings[NUM_LETTERS_IN_ALPHABET];

  /* Set all connections to VALUE_NOT_SET so it can later be detected if they
    have been set */
  void setMappingsToNotSet();

  /* Set mapping from config file */
  void setMappingsFromFile(string configFname);

  /* Reads in int pairs from ifstream and maps them to each other */
  void parseConfigFile(ifstream& inputStream, string errorLocation,
                      int& numMappingsReadIn);

  /* Checks NUM_LETTERS_IN_ALPHABET ints have been read in */
  void checkCorrectNumParamsReadIn(int numMappingsReadIn, string errorLocation);

public:
  /*Reflector contructor- takes config filename as a parameter
    Config file should be of the form 13 index pairs which will be mapped to
    each other
    See reflectors/I.rf for an example*/
  Reflector(string configFname);

  /* Returns reflector mapping of an index */
  int getMapping(const int index);

  /* Outputs a a visual representation of the current state of the plugboard to
    cout */
  void print();
};

#endif
