#ifndef PLUGBOARD_H
#define PLUGBOARD_H

#include "utils.hpp"
#include <string>

class Plugboard{
  int mappings[NUM_LETTERS_IN_ALPHABET];

  /* Set all connections to VALUE_NOT_SET so it can later be checked if a
    connection has been set */
  void setDefaultMappings();

  /* Set mappings from config file */
  void setMappingsFromFile(std::string configFname);

public:
  /*Plugboard contructor- takes config filename as a parameter
  Config file should be of the form: index mapping etc...
  See plugboards/I.pb for an example*/
  Plugboard(std::string configFname);

  /* Returns plugboard mapping of an index */
  int getForwardMapping(const int index);

  /* Returns plugboard index from a mapping */
  int getBackwardMapping(const int mapping);

  /* Outputs a a visual representation of the current state of the plugboard to
    cout */
  void print();
};

#endif
