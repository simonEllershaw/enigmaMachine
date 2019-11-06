#include "rotor.hpp"
#include "plugboard.hpp"
#include "reflector.hpp"
#include <vector>
#include <fstream>

class Enigma{
  std::vector<Rotor*> rotorVector;
  Plugboard* plugboard = nullptr;
  Reflector* reflector = nullptr;

  void setRotorVector(const char* rotorPositionsFname,
                    std::vector<const char*> rotorFnames);

  void checkRotorPositionIsValid(std::ifstream& rotorPositionStream,
                                const int rotorPosition);

  public:

  Enigma(const char* plugboardFname, const char* reflectorFname,
        std::vector<const char*> rotorFnames, const char* rotorPositionsFname);

  char enocdeChar(char input);

  ~Enigma();
};
