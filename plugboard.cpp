#include "plugboard.hpp"
#include "errors.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <iomanip>

using namespace std;

//////////////////////////// Public Functions ////////////////////////////////

Plugboard::Plugboard(string configFname){
  setDefaultMappings();
  setMappingsFromFile(configFname);
}

void Plugboard::print(){
  cout << setw(MAPPING_INDENT) << left << "Plugboard: ";
  for(int i = 0; i < NUM_LETTERS_IN_ALPHABET; i ++){
    cout << setw(DIGIT_SPACING) << right << getForwardMapping(i);
  }
  cout << endl;
}

//////////////////////////// Private Functions ////////////////////////////////

void Plugboard::setDefaultMappings(){
  // Default mapping is each index to itself i.e. 0 to 0
  for(int index = 0; index < NUM_LETTERS_IN_ALPHABET; index ++){
    mappings[index] = index;
  }
}

void Plugboard::setMappingsFromFile(string configFname){
  ifstream inputStream;
  int mapping, index;
  string errorLocation = " in plugboard file " + configFname;

  openConfigFile(configFname, inputStream);

  // Get pairs of ints from file the first is the index and the second the
  // new mapping of that index
  while(!inputStream.fail()){
    index = getNextInt(inputStream, errorLocation);
    // If EOF here then an even number of ints have been parsed
    // so break without error
    if(inputStream.eof())
      break;

    mapping = getNextInt(inputStream, errorLocation);
    // If EOF here an odd number of ints have been parsed so throw exception
    if(inputStream.eof()){
      printErrorMessage("Incorrect number of parameters" + errorLocation);
      throw INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
    }

    checkIntWithinAlphabet(index, errorLocation);
    checkIntWithinAlphabet(mapping, errorLocation);

    // Check not mapping the index to itself and that it has not been previously
    // set
    if(index == mapping || mappings[index] != index){
      printErrorMessage("The configuration" + errorLocation +
                        " is impossible.");
      throw IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
    }

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
