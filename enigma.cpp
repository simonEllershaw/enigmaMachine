#include "enigma.hpp"
#include "rotor.hpp"
#include "plugboard.hpp"
#include "reflector.hpp"
#include "errors.h"
#include<iostream>
#include <fstream>

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


  char enocdeChar(char input){
    return 'A';
  }

  Enigma::~Enigma(){
    delete plugboard;
    delete reflector;
    for(auto rotor = rotorVector.begin(); rotor != rotorVector.end(); ++rotor)
      delete *rotor;
  }



int main(int argc, char** argv){
  if(argc < 4){
    throw INSUFFICIENT_NUMBER_OF_PARAMETERS;
  }

  const char* plugboardFname = argv[1];
  const char* reflectorFname = argv[2];
  const char* rotorPositionsFname = argv[argc-1];

  std::vector<const char*> rotorFnames;
  for(int argNumber = 3; argNumber < argc-1; argNumber++)
    rotorFnames.push_back(argv[argNumber]);
  std::cout << plugboardFname << reflectorFname
            << rotorPositionsFname << std::endl;
  Enigma(plugboardFname, reflectorFname, rotorFnames, rotorPositionsFname);
  return 0;
}
// ./enigma plugboards/I.pb reflectors/I.rf rotors/I.rot rotors/I.pos
