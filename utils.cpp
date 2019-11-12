#include "utils.hpp"
#include "errors.h"
#include <iostream>

int getNextInt(std::ifstream& inputStream, std::string errorLocation){
  int nextInt;
  inputStream >> nextInt;
  // Flag set if last character read in not a digit
  if(inputStream.rdstate() == std::ifstream::failbit){
    printErrorMessage("Non-numeric character" + errorLocation);
    throw NON_NUMERIC_CHARACTER;
  }
  return nextInt;
}

void printErrorMessage(std::string errorMessage){
  std::cerr << errorMessage << std::endl;
}

void openConfigFile(std::string configFname,ifstream& inputStream){
  inputStream.open(configFname);
  if(inputStream.fail()){
    printErrorMessage("Could not open " + configFname);
    throw ERROR_OPENING_CONFIGURATION_FILE;
  }
}

void checkIntWithinAlphabet(int inputToCheck, string errorLocation){
  if(inputToCheck < 0 || inputToCheck >= NUM_LETTERS_IN_ALPHABET){
    printErrorMessage("The input value " + std::to_string(inputToCheck) +
                      " is not within the bounds 0-"
                      + std::to_string(NUM_LETTERS_IN_ALPHABET) +
                      ". Value found" + errorLocation);
    throw INVALID_INDEX;
  }
}
