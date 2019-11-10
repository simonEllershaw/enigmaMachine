#include "rotor.hpp"
#include "errors.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <iomanip>



//////////////////////// Public Functions /////////////////////////////////////

Rotor::Rotor(std::string configFname, const int startingPosition){
  std::ifstream inputStream;
  std::string errorLocation = "for mapping in rotor file " + configFname;

  // Non config file dependent setting functions
  positionAtOrigin = startingPosition;
  setMappingsToNotSet();

  // Config file dependent setting functions
  inputStream.open(configFname);
  if(inputStream.fail()){
    printErrorMessage("Could not open " + configFname);
    throw ERROR_OPENING_CONFIGURATION_FILE;
  }
  setMappingsFromFStream(inputStream, errorLocation);
  setNotchesFromFStream(inputStream, errorLocation);
  inputStream.close();
}

int Rotor::getForwardMapping(const int index){
  // Check index is betweeen 0-NUM_LETTERS_IN_ALPHABET
  if(index < 0 || index >= NUM_LETTERS_IN_ALPHABET)
    throw INVALID_INDEX;

  int positionAdjustedIndex = mapOverflow(index + positionAtOrigin);
  // Get mapping at index adjusted for position of rotor
  return mapOverflow(mappings[positionAdjustedIndex] - positionAtOrigin);
}

int Rotor::getBackwardMapping(const int mapping){
  int index = 0;
  // Find index of mapping
  for(; mappings[index] != mapOverflow(mapping + positionAtOrigin)
      ; index++);
  // Account for position of rotor
  return mapOverflow(index - positionAtOrigin);
}

bool Rotor::aNotchIsAtOrigin(){
  return notches[positionAtOrigin];
}

void Rotor::rotateRotor(){
  positionAtOrigin = mapOverflow(positionAtOrigin + 1); // Not sure if correct direction
}

void Rotor::print(){
  std::cout << std::setw(MAPPING_INDENT) << std::left << "Rotor: ";
  for(int i = 0; i < NUM_LETTERS_IN_ALPHABET; i ++)
    std::cout << std::setw(DIGIT_SPACING) << std::right << mappings[i];
  std::cout << std::endl;
  std::cout << std::setw(MAPPING_INDENT) << std::left << "Notches: ";
  for(int i = 0; i < NUM_LETTERS_IN_ALPHABET; i ++)
    std::cout << std::setw(DIGIT_SPACING) << std::right << notches[i];
  std::cout << " Position at origin: " << positionAtOrigin << std::endl;
}


//////////////////////////// Private Functions ////////////////////////////////

int Rotor::mapOverflow(int index){
    return (index + NUM_LETTERS_IN_ALPHABET) % NUM_LETTERS_IN_ALPHABET;
}

void Rotor::setMappingsToNotSet(){
  for(int position = 0; position < NUM_LETTERS_IN_ALPHABET; position ++)
    mappings[position] = VALUE_NOT_SET;
  }

void Rotor::setMappingsFromFStream(std::ifstream& inputStream,
                                std::string errorLocation){
  int mapping, index;
  // Allows checking output not already mapped
  int outputMappings[NUM_LETTERS_IN_ALPHABET];

  for(index = 0; index < NUM_LETTERS_IN_ALPHABET; index++)
    outputMappings[index] = VALUE_NOT_SET;

  // Get 26 ints check the int is valid and set as rotor mapping
  for(index = 0; index < NUM_LETTERS_IN_ALPHABET; index++){
    mapping = getNextInt(inputStream, errorLocation);

    // Check input mappings within range 0-25
    if(mapping < 0 || mapping >= NUM_LETTERS_IN_ALPHABET)
      throw INVALID_INDEX;
    // Check mapping has not previously been set as output
    if(outputMappings[mapping] != VALUE_NOT_SET){
          printErrorMessage("Invalid mapping of input " + std::to_string(index)+
                            " to output " + std::to_string(mapping) +" (output "
                            + std::to_string(mapping)
                            + " is already mapped to from input "
                            + std::to_string(outputMappings[mapping]) + ") in "
                            + errorLocation);
          throw INVALID_ROTOR_MAPPING;
        }

    // Update mappings
    mappings[index] = mapping;
    outputMappings[mapping] = index;
    }
  // If inputStream fails less than 26 ints present in .rot file
  if(inputStream.fail()){
    printErrorMessage("Not all inputs mapped in " + errorLocation);
    throw INVALID_ROTOR_MAPPING;
  }
}

void Rotor::setNotchesFromFStream(std::ifstream& inputStream,
                                std::string errorLocation){
  int notchPosition;

  notchPosition = getNextInt(inputStream, errorLocation);

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
    notchPosition = getNextInt(inputStream, errorLocation);
    }

}
