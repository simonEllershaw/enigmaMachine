#include <fstream>

const int NUM_LETTERS_IN_ALPHABET = 26;

/* Returns next int (can be larger than one digit). If next char is not a digit
  a NON_NUMERIC_CHARACTER error will be thrown */
int getNextInt(std::ifstream& inputStream);
