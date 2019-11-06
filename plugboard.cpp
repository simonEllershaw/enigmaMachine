#include "plugboard.hpp"
#include "errors.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <exception>


Plugboard::Plugboard(const char* configFname){
  setDefaultMappings();
  setMappingsFromFile(configFname);
}


void Plugboard::setDefaultMappings(){
  // Default mapping is each index to itself
  for(int index = 0; index < NUM_LETTERS_IN_ALPHABET; index ++)
    mappings[index] = index;
}


void Plugboard::setMappingsFromFile(const char* configFname){
  std::ifstream inputStream;
  int mapping, index;

  inputStream.open(configFname);
  if(inputStream.fail())
    throw ERROR_OPENING_CONFIGURATION_FILE;

  // Get pairs of ints from file the first is the index and the second the
  // new mapping of that index
  while(!inputStream.fail()){
    index = getNextInt(inputStream);
    // If EOF here then an even number of ints have been parsed so break without
    // error
    if(inputStream.eof())
      break;

    mapping = getNextInt(inputStream);
    // If EOF here an odd number of ints have been parsed so throw exception
    if(inputStream.eof())
      throw INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;

    // Check input mappings within range 0-25
    if(index < 0 || index >= NUM_LETTERS_IN_ALPHABET ||
      mapping < 0 || mapping >= NUM_LETTERS_IN_ALPHABET)
      throw INVALID_INDEX;
    // Check not mapping index to itself and that it has not been previously
    // been set
    if(index == mapping || mappings[index] != index)
      throw IMPOSSIBLE_PLUGBOARD_CONFIGURATION;

    // Update mapping
    mappings[index] = mapping;
    }

  inputStream.close();
}


int Plugboard::getMapping(const int index){
  return mappings[index];
}
