
#include <string>
#include <H5Cpp.h>


namespace pandora {

class BaseContainer {
  
protected:
  
  BaseContainer(H5::Group h5group) : h5group(h5group) {};

  bool attrExists(std::string name);
  bool objectExists(std::string path);
  
  H5::Group h5group;
};

  
}