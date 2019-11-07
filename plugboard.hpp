#ifndef PLUGBOARD_H
#define PLUGBOARD_H
#include "utils.hpp"

class Plugboard{
  int mappings[NUM_LETTERS_IN_ALPHABET];

  /* Set all connections to have a value of -1 so it can be detected if they
    have been set or not later (as -1 is an invalid mapping) */
  void setDefaultMappings();
  /* Set mapping from config file */
  void setMappingsFromFile(const char* configFname);

public:
  /*Plugboard contructor- takes config filename as a parameter
  Config file should be of the form: index mapping etc...
  See plugboards/I.pb for an example*/
  Plugboard(const char* configFname);

  /* Returns plugboard mapping of an index */
  int getForwardMapping(const int index);

  /* Returns plugboard index from an input mapping */
  int getBackwardMapping(const int mapping);

  void print();
};

#endif
