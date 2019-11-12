#include "reflector.hpp"
#include "errors.h"
#include <iostream>
#include <cstdlib>
#include <exception>
#include <iomanip>

using namespace std;

/////////////////////////// Public Functions /////////////////////////////////

Reflector::Reflector(string configFname){
  setMappingsToNotSet();
  setMappingsFromFile(configFname);
}

int Reflector::getMapping(const int index){
  return mappings[index];
}

void Reflector::print(){
  cout << setw(MAPPING_INDENT) << left << "Reflector: ";
  for(int i = 0; i < NUM_LETTERS_IN_ALPHABET; i ++)
    cout << setw(DIGIT_SPACING) << right << getMapping(i);
  cout << endl;
}

/////////////////////////// Private Functions /////////////////////////////////

void Reflector::setMappingsToNotSet(){
  for(int position = 0; position < NUM_LETTERS_IN_ALPHABET; position ++)
    mappings[position] = VALUE_NOT_SET;
}

void Reflector::setMappingsFromFile(string configFname){
  ifstream inputStream;
  int numMappingsReadIn = 0;
  string errorLocation = " in reflector file " + configFname;

  openConfigFile(configFname, inputStream);
  parseConfigFile(inputStream, errorLocation, numMappingsReadIn);
  checkCorrectNumParamsReadIn(numMappingsReadIn, errorLocation);

  inputStream.close();
}

void Reflector::parseConfigFile(ifstream& inputStream, string errorLocation,
                              int& numMappingsReadIn){
  int index1, index2;
  // Get pairs of ints from file these are the two indicies to be mapped to
  // each other
  while(!inputStream.fail()){
    index1 = getNextInt(inputStream, errorLocation);
    if(inputStream.fail())
      break;
    numMappingsReadIn ++;

    index2 = getNextInt(inputStream, errorLocation);
    if(inputStream.fail())
      break;
    numMappingsReadIn ++;

    checkIntWithinAlphabet(index1, errorLocation);
    checkIntWithinAlphabet(index2, errorLocation);

    // Check not mapping index to itself and that it has not been previously
    // assigned i.e. differs from VALUE_NOT_SET
    if(index1 == index2 || mappings[index1] != VALUE_NOT_SET ||
        mappings[index2] != VALUE_NOT_SET){
          printErrorMessage("Invalid reflector mapping " +
                            to_string(index1) + " to " +
                            to_string(index2) + errorLocation);
          throw INVALID_REFLECTOR_MAPPING;
        }
    // Update mapping
    mappings[index1] = index2;
    mappings[index2] = index1;
    }
}

void Reflector::checkCorrectNumParamsReadIn(int numMappingsReadIn,
                                            string errorLocation){
  string additionalInfo = "";
  // If this test passes then as no repeated mappings are allowed all indicies
  // must have been mapped
  if(numMappingsReadIn!=(NUM_LETTERS_IN_ALPHABET)){
    if(numMappingsReadIn % 2 == 1)
      additionalInfo = "(odd) ";
    printErrorMessage("Incorrect " + additionalInfo + "number of mappings"
                      + errorLocation);
    throw INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
  }
}
