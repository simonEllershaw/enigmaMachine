#ifndef REFLECTOR_H
#define REFLECTOR_H

#include "utils.hpp"

class Reflector{
  int mappings[NUM_LETTERS_IN_ALPHABET];

  /* Set all connections to have a value of -1 so it can be detected if they
    have been set or not later (as -1 is an invalid mapping) */
  void setMappingsToNotSet();

  /* Set mapping from config file */
  void setMappingsFromFile(const char* configFname);
public:
  /*Reflector contructor- takes config filename as a parameter
  Config file should be of the for index pa*/
  Reflector(const char* configFname);

  /* Returns reflector mapping of an index */
  int getMapping(const int index);
};

#endif
