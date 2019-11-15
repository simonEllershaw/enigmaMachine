#include "rotor.hpp"
#include "errors.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <iomanip>

using namespace std;


//////////////////////// Public Functions /////////////////////////////////////

Rotor::Rotor(string configFname, const int startingPosition){
  ifstream inputStream;
  string errorLocation = " for mapping in rotor file " + configFname;

  // Non-config file dependent setting functions
  positionAtOrigin = startingPosition;
  setMappingsToNotSet();

  // Config file dependent setting functions
  openConfigFile(configFname, inputStream);
  setMappingsFromFStream(inputStream, errorLocation);
  setNotchesFromFStream(inputStream, errorLocation);
  inputStream.close();
}

int Rotor::getForwardMapping(const int index){
  // Gets mapping from index.
  // Importantly has to account for what position is the rotor is in and map
  // any overflow e.g. -1 -> 25
  int positionAdjustedIndex = mapOverflow(index + positionAtOrigin);
  // Account for position of rotor on output
  return mapOverflow(mappings[positionAdjustedIndex] - positionAtOrigin);
}

int Rotor::getBackwardMapping(const int mapping){
  int index = 0;
  // Find index of mapping
  for(; mappings[index] != mapOverflow(mapping + positionAtOrigin); index++);
  // Account for position of rotor on output
  return mapOverflow(index - positionAtOrigin);
}

bool Rotor::aNotchIsAtOrigin(){
  return notches[positionAtOrigin];
}

void Rotor::rotateRotor(){
  positionAtOrigin = mapOverflow(positionAtOrigin + 1);
}

void Rotor::print(){
  cout << setw(MAPPING_INDENT) << left << "Rotor: ";
  for(int i = 0; i < NUM_LETTERS_IN_ALPHABET; i ++){
    cout << setw(DIGIT_SPACING) << right << mappings[i];
  }
  cout << endl;
  cout << setw(MAPPING_INDENT) << left << "Notches: ";
  for(int i = 0; i < NUM_LETTERS_IN_ALPHABET; i ++){
    cout << setw(DIGIT_SPACING) << right << notches[i];
  }
  cout << " Position at origin: " << positionAtOrigin << endl;
}


//////////////////////////// Private Functions ////////////////////////////////

int Rotor::mapOverflow(int index){
    // Maps -1 -> 25 and 26->0 etc...
    return (index + NUM_LETTERS_IN_ALPHABET) % NUM_LETTERS_IN_ALPHABET;
}

void Rotor::setMappingsToNotSet(){
  for(int position = 0; position < NUM_LETTERS_IN_ALPHABET; position ++){
    mappings[position] = VALUE_NOT_SET;
  }
}

void Rotor::setMappingsFromFStream(ifstream& inputStream, string errorLocation){
  int mapping, index;
  // Allows checking output not already mapped
  int outputMappings[NUM_LETTERS_IN_ALPHABET];

  for(index = 0; index < NUM_LETTERS_IN_ALPHABET; index++){
    outputMappings[index] = VALUE_NOT_SET;
  }

  // Get 26 ints check the int is valid and set as rotor mapping
  for(index = 0; index < NUM_LETTERS_IN_ALPHABET; index++){
    mapping = getNextInt(inputStream, errorLocation);
    if(inputStream.fail()){
      cerr << "Not all inputs mapped" + errorLocation;
      throw INVALID_ROTOR_MAPPING;
    }

    checkIntWithinAlphabet(mapping, errorLocation);

    // Check mapping has not previously been set as output
    if(outputMappings[mapping] != VALUE_NOT_SET){
          printErrorMessage("Invalid mapping of input " + to_string(index)+
                            " to output " + to_string(mapping) +" (output "
                            + to_string(mapping)
                            + " is already mapped to from input "
                            + to_string(outputMappings[mapping]) + ")"
                            + errorLocation);
          throw INVALID_ROTOR_MAPPING;
        }
    // Update mappings
    mappings[index] = mapping;
    outputMappings[mapping] = index;
    }
}

void Rotor::setNotchesFromFStream(ifstream& inputStream, string errorLocation){
  int notchPosition;

  notchPosition = getNextInt(inputStream, errorLocation);

  // Get remaing ints from .rot file and assign as notch positions
  while(!inputStream.fail()){
    checkIntWithinAlphabet(notchPosition, errorLocation);
    // Check notch only set once
    if(notches[notchPosition]){
          printErrorMessage("Notch " + to_string(notchPosition) + " has been" +
                            " set twice" + errorLocation);
          throw INVALID_ROTOR_MAPPING;
        }

    // Update and read in next notches
    notches[notchPosition] = true;
    notchPosition = getNextInt(inputStream, errorLocation);
    }
}
