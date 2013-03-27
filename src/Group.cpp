#include <iostream>

#include <hdf5.h>

#include <pandora/Group.hpp>

namespace pandora {

Group::Group()
    : h5group()
{}

Group::Group(H5::Group h5group)
    : h5group(h5group)
{}

Group::Group(const Group &group)
    : h5group(group.h5group)
{}

void Group::operator=(const Group &group)
{
  h5group = group.h5group;
}

bool Group::hasAttr(std::string name) const {
  return H5Aexists(h5group.getId(), name.c_str());
}

void Group::removeAttr(std::string name) const {
  h5group.removeAttr(name);
}

//Oh the hacks ;-)  

struct TypeSpec
{
  H5::DataType fileType;
  H5::DataType memType;
};

template<typename T> TypeSpec type_klassify() {
  TypeSpec e;
  //FIXME throw runtime exception here
  return e;
}


template<> TypeSpec type_klassify<int>() {
  return {H5::PredType::STD_I32LE, H5::PredType::NATIVE_INT};
}

template<> TypeSpec type_klassify<double>() {
  return {H5::PredType::IEEE_F32LE, H5::PredType::NATIVE_DOUBLE};
}

template<> TypeSpec type_klassify<std::string>() {
  return {H5::PredType::C_S1, H5::PredType::C_S1};
}


template<typename T> class Nyx
{

protected:

  T &value;
  TypeSpec m;

public:

  Nyx(T &val)
      : value(val), m(type_klassify<T>())
  {}

  virtual H5::DataSpace getDataSpace() const {
    return H5::DataSpace();
  }

  virtual H5::DataType getFileType() const {
    return m.fileType;
  }

  virtual H5::DataType getMemType() const {
    return m.memType;
  }

  virtual void writeAttribute(H5::Attribute &attr) {
    attr.write(m.memType, &value);
  }

  virtual void readAttribute(H5::Attribute &attr) {
    attr.read(m.memType, &value);
  }

  virtual ~Nyx() {}

};

template<typename T> class Charon: public Nyx<T>
{
public:
  Charon(T &val)
    : Nyx<T>(val)
  {}
};

template<> class Charon<std::string> : public Nyx<std::string>
{

public:
  Charon(std::string &val)
    : Nyx<std::string>(val)
  {}

  virtual H5::DataType getFileType() const {
    H5::AtomType ftype = H5::PredType::C_S1;
    ftype.setSize(value.length());
    return ftype;
  }

  virtual void writeAttribute(H5::Attribute &attr) {
    H5::StrType memtype = attr.getStrType();
    attr.write(memtype, value);
    //attr.write(m.memType, value); //FIXME This is just a quick hack...
  }

  virtual void readAttribute(H5::Attribute &attr) {
    H5::StrType memtype = attr.getStrType();
    //attr.read(m.memType, value);
    attr.read(memtype, value); //FIXME This is just a quick hack...
  }

  virtual ~Charon() {}
};

template<typename T> void Group::setAttr(std::string name, T value) const {
  H5::Attribute attr;
  Charon<T> charon = Charon<T>(value);

  if (hasAttr(name)) {
    attr = h5group.openAttribute(name);
  } else {
    H5::DataType fileType = charon.getFileType();
    H5::DataSpace fileSpace = charon.getDataSpace();
    attr = h5group.createAttribute(name, fileType, fileSpace);
  }

  charon.writeAttribute(attr);
}

template<typename T> bool Group::getAttr(std::string name, T &value) const {

  if (!hasAttr(name)) {
    return false;
  }

  H5::Attribute attr = h5group.openAttribute(name);
  Charon<T> charon = Charon<T>(value);
  charon.readAttribute(attr);

  return true;
}

template void Group::setAttr<int>(std::string name, int value) const;

template void Group::setAttr<double>(std::string name, double value) const;

template void Group::setAttr<std::string>(std::string name, std::string value) const;

template bool Group::getAttr<int>(std::string name, int &value) const;

template bool Group::getAttr<double>(std::string name, double &value) const;

template bool Group::getAttr<std::string>(std::string name, std::string &value) const;

bool Group::hasObject(std::string name) const {
  hsize_t num = h5group.getNumObjs();
  for (hsize_t i = 0; i < num; i++) {
    if (h5group.getObjnameByIdx(i) == name)
      return true;
  }
  return false;
}

size_t Group::objectCount() const {
  return h5group.getNumObjs();
}

std::string Group::objectName(size_t index) const {
  return h5group.getObjnameByIdx(index);
}

bool Group::hasData(std::string name) const {
  if (hasObject(name)) {
    H5G_stat_t info;
    h5group.getObjinfo(name, info);
    if (info.type == H5G_DATASET) {
      return true;
    }
  }
  return false;
}

void Group::removeData(std::string name) {
  if (hasData(name))
    h5group.unlink(name);
}

H5::DataSet Group::openData(std::string name) const {
  H5::DataSet data = h5group.openDataSet(name);
  return data;
}

bool Group::hasGroup(std::string name) const {
  if (hasObject(name)) {
    H5G_stat_t info;
    h5group.getObjinfo(name, info);
    if (info.type == H5G_GROUP) {
      return true;
    }
  }
  return false;
}

Group Group::openGroup(std::string name, bool create) const {
  Group g;
  if (hasGroup(name)) {
    g = Group(h5group.openGroup(name));
  } else if (create) {
    g = Group(h5group.createGroup(name));
  }
  return g;
}

void Group::removeGroup(std::string name) {
  if (hasGroup(name))
    h5group.unlink(name);
}

bool Group::operator==(const Group &group) const {
  return h5group.getLocId() == group.h5group.getLocId();
}

bool Group::operator!=(const Group &group) const {
  return h5group.getLocId() != group.h5group.getLocId();
}

H5::Group Group::h5Group() const {
  return h5group;
}

Group::~Group() {}

}
