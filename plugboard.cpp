#include "plugboard.hpp"
#include "errors.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <iomanip>


Plugboard::Plugboard(std::string configFname){
  setDefaultMappings();
  setMappingsFromFile(configFname);
}


void Plugboard::setDefaultMappings(){
  // Default mapping is each index to itself
  for(int index = 0; index < NUM_LETTERS_IN_ALPHABET; index ++)
    mappings[index] = index;
}


void Plugboard::setMappingsFromFile(std::string configFname){
  std::ifstream inputStream;
  int mapping, index;
  std::string errorLocation = "plugboard file: " + configFname;

  inputStream.open(configFname);
  if(inputStream.fail())
    throw ERROR_OPENING_CONFIGURATION_FILE;

  // Get pairs of ints from file the first is the index and the second the
  // new mapping of that index
  while(!inputStream.fail()){
    index = getNextInt(inputStream, errorLocation);
    // If EOF here then an even number of ints have been parsed so break without
    // error
    if(inputStream.eof())
      break;

    mapping = getNextInt(inputStream, errorLocation);
    // If EOF here an odd number of ints have been parsed so throw exception
    if(inputStream.eof()){
      std::cout << "Incorrect number of parameters in " << errorLocation;
      throw INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
    }
    
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
    mappings[mapping] = index;
    }

  inputStream.close();
}


int Plugboard::getForwardMapping(const int index){
  return mappings[index];
}

int Plugboard::getBackwardMapping(const int mapping){
  int index = 0;
  for(; mappings[index] != mapping; index++);
  return index;
}

void Plugboard::print(){
  std::cout << std::setw(MAPPING_INDENT) << std::left << "Plugboard: ";
  for(int i = 0; i < NUM_LETTERS_IN_ALPHABET; i ++)
    std::cout << std::setw(DIGIT_SPACING) << std::right << getForwardMapping(i);
  std::cout << std::endl;
}
