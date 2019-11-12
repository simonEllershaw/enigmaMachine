                          ENIGMA MACHINE
                      Author Simon Ellershaw

A C++ implementation of the enigma encoding machine used by Germany in WWII. A
good explanation of its workings can be found here:
http://users.telenet.be/d.rijmenants/en/enigmatech.htm

To run:
  run make on the cmd line
  Cmd line call:
  ./enigma plugboardFile reflectorFile rotorFiles rotorPositionFile
  Example call: ./enigma plugboards/I.pb reflectors/I.rf rotors/I.rot
                rotors/II.rot rotors/III.rot rotors/I.pos
  Type plaintext in ALL CAPS into the command line

  Note: Can have between 0-N rotors

File Types
  Plugboard - Pairs of ints mapping the first int to the second
              By default the input is mapped to itseld
              eg. 0 5 means 0->5
              Example file: plugboards/I.pb
  Reflector - 13 pairs of ints mapping to each other
              e.g. 2 4 means 2->4 and 4->2
              Example file: reflectors/I.rf
  Rotor- First 26 ints are mappings of that index to an output. The remaining
         ints are the position of notches on the rotor
         Example file: rotors/I.rot
  rotorPositionFile - Same number of ints as number of rotorFiles
                      Example file: rotor/I.pos

  Note: All inputs must be between 0-25 and not have repeated mapping or map
        to themselves
