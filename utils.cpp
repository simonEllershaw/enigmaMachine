#include "utils.hpp"
#include "errors.h"

int getNextInt(std::ifstream& inputStream){
  int nextInt;
  inputStream >> nextInt;
  // Flag set if last character read in not a digit
  if(inputStream.rdstate() == std::ifstream::failbit)
    throw NON_NUMERIC_CHARACTER;
  return nextInt;
}
