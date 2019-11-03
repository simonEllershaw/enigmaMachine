#include "reflector.hpp"
#include "errors.h"
#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <exception>


Reflector::Reflector(const char* configFname){
  setDefaultMappings();
  setMappingsFromFile(configFname);
}

void Reflector::setDefaultMappings(){
  for(int position = 0; position < NUM_LETTERS_IN_ALPHABET; position ++)
    connections[position] = defaultMappingValue;
}

void Reflector::setMappingsFromFile(const char* configFname){
  std::ifstream inputStream;
  int index1, index2, numMappingsReadIn;

  inputStream.open(configFname);
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
    if(index1 == index2 || connections[index1] != defaultMappingValue ||
        connections[index2] != defaultMappingValue){
          throw INVALID_REFLECTOR_MAPPING;
        }
    // Update mapping
    connections[index1] = index2;
    connections[index2] = index1;

    index1 = getNextInt(inputStream);
    index2 = getNextInt(inputStream);
    }

  if(numMappingsReadIn!=(NUM_LETTERS_IN_ALPHABET/2)){
    throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
  }

  inputStream.close();
}



int Reflector::get_mapping(const int index){
  return connections[index];
}


// Interim test programe
int main(){
  try{
    Reflector test = Reflector("reflectors/nonNumeric.rf");
    for(int i = 0; i < NUM_LETTERS_IN_ALPHABET; i ++)
      std::cout << i << " " << test.get_mapping(i) << std::endl;
  }
  catch(int e){
    std::cout << "Caught an exception " << e;
  }
}
