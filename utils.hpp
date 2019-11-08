#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <string>

const int NUM_LETTERS_IN_ALPHABET = 26;

// Used to show value not set, -1 is not a valid array so cannot be set by a
// config file.
const int VALUE_NOT_SET = -1;
const int MAPPING_INDENT = 15;
const int DIGIT_SPACING = 3;

/* Returns next int (can be larger than one digit). If next char is not a digit
  a NON_NUMERIC_CHARACTER error will be thrown */
int getNextInt(std::ifstream& inputStream, std::string errorLocation);

/* Prints error message in a standard format */
void printErrorMessage(std::string errorMessage);

#endif
