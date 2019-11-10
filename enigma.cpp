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

Enigma::Enigma(std::string plugboardFname, std::string reflectorFname,
              std::vector<std::string> rotorFnames,
              std::string rotorPositionsFname){

  plugboard = new Plugboard(plugboardFname);
  reflector = new Reflector(reflectorFname);
  setRotorVector(rotorPositionsFname, rotorFnames);
}

void Enigma::setRotorVector(std::string rotorPositionsFname,
                          std::vector<std::string> rotorFnames){
  std::ifstream rotorPositionStream;
  int rotorPosition, rotorNumber = 0;
  std::string errorLocation = " in rotor positions file " + rotorPositionsFname;

  rotorPositionStream.open(rotorPositionsFname);
  if(rotorPositionStream.fail()){
    printErrorMessage("Could not open " + rotorPositionsFname);
    throw ERROR_OPENING_CONFIGURATION_FILE;
  }

  auto fname = rotorFnames.begin();
  rotorPosition = getNextInt(rotorPositionStream, errorLocation);
  while(!rotorPositionStream.fail()){
      checkRotorPositionIsValid(rotorPositionStream, rotorPosition, rotorNumber,
                                errorLocation);
      rotorVector.push_back(new Rotor(*fname, rotorPosition));
      ++fname; //Error for not enough files??
      rotorNumber++;
      rotorPosition = getNextInt(rotorPositionStream, errorLocation);
    }
  // If when rotorPositionStream fails not all rotors have been initialised
  // not enough rotor positions where given in the config file
  if(fname != rotorFnames.end()){
    printErrorMessage("No starting position for rotor " +
    std::to_string(rotorNumber) + errorLocation);
    throw NO_ROTOR_STARTING_POSITION;
  }
}

void Enigma::checkRotorPositionIsValid(std::ifstream& rotorPositionStream,
                              const int rotorPosition, const int rotorNumber,
                              std::string errorLocation){
  if(rotorPosition < 0 || rotorPosition >= NUM_LETTERS_IN_ALPHABET){
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
  int convertedInput;

  if(input < 'A' || input >= 'Z'){
    printErrorMessage(std::string(1, input) + " is not a valid input character"
                      " (input characters must be upper case letters A-Z)!");
    throw INVALID_INPUT_CHARACTER;
  }

  rotateRotors();

  convertedInput = input - CAP_A_ASCII_VALUE;


  // Encoding right to left
  convertedInput = plugboard->getForwardMapping(convertedInput);
  for(auto rotor = rotorVector.rbegin(); rotor != rotorVector.rend(); ++rotor)
    convertedInput = (*rotor)->getForwardMapping(convertedInput);
  convertedInput = reflector->getMapping(convertedInput);
  // Encoding left to right
  for(auto rotor = rotorVector.begin(); rotor != rotorVector.end(); ++rotor)
    convertedInput = (*rotor)->getBackwardMapping(convertedInput);
  convertedInput = plugboard->getBackwardMapping(convertedInput);
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


void getRotorFnamesFromCmdLine(std::vector<std::string>& rotorFnames, int argc,
                              char**argv){
  for(int argNumber = 3; argNumber < argc-1; argNumber++)
    rotorFnames.push_back(argv[argNumber]);
}

void IOEnigmaInterface(Enigma* enigma){
  using namespace std;
  std::string input;
  getline(cin, input, '\n');

  for(int index = 0; input[index] != '\0'; index++){
    if(input[index] != ' ')
      std::cout << enigma->encodeChar(input[index]);
  }
}

int main(int argc, char** argv){
  try{
    if(argc < 4){
      printErrorMessage("usage: enigma plugboard-file reflector-file"
                        "(<rotor-file>)* rotor-positions ");
      throw INSUFFICIENT_NUMBER_OF_PARAMETERS;
    }

    std::vector<std::string> rotorFnames;
    std::string plugboardFname = argv[1];
    std::string reflectorFname = argv[2];
    std::string rotorPositionsFname = argv[argc-1];
    getRotorFnamesFromCmdLine(rotorFnames, argc, argv);

    Enigma* enigma = new Enigma(plugboardFname, reflectorFname, rotorFnames,
                                rotorPositionsFname);

    IOEnigmaInterface(enigma);
    return 0;
  }
  catch(int e){
    return e;
  }
}
// ./enigma plugboards/I.pb reflectors/I.rf rotors/I.rot rotors/II.rot rotors/III.rot rotors/I.pos
