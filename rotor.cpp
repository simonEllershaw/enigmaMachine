#include "rotor.hpp"
#include "errors.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <exception>


//////////////////////// Public Functions /////////////////////////////////////

Rotor::Rotor(const char* configFname, const int startingPosition){
  std::ifstream inputStream;

  // Non config file dependent setting functions
  positionAtOrigin = startingPosition;
  setMappingsToNotSet();

  // Config file dependent setting functions
  inputStream.open(configFname);
  if(inputStream.fail())
    throw ERROR_OPENING_CONFIGURATION_FILE;
  setMappingsFromFile(inputStream);
  setNotchesFromFile(inputStream);
  inputStream.close();
}

int Rotor::getMapping(const int index){
  return mappings[(index + positionAtOrigin) % NUM_LETTERS_IN_ALPHABET];
}

bool Rotor::aNotchIsAtOrigin(){
  return notches[positionAtOrigin];
}

void Rotor::rotateRotor(){
  positionAtOrigin = (positionAtOrigin + 1) % NUM_LETTERS_IN_ALPHABET; // Not sure if correct direction
}


//////////////////////////// Private Functions ////////////////////////////////

void Rotor::setMappingsToNotSet(){
  for(int position = 0; position < NUM_LETTERS_IN_ALPHABET; position ++)
    mappings[position] = VALUE_NOT_SET;
  }

//Comments need updating

/* Set mapping from config file */
void Rotor::setMappingsFromFile(std::ifstream& inputStream){
  int mapping, index;
  bool prevSetAsOutput[26] = {0};


  // Get pairs of ints from file these are the two indicies to be mapped to
  // each other
  for(index = 0; index < NUM_LETTERS_IN_ALPHABET; index++){
    // Read in next indicies
    mapping = getNextInt(inputStream);
    // Check input mappings within range 0-25
    if(mapping < 0 || mapping >= NUM_LETTERS_IN_ALPHABET)
      throw INVALID_INDEX;
    // Check not mapping index to itself and that it has not been previously
    // assigned (the current value at the index is not the defaultMappingValue)
    if(prevSetAsOutput[mapping]){
          throw INVALID_ROTOR_MAPPING;
        }

    // Update mappings
    mappings[index] = mapping;
    prevSetAsOutput[mapping] = true;


    }

  // If this test passes then as no repeated mappings are allowed all indicies
  // must have been mapped
  if(inputStream.fail()){
    throw INVALID_ROTOR_MAPPING;
  }
}

void Rotor::setNotchesFromFile(std::ifstream& inputStream){
  int notchPosition;

  notchPosition = getNextInt(inputStream);

  // Get pairs of ints from file these are the two indicies to be mapped to
  // each other
  while(!inputStream.fail()){

    // Check input mappings within range 0-25
    if(notchPosition < 0 || notchPosition >= NUM_LETTERS_IN_ALPHABET)
      throw INVALID_INDEX;
    // Check not mapping index to itself and that it has not been previously
    // assigned (the current value at the index is not the defaultMappingValue)
    if(notches[notchPosition]){
          throw INVALID_ROTOR_MAPPING;
        }

    // Update notches
    notches[notchPosition] = true;

    // Read in next indicies
    notchPosition = getNextInt(inputStream);
    }
}



// Interim test programe
int main(){
  try{
    Rotor test = Rotor("testFiles/rotors/asdasd.rot", 0);
    for(int i = 0; i < NUM_LETTERS_IN_ALPHABET; i ++)
      std::cout << i << " " << test.getMapping(i) << std::endl;
  }
  catch(int e){
    std::cout << "Caught an exception " << e;
  }
  return 0;
}
