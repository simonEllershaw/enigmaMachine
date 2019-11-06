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

int Rotor::getFowardMapping(const int index){
  // Check index is betweeen 0-NUM_LETTERS_IN_ALPHABET
  if(index < 0 || index >= NUM_LETTERS_IN_ALPHABET)
    throw INVALID_INDEX;
  // Get mapping at index adjusted for position of rotor
  return mappings[(index + positionAtOrigin) % NUM_LETTERS_IN_ALPHABET]
          - positionAtOrigin;
}

int Rotor::getBackwardMapping(const int mapping){
  int index = 0;
  // Check mapping is betweeen 0-NUM_LETTERS_IN_ALPHABET
  if(mapping < 0 || mapping >= NUM_LETTERS_IN_ALPHABET)
    throw INVALID_INDEX;

  // Find index of mapping
  for(; mappings[index] != mapping + positionAtOrigin && index < NUM_LETTERS_IN_ALPHABET; index++);

  // Check an index is found
  if(index >= NUM_LETTERS_IN_ALPHABET)
    throw INVALID_ROTOR_MAPPING;

  // Account for position of rotor
  return (index - positionAtOrigin) % 26;
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

void Rotor::setMappingsFromFile(std::ifstream& inputStream){
  int mapping, index;
  bool prevSetAsOutput[26] = {0}; // Allows checking output not already mapped

  // Get 26 ints check the int is valid and set as rotor mapping
  for(index = 0; index < NUM_LETTERS_IN_ALPHABET; index++){
    mapping = getNextInt(inputStream);

    // Check input mappings within range 0-25
    if(mapping < 0 || mapping >= NUM_LETTERS_IN_ALPHABET)
      throw INVALID_INDEX;
    // Check mapping has not previously been set as output
    if(prevSetAsOutput[mapping]){
          throw INVALID_ROTOR_MAPPING;
        }

    // Update mappings
    mappings[index] = mapping;
    prevSetAsOutput[mapping] = true;
    }

  // If inputStream fails less than 26 ints present in .rot file
  if(inputStream.fail()){
    throw INVALID_ROTOR_MAPPING;
  }
}

void Rotor::setNotchesFromFile(std::ifstream& inputStream){
  int notchPosition;

  notchPosition = getNextInt(inputStream);

  // Get remaing ints from .rot file and assign as notch positions
  while(!inputStream.fail()){
    // Check input mappings within range 0-25
    if(notchPosition < 0 || notchPosition >= NUM_LETTERS_IN_ALPHABET)
      throw INVALID_INDEX;
    // Check notch only set once
    if(notches[notchPosition]){
          throw INVALID_ROTOR_MAPPING;
        }

    // Update notches
    notches[notchPosition] = true;

    // Read in next indicies
    notchPosition = getNextInt(inputStream);
    }
}
