#include "utils.hpp"
#include "errors.h"
#include <iostream>

int getNextInt(std::ifstream& inputStream, std::string errorLocation){
  int nextInt;
  inputStream >> nextInt;
  // Flag set if last character read in not a digit
  if(inputStream.rdstate() == std::ifstream::failbit){
    printErrorMessage("Non-numeric character in " + errorLocation);
    throw NON_NUMERIC_CHARACTER;
  }
  return nextInt;
}

void printErrorMessage(std::string errorMessage){
  using namespace std;
  cout << errorMessage;
}
