class Plugboard{
  int mappings[26];

  /* Set connection */
  void setDefaultMappings();
  void setMappingsFromFile(const char* configFname);
public:
  /*Plugboard contructor- takes config filename as a parameter
  Config file should be of the form: index mapping etc...
  See plugboards/I.pb for an example*/
  Plugboard(const char* configFname);

  /* Returns plugboard mapping of an index */
  int getMapping(const int index);
};
