#include "rotor.hpp"
#include "reflector.hpp"
#include "plugboard.hpp"
#include "enigma.hpp"
#include <iostream>

using namespace std;

void testPlugboardLoading(const char* configFname, const char* message){
  try{
    cout << message << ":" << endl;
    Plugboard testPlugboard = Plugboard(configFname);
    testPlugboard.print();
  }
  catch(int e){
    cout << "Error code: " << e << endl;;
  }
  cout << endl;
}


void testReflectorLoading(const char* configFname, const char* message){
  try{
    cout << message << ":" << endl;
    Reflector testReflector = Reflector(configFname);
    testReflector.print();
  }
  catch(int e){
    cout << "Error code: " << e << endl;;
  }
  cout << endl;
}


void testRotorLoading(const char* configFname, const char* message){
  try{
    cout << message << ":" << endl;
    Rotor testRotor = Rotor(configFname, 0);
    cout << "Loaded rotor mappings:" << endl;
    testRotor.print();
  }
  catch(int e){
    cout << "Error code: " << e << endl;
  }
  cout << endl;
}


void testRotorFunctionailty(const char* fname){
  cout << "Rotor Function Testing:" << endl;
  // Rotor with notch at 0, 13
  Rotor testRotor = Rotor(fname, 0);

  cout << "Reverse mappings:" << endl;
  for(int i = 0; i < NUM_LETTERS_IN_ALPHABET; i ++){
    cout << i << " ";
  }
  cout << endl;
  for(int i = 0; i < NUM_LETTERS_IN_ALPHABET; i ++){
    cout << testRotor.getBackwardMapping(i) << " ";
  }
  cout << endl;

  cout << "Current state:" << endl;
  testRotor.print();
  testRotor.rotateRotor();
  cout << "Rotate once:" << endl;
  testRotor.print();
}


int testEnigma(){

  cout << endl << "///////////////// Plugboard Testing ////////////////////////"
            << endl;
  cout << "Plugboard Loading Testing" << endl;
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
  cout << endl;

  cout << "///////////////// Reflector Testing ////////////////////////////////"
            << endl;
  cout << "Reflector Loading Testing" << endl;
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
  cout << endl;

  cout << "////////////////////// Rotor Testing //////////////////////////////"
            << endl;
  cout << "Rotor Loading Testing" << endl;
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

  testRotorFunctionailty("rotors/V.rot");

  cout << endl;

  return 0;
}
