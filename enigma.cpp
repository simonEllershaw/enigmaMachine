#include "enigma.hpp"
#include "rotor.hpp"
#include "plugboard.hpp"
#include "reflector.hpp"
#include "errors.h"
#include<iostream>
#include <fstream>
#include <iomanip>

const int CAP_A_ASCII_VALUE = int('A');
const int MAX_INPUT_SIZE = 80;

Enigma::Enigma(const char* plugboardFname, const char* reflectorFname,
      std::vector<const char*> rotorFnames, const char* rotorPositionsFname){

  plugboard = new Plugboard(plugboardFname);
  reflector = new Reflector(reflectorFname);
  setRotorVector(rotorPositionsFname, rotorFnames);
}

void Enigma::setRotorVector(const char* rotorPositionsFname,
                          std::vector<const char*> rotorFnames){
  std::ifstream rotorPositionStream;
  int rotorPosition;

  rotorPositionStream.open(rotorPositionsFname);
  if(rotorPositionStream.fail()){
    throw ERROR_OPENING_CONFIGURATION_FILE;
  }

  for(auto fname = rotorFnames.begin(); fname != rotorFnames.end();
      ++fname){
      rotorPosition = getNextInt(rotorPositionStream);
      checkRotorPositionIsValid(rotorPositionStream, rotorPosition);
      rotorVector.push_back(new Rotor(*fname, rotorPosition));
    }
}

void Enigma::checkRotorPositionIsValid(std::ifstream& rotorPositionStream,
                                const int rotorPosition){
  if(rotorPositionStream.fail()){
    throw NO_ROTOR_STARTING_POSITION;
  }
  else if(rotorPosition < 0 || rotorPosition >= NUM_LETTERS_IN_ALPHABET){
    throw INVALID_INDEX;
  }
}

void Enigma::print(){
  std::cout << "Enigma Machine State:" << std::endl;
  std::cout << std::setw(MAPPING_INDENT) << std::left << "Index: ";
  for(int i = 0; i < NUM_LETTERS_IN_ALPHABET; i ++)
    std::cout << std::setw(DIGIT_SPACING) << std::right  << i;
  std::cout << std::endl;

  plugboard->print();

  for(auto rotor = rotorVector.rbegin(); rotor != rotorVector.rend(); ++rotor)
    (*rotor)->print();
  reflector->print();

  std::cout << "Note: Rightmost rotors at top leftmost at the bottom" << std::endl;
}

char Enigma::encodeChar(char input){
  rotateRotors();

  print();
  int convertedInput = input - CAP_A_ASCII_VALUE;
  if(convertedInput < 0 || convertedInput >= NUM_LETTERS_IN_ALPHABET)
    throw INVALID_INPUT_CHARACTER;

  std::cout << "Input: " << convertedInput <<std::endl;
  // Encoding right to left
  convertedInput = plugboard->getForwardMapping(convertedInput);
  std::cout << "Plugboard: " << convertedInput << std::endl;
  for(auto rotor = rotorVector.rbegin(); rotor != rotorVector.rend(); ++rotor){
    convertedInput = (*rotor)->getForwardMapping(convertedInput);
    std::cout << "Forward rotor: " << convertedInput << std::endl;}
  convertedInput = reflector->getMapping(convertedInput);
  std::cout << "Reflector " << convertedInput << std::endl;
  // Encoding left to right
  for(auto rotor = rotorVector.begin(); rotor != rotorVector.end(); ++rotor){
    convertedInput = (*rotor)->getBackwardMapping(convertedInput);
    std::cout << "Backward rotor: " << convertedInput << std::endl;}
  convertedInput = plugboard->getBackwardMapping(convertedInput);
  std::cout << "Backward plugboard: " << convertedInput << std::endl;
  return char(convertedInput + CAP_A_ASCII_VALUE);
}

void Enigma::rotateRotors(){
  for(auto rotor = rotorVector.rbegin(); rotor != rotorVector.rend(); ++rotor){
    (*rotor)->rotateRotor();
    if(!(*rotor)->aNotchIsAtOrigin())
      break;
    }
}

Enigma::~Enigma(){
  delete plugboard;
  delete reflector;
  for(auto rotor = rotorVector.begin(); rotor != rotorVector.end(); ++rotor)
    delete *rotor;
}


void getRotorFnamesFromCmdLine(std::vector<const char*>& rotorFnames, int argc,
                              char**argv){
  for(int argNumber = 3; argNumber < argc-1; argNumber++)
    rotorFnames.push_back(argv[argNumber]);
}

void IOEnigmaInterface(Enigma* enigma){
  char input[MAX_INPUT_SIZE];
  std::cout << "Please enter phrase to encrypt in all caps: ";
  std::cin >> input;

  for(int index = 0; input[index] != '\0'; index++)
    std::cout << enigma->encodeChar(input[index]);
  std::cout<<std::endl;
}


int main(int argc, char** argv){
  if(argc < 4)
    throw INSUFFICIENT_NUMBER_OF_PARAMETERS;

  std::vector<const char*> rotorFnames;
  const char* plugboardFname = argv[1];
  const char* reflectorFname = argv[2];
  const char* rotorPositionsFname = argv[argc-1];
  getRotorFnamesFromCmdLine(rotorFnames, argc, argv);

  Enigma* enigma = new Enigma(plugboardFname, reflectorFname, rotorFnames,
                              rotorPositionsFname);

  IOEnigmaInterface(enigma);

  return 0;
}
// ./enigma plugboards/I.pb reflectors/I.rf rotors/I.rot rotors/II.rot rotors/III.rot rotors/I.pos
