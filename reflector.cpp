#include "reflector.hpp"
#include "errors.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <iomanip>



Reflector::Reflector(const char* configFname){
  setMappingsToNotSet();
  setMappingsFromFile(configFname);
}

void Reflector::setMappingsToNotSet(){
  for(int position = 0; position < NUM_LETTERS_IN_ALPHABET; position ++)
    mappings[position] = VALUE_NOT_SET;
}

void Reflector::setMappingsFromFile(const char* configFname){
  std::ifstream inputStream;
  int index1, index2, numMappingsReadIn;

  inputStream.open(configFname);
  if(inputStream.fail())
    throw ERROR_OPENING_CONFIGURATION_FILE;

  index1 = getNextInt(inputStream);
  index2 = getNextInt(inputStream);

  // Get pairs of ints from file these are the two indicies to be mapped to
  // each other
  for(numMappingsReadIn = 0; !inputStream.fail(); numMappingsReadIn++){
    // Check input mappings within range 0-25
    if(index1 < 0 || index1 >= NUM_LETTERS_IN_ALPHABET ||
      index2 < 0 || index2 >= NUM_LETTERS_IN_ALPHABET)
      throw INVALID_INDEX;
    // Check not mapping index to itself and that it has not been previously
    // assigned (the current value at the index is not the defaultMappingValue)
    if(index1 == index2 || mappings[index1] != VALUE_NOT_SET ||
        mappings[index2] != VALUE_NOT_SET){
          throw INVALID_REFLECTOR_MAPPING;
        }
    // Update mapping
    mappings[index1] = index2;
    mappings[index2] = index1;

    // Read in next indicies
    index1 = getNextInt(inputStream);
    index2 = getNextInt(inputStream);
    }

  // If this test passes then as no repeated mappings are allowed all indicies
  // must have been mapped
  if(numMappingsReadIn!=(NUM_LETTERS_IN_ALPHABET/2)){
    throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
  }

  inputStream.close();
}



int Reflector::getMapping(const int index){
  return mappings[index];
}

void Reflector::print(){
  std::cout << std::setw(MAPPING_INDENT) << std::left << "Reflector: ";
  for(int i = 0; i < NUM_LETTERS_IN_ALPHABET; i ++)
    std::cout << std::setw(DIGIT_SPACING) << std::right << getMapping(i);
  std::cout << std::endl;
}
