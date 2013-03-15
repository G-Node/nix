
#ifndef PAN_GROUP_H_INCLUDE
#define PAN_GROUP_H_INCLUDE

#include <string>
#include <H5Cpp.h>


namespace pandora {

class Group {

private:

  H5::Group h5group;

public:

  Group();

  Group(H5::Group h5group);

  Group(const Group &group);

  void operator=(const Group &group);

  bool hasAttr(std::string name) const;
  void delAttr(std::string name) const;

  template <typename T>
  void setAttr(std::string name, T value) const;

  template <typename T>
  bool getAttr(std::string name, T &value) const;
  
  bool hasObject(std::string path) const;
  size_t objectCount() const;
  std::string objectName(size_t index) const;

  bool hasData(std::string name) const;
  H5::DataSet openData(std::string name) const;
  void delData(std::string name);

  bool hasGroup(std::string name) const;
  Group openGroup(std::string name, bool create = true) const;
  void delGroup(std::string name);

  bool operator==(const Group &group) const;
  bool operator!=(const Group &group) const;

  virtual ~Group();
}; // group Group

}  // namespace pandora

#endif /* PAN_GROUP_H_INCLUDE */
