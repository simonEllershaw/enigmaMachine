#ifndef REFLECTOR_H
#define REFLECTOR_H

#include "utils.hpp"
#include <string>

class Reflector{
  int mappings[NUM_LETTERS_IN_ALPHABET];

  /* Set all connections to have a value of -1 so it can be detected if they
    have been set or not later (as -1 is an invalid mapping) */
  void setMappingsToNotSet();

  /* Set mapping from config file */
  void setMappingsFromFile(std::string configFname);
public:
  /*Reflector contructor- takes config filename as a parameter
  Config file should be of the for index pa*/
  Reflector(std::string configFname);

  /* Returns reflector mapping of an index */
  int getMapping(const int index);

  void print();
};

#endif
