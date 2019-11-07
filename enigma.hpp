#include "rotor.hpp"
#include "plugboard.hpp"
#include "reflector.hpp"
#include <vector>
#include <fstream>

class Enigma{
  std::vector<Rotor*> rotorVector;
  Plugboard* plugboard = nullptr;
  Reflector* reflector = nullptr;

  /* Creates rotors using cmd line config files and adds to rotorVector */
  void setRotorVector(const char* rotorPositionsFname,
                    std::vector<const char*> rotorFnames);

  /* Checks the .pos file supplied is valid, throws exceptions if not */
  void checkRotorPositionIsValid(std::ifstream& rotorPositionStream,
                                const int rotorPosition);

   /* Rotates leftmost rotor 1 position, updates rotors to the right if notch at
      the top */
  void rotateRotors();

  public:

  /* Creates Enigma object using files given on cmd line */
  Enigma(const char* plugboardFname, const char* reflectorFname,
        std::vector<const char*> rotorFnames, const char* rotorPositionsFname);

  /* Returns a single encoded char, also updates internal state of the Enigma
    machine e.g. rotates rotors */
  char encodeChar(char input);

  /* Outputs text representation of the state of the enigma machine to cmd line */
  void print();

  ~Enigma();
};

/* Updates rotorFnames vector with the rotor filenames supplied on the cmd line*/
void getRotorFnamesFromCmdLine(std::vector<const char*>& rotorFnames, int argc,
                              char**argv);
