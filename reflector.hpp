class Reflector{
  int mappings[26];
  int defaultMappingValue = -1;

  /* Set all connections to have a value of -1 so it can be detected if they
    have been set or not later (as -1 is an invalid mapping) */
  void setDefaultMappings();

  /* Set mapping from config file */
  void setMappingsFromFile(const char* configFname);
public:
  /*Reflector contructor- takes config filename as a parameter
  Config file should be of the for index pa*/
  Reflector(const char* configFname);

  /* Returns reflector mapping of an index */
  int getMapping(const int index);
};
