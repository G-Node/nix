
#ifndef PAN_GROUP_H_INCLUDE
#define PAN_GROUP_H_INCLUDE

#include <string>
#include <H5Cpp.h>

#include <pandora/Charon.hpp>

namespace pandora {

//forward delcaration
class DataSet;

class Group {

private:

  H5::Group h5group;

public:

  Group();

  Group(H5::Group h5group);

  Group(const Group &group);

  void operator=(const Group &group);

  bool hasAttr(std::string name) const;
  void removeAttr(std::string name) const;

  template <typename T>
  void setAttr(std::string name, const T &value) const;

  template <typename T>
  bool getAttr(std::string name, T &value) const;
  
  bool hasObject(std::string path) const;
  size_t objectCount() const;
  std::string objectName(size_t index) const;

  bool hasData(std::string name) const;
  DataSet openData(std::string name) const;
  void removeData(std::string name);

  bool hasGroup(std::string name) const;
  Group openGroup(std::string name, bool create = true) const;
  void removeGroup(std::string name);

  bool operator==(const Group &group) const;
  bool operator!=(const Group &group) const;

  H5::Group h5Group() const;
  virtual ~Group();
}; // group Group

  
  //template functions
  
template<typename T> void Group::setAttr(std::string name, const T &value) const
{
  Charon<const T> charon(value);
  H5::Attribute attr;
  
  if (hasAttr(name)) {
    attr = h5group.openAttribute(name);
  } else {
    H5::DataType fileType = charon.getFileType();
    H5::DataSpace fileSpace = charon.createDataSpace(false);
    attr = h5group.createAttribute(name, fileType, fileSpace);
  }

  typedef typename Charon<const T>::dbox_type dbox_type;
  dbox_type data = charon.get_data();
  attr.write(charon.getMemType(), *data);
  data.finish();
}

template<typename T> bool Group::getAttr(std::string name, T &value) const
{
  
  if (!hasAttr(name)) {
    return false;
  }

  Charon<T> charon(value);
  H5::Attribute attr = h5group.openAttribute(name);

  H5::DataSpace space = attr.getSpace();
  int rank = space.getSimpleExtentNdims();
  PSize dims(static_cast<size_t>(rank));
  space.getSimpleExtentDims (&dims[0], nullptr);
  charon.resize(dims);

  typedef typename Charon<T>::dbox_type dbox_type;
  dbox_type data = charon.get_data();
  attr.read(charon.getMemType(), *data);
  data.finish(&space);

  return true;
}

}  // namespace pandora


#endif /* PAN_GROUP_H_INCLUDE */
