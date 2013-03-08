
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

  void delAttr(std::string name) const;

  bool getAttr(std::string name, std::string &value) const;
  void setAttr(std::string name, std::string  value) const;

  bool hasData(std::string name) const;
  H5::DataSet openData(std::string name) const;
  void delData(std::string name);

  bool hasGroup(std::string name) const;
  H5::Group openGroup(std::string name) const;
  void delGroup(std::string name);

  H5::Group h5group;
};


}

#endif /* PAN_BASE_CONTAINER_H_INCLUDE */
