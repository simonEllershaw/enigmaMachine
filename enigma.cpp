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

  int convertedInput = input - CAP_A_ASCII_VALUE;
  if(convertedInput < 0 || convertedInput >= NUM_LETTERS_IN_ALPHABET)
    throw INVALID_INPUT_CHARACTER;

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


void getRotorFnamesFromCmdLine(std::vector<const char*>& rotorFnames, int argc,
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

void outputErrorMessage(const int e){
  using namespace std;
  cout << endl << "ERROR: ";
  switch(e){
    case 1:
      cout << "INSUFFICIENT_NUMBER_OF_PARAMETERS"; break;
    case 2:
      cout << "INVALID_INPUT_CHARACTER"; break;
    case 3:
      cout << "INVALID_INDEX"; break;
    case 4:
      cout << "NON_NUMERIC_CHARACTER"; break;
    case 5:
      cout << "IMPOSSIBLE_PLUGBOARD_CONFIGURATION"; break;
    case 6:
      cout << "INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS"; break;
    case 7:
      cout << "INVALID_ROTOR_MAPPING"; break;
    case 8:
      cout << "NO_ROTOR_STARTING_POSITION"; break;
    case 9:
      cout << "INVALID_REFLECTOR_MAPPING"; break;
    case 10:
      cout << "INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS"; break;
    case 11:
      cout << "ERROR_OPENING_CONFIGURATION_FILE"; break;
    default:
      cout << "UNDEFINED_ERROR_CODE";
    }
    cout << endl;
}


int main(int argc, char** argv){
  try{
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
  catch(int e){
    outputErrorMessage(e);
    return e;
  }

  return 0;
}
// ./enigma plugboards/I.pb reflectors/I.rf rotors/I.rot rotors/II.rot rotors/III.rot rotors/I.pos
