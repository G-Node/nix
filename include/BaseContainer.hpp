
#include <string>
#include <H5Cpp.h>


namespace pandora {

class BaseContainer {
  
protected:
  
  BaseContainer(H5::Group h5group) : h5group(h5group) {};

  bool attrExists(std::string name);
  bool objectExists(std::string path);
  
  bool getAttr(std::string name, std::string &value);
  void setAttr(std::string name, std::string  value);
  
  H5::Group h5group;
};

  
}