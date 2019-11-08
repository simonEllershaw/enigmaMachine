#include "rotor.hpp"
#include "reflector.hpp"
#include "plugboard.hpp"
#include <iostream>
#include "enigma.hpp"
#include <vector>

void testPlugboardLoading(const char* configFname, const char* message){
  try{
    std::cout << message << ":" << std::endl;
    Plugboard testPlugboard = Plugboard(configFname);
    testPlugboard.print();
  }
  catch(int e){
    std::cout << "Caught an exception " << e << std::endl;;
  }
}

void testReflectorLoading(const char* configFname, const char* message){
  try{
    std::cout << message << ":" << std::endl;
    Reflector testReflector = Reflector(configFname);
    testReflector.print();
  }
  catch(int e){
    std::cout << "Caught an exception " << e << std::endl;;
  }
}


void testRotorLoading(const char* configFname, const char* message){
  try{
    std::cout << message << ":" << std::endl;
    Rotor testRotor = Rotor(configFname, 0);
    std::cout << "Loaded rotor mappings:" << std::endl;
    testRotor.print();
  }
  catch(int e){
    std::cout << "Caught an exception " << e << std::endl;
  }
}


int main(){

  std::cout << "///////////////// Plugboard Testing //////////////////////////////"
            << std::endl;
  std::cout << "Plugboard Loading Testing" << std::endl;
  testPlugboardLoading("plugboards/I.pb",
                    "Success- standard load");
  testPlugboardLoading("testFiles/plugboards/difLines.pb",
                    "Success- load input of different line");
  testPlugboardLoading("testFiles/plugboards/oddNumber.pb",
                    "Fail- odd number of ints in file");
  testPlugboardLoading("testFiles/plugboards/doesNotExist.pb",
                    "Fail- file does not exist");
  testPlugboardLoading("testFiles/plugboards/outOfBounds.pb",
                    "Fail- indicies not between 0-25");
  testPlugboardLoading("testFiles/plugboards/nonNumeric.pb",
                    "Fail- non numeric input");
  testPlugboardLoading("testFiles/plugboards/selfMap.pb",
                    "Fail- self mapped");
  testPlugboardLoading("testFiles/plugboards/repeatedMapping.pb",
                    "Fail- repeated mappings");
  std::cout << std::endl;

  std::cout << "///////////////// Reflector Testing //////////////////////////////"
            << std::endl;
  std::cout << "Reflector Loading Testing" << std::endl;
  testReflectorLoading("reflectors/I.rf",
                    "Success- standard load");
  testReflectorLoading("testFiles/reflectors/indicesTooLarge.rf",
                    "Fail- indicies too large");
  testReflectorLoading("testFiles/reflectors/invlalid.rf",
                    "Fail- not a file");
  testReflectorLoading("testFiles/reflectors/mapItself.rf",
                    "Fail- maps too itself");
  testReflectorLoading("testFiles/reflectors/negative.rf",
                    "Fail- indicies too small");
  testReflectorLoading("testFiles/reflectors/nonNumeric.rf",
                    "Fail- non numeric input");
  testReflectorLoading("testFiles/reflectors/null.rf",
                    "Fail- too few inputs");
  testReflectorLoading("testFiles/reflectors/repeatedMapping.rf",
                    "Fail- repeated mapping");
  testReflectorLoading("testFiles/reflectors/tooManyInputs.rf",
                    "Fail- too many inputs");
  std::cout << std::endl;

  std::cout << "///////////////// Rotor Testing //////////////////////////////"
            << std::endl;
  std::cout << "Rotor Loading Testing" << std::endl;
  testRotorLoading("rotors/I.rot",
                    "Success- standard load");
  testRotorLoading("testFiles/rotors/noNotches.rot",
                    "Success- no notches allowed");
  testRotorLoading("testFiles/rotors/notAFile.rot",
                    "Fail- invalid filename");
  testRotorLoading("testFiles/rotors/multipleMapping.rot",
                    "Fail- multiple mappings");
  testRotorLoading("testFiles/rotors/multipleNotches.rot",
                    "Fail- multiple notches");
  testRotorLoading("testFiles/rotors/nonNumericMapping.rot",
                    "Fail- non numeric mapping");
  testRotorLoading("testFiles/rotors/nonNumericNotches.rot",
                    "Fail- non numeric notches");
  testRotorLoading("testFiles/rotors/notEnoughInputsMapping.rot",
                    "Fail- not enough mappings");
  testRotorLoading("testFiles/rotors/tooLargerInputMapping.rot",
                  "Fail- to large mapping input");
  testRotorLoading("testFiles/rotors/tooLargerInputNotches.rot",
                    "Fail- to large notch input");
  testRotorLoading("testFiles/rotors/tooSmallerInputMapping.rot",
                    "Fail- too small input mappings");
  testRotorLoading("testFiles/rotors/tooSmallerInputNotches.rot",
                    "Fail- too small input notches");

  std::cout << "Rotor Function Testing:" << std::endl;
  // Rotor with notch at 0, 13
  Rotor testRotor = Rotor("rotors/V.rot", 0);

  std::cout << "Reverse mappings:" << std::endl;
  for(int i = 0; i < NUM_LETTERS_IN_ALPHABET; i ++)
    std::cout << i << " " << testRotor.getBackwardMapping(i) << std::endl;

  std::cout << "Current state:" << std::endl;
  testRotor.print();
  testRotor.rotateRotor();
  std::cout << "Rotate once:" << std::endl;
  testRotor.print();

}
