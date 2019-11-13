#include "enigma.hpp"
#include "errors.h"
#include "utils.hpp"
#include "test.hpp"
#include<iostream>
#include <fstream>
#include <iomanip>


using namespace std;

const int CAP_A_ASCII_VALUE = int('A');

///////////////////////////// Public Functions ////////////////////////////////

Enigma::Enigma(string plugboardFname, string reflectorFname,
              vector<string> rotorFnames, string rotorPositionsFname){
  plugboard = new Plugboard(plugboardFname);
  reflector = new Reflector(reflectorFname);
  setRotorVector(rotorPositionsFname, rotorFnames);
}

void Enigma::IOInterface(){
  string input;
  getline(cin, input, '\n');

  for(int index = 0; input[index] != '\0'; index++){
    //Skip whitespace
    if(input[index] != ' ')
      cout << encodeChar(input[index]);
  }
}

void Enigma::print(){
  cout << "Enigma Machine State:" << endl;
  cout << setw(MAPPING_INDENT) << left << "Index: ";
  for(int i = 0; i < NUM_LETTERS_IN_ALPHABET; i ++)
    cout << setw(DIGIT_SPACING) << right  << i;
  cout << endl;

  plugboard->print();

  for(auto rotor = rotorVector.rbegin(); rotor != rotorVector.rend(); ++rotor)
    (*rotor)->print();
  reflector->print();

  cout << "Note: Rightmost rotors at top leftmost at the bottom" << endl;
}

//////////////////////////// Private Functions ////////////////////////////////

void Enigma::setRotorVector(string rotorPositionsFname,
                          vector<string> rotorFnames){
  ifstream rotorPositionStream;
  int rotorPosition, rotorNumber = 0;
  string errorLocation = " in rotor positions file " + rotorPositionsFname;

  // Set up vector of rotors reading the starting position from file
  openConfigFile(rotorPositionsFname, rotorPositionStream);
  auto fname = rotorFnames.begin();
  rotorPosition = getNextInt(rotorPositionStream, errorLocation);

  // Ensure still a position and rotorFname to use
  while(!rotorPositionStream.fail() && fname != rotorFnames.end()){
      // Check params and put new rotor in vector
      checkIntWithinAlphabet(rotorPosition, errorLocation);
      rotorVector.push_back(new Rotor(*fname, rotorPosition));
      // Get parameters for next rotor
      ++fname;
      rotorNumber++;
      rotorPosition = getNextInt(rotorPositionStream, errorLocation);
    }

  // If when rotorPositionStream fails not all rotors have been initialised
  // not enough rotor positions where given in the config file
  if(fname != rotorFnames.end()){
    printErrorMessage("No starting position for rotor " +
                      to_string(rotorNumber) + " in rotor position file: "
                      + rotorPositionsFname);
    throw NO_ROTOR_STARTING_POSITION;
  }
}

char Enigma::encodeChar(char input){
  int convertedInput;
  // Check input is a capital alphabetic letter
  if(input < 'A' || input >= 'Z'){
    printErrorMessage(string(1, input) + " is not a valid input character"
                      " (input characters must be upper case letters A-Z)!");
    throw INVALID_INPUT_CHARACTER;
  }
  // Note rotor rotated before encoding
  rotateRotors();

  // A maps to 0 and so on
  convertedInput = input - CAP_A_ASCII_VALUE;

  // Encoding order: Plugboard-> Forward Rotors-> Reflector-> Backward Rotor
  //                  -> Plugboard
  convertedInput = plugboard->getForwardMapping(convertedInput);
  for(auto rotor = rotorVector.rbegin(); rotor != rotorVector.rend(); ++rotor)
    convertedInput = (*rotor)->getForwardMapping(convertedInput);
  convertedInput = reflector->getMapping(convertedInput);
  for(auto rotor = rotorVector.begin(); rotor != rotorVector.end(); ++rotor)
    convertedInput = (*rotor)->getBackwardMapping(convertedInput);
  convertedInput = plugboard->getBackwardMapping(convertedInput);

  // Convert back to a char
  return char(convertedInput + CAP_A_ASCII_VALUE);
}

void Enigma::rotateRotors(){
  // Rotate starting rotor 1 position, if notch at the top rotate the next
  // If no notch at the top end.
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

/////////////////////// Free Functions ///////////////////////////////////////

void getRotorFnamesFromCmdLine(vector<string>& rotorFnames, int argc,
                              char**argv){
  const int numArgsBeforeRotors = 3, numArgsAfterRotors = 1;
  for(int argNumber = numArgsBeforeRotors; argNumber < argc-numArgsAfterRotors;
      argNumber++)
    rotorFnames.push_back(argv[argNumber]);
}

int main(int argc, char** argv){
  try{
    // Run test programme
    if(argc == 2 && string(argv[1]) == "test")
      return testEnigma();
    // Check correct number of command line args
    else if(argc < 4){
      printErrorMessage("usage: enigma plugboard-file reflector-file"
                        " (<rotor-file>)* rotor-positions");
      throw INSUFFICIENT_NUMBER_OF_PARAMETERS;
    }

    //Create Enigma machine
    vector<string> rotorFnames;
    string plugboardFname = argv[1];
    string reflectorFname = argv[2];
    string rotorPositionsFname = argv[argc-1];
    getRotorFnamesFromCmdLine(rotorFnames, argc, argv);
    Enigma* enigma = new Enigma(plugboardFname, reflectorFname, rotorFnames,
                                rotorPositionsFname);
    // Encode message
    enigma->IOInterface();
    return 0;
  }
  catch(int e){
    return e;
  }
}
