
#ifndef PAN_BASE_CONTAINER_H_INCLUDE
#define PAN_BASE_CONTAINER_H_INCLUDE

#include <string>
#include <H5Cpp.h>


namespace pandora {

class BaseContainer {
  
protected:
  BaseContainer(){};
  
  BaseContainer(H5::Group h5group) : h5group(h5group) {};

  bool attrExists(std::string name) const;
  bool objectExists(std::string path) const;
  
  bool getAttr(std::string name, std::string &value) const;
  void setAttr(std::string name, std::string  value) const;
  
  H5::Group h5group;
};

  
}

#endif /* PAN_BASE_CONTAINER_H_INCLUDE */
