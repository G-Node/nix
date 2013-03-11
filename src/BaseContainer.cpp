
#include <iostream>

#include <hdf5.h>

#include <pandora/BaseContainer.hpp>

namespace pandora {

bool BaseContainer::attrExists( std::string name ) const {
  return H5Aexists(h5group.getId(), name.c_str());
}

bool BaseContainer::objectExists( std::string path ) const {
  htri_t ret = H5Lexists(h5group.getId(), path.c_str(), H5P_DEFAULT);
  if (!ret)
    return false;

  //ret = H5Oexists_by_name(h5group.getId(), path.c_str(), H5P_DEFAULT);
  return true;
}

void BaseContainer::delAttr( std::string name ) const {
  h5group.removeAttr(name);
}


//Oh the hacks ;-)  

struct TypeSpec {
  H5::DataType fileType;
  H5::DataType memType;
  
};
  
template <typename T>
TypeSpec type_klassify()
{
  TypeSpec e;
  //FIXME throw runtime exception here
  return e;
};
  
template <> TypeSpec type_klassify<int>() { return {H5::PredType::STD_I32LE, H5::PredType::NATIVE_INT}; };
template <> TypeSpec type_klassify<double>() { return {H5::PredType::IEEE_F32LE, H5::PredType::NATIVE_DOUBLE}; };
template <> TypeSpec type_klassify<std::string>() { return {H5::PredType::C_S1, H5::PredType::C_S1}; };
  
template <typename T>
class Nyx {
public:
  Nyx(T &val) : value(val), m(type_klassify<T>()) { }
  virtual H5::DataSpace getDataSpace() const {
    return H5::DataSpace();
  }
  virtual H5::DataType getFileType() const { return m.fileType; }
  virtual H5::DataType getMemType() const { return m.memType; }
  
  virtual void writeAttribute(H5::Attribute &attr) {
    attr.write(m.memType, &value);
  }
  
  virtual void readAttribute(H5::Attribute &attr) {
    attr.read(m.memType, &value);
  }
  
  virtual ~Nyx() {}
protected:
  T &value;
  TypeSpec m;
};
  
template <typename T>
class Charon : public Nyx<T> {
public:
  Charon(T &val) : Nyx<T>(val) {}
};

template <>
class Charon<std::string> : public Nyx<std::string> {
public:
  Charon(std::string &val) : Nyx<std::string>(val) {}
  virtual H5::DataType getFileType() const {
    H5::AtomType ftype = H5::PredType::C_S1;
    ftype.setSize(value.length());
    return ftype;
  }
  virtual void writeAttribute(H5::Attribute &attr) {
    attr.write(m.memType, value);
  }
  
  virtual void readAttribute(H5::Attribute &attr) {
    attr.read(m.memType, value);
  }
  
  virtual ~Charon() {}
};


template <typename T>
void BaseContainer::setAttr(std::string name, T value) const
{
  H5::Attribute attr;
  Charon<T> charon = Charon<T>(value);
  
  if (attrExists(name)) {
    attr = h5group.openAttribute(name);
  } else {
    H5::DataType fileType = charon.getFileType();
    H5::DataSpace fileSpace = charon.getDataSpace();
    attr = h5group.createAttribute(name, fileType, fileSpace);
  }
  
  charon.writeAttribute(attr);
}
 
template <typename T>
bool BaseContainer::getAttr (std::string name, T &value) const {
  
  if (!attrExists(name)) {
    return false;
  }
  
  H5::Attribute attr = h5group.openAttribute(name);
  Charon<T> charon = Charon<T>(value);
  charon.readAttribute(attr);
  
  return true;
}

template void BaseContainer::setAttr<int>(std::string name, int value) const;
template void BaseContainer::setAttr<double>(std::string name, double value) const;
template void BaseContainer::setAttr<std::string>(std::string name, std::string value) const;
  
template bool BaseContainer::getAttr<int>(std::string name, int &value) const;
template bool BaseContainer::getAttr<double>(std::string name, double &value) const;
template bool BaseContainer::getAttr<std::string>(std::string name, std::string &value) const;

  
bool BaseContainer::getAttr(std::string name, std::string &value) const {
  return getAttr<std::string>(name, value);
}

void BaseContainer::setAttr(std::string name, std::string value) const {
  setAttr<std::string>(name, value);
}
  
bool BaseContainer::hasData( std::string name ) const {
  try {
    H5::DataSet data = h5group.openDataSet(name);
    data.close();
    return true;
  } catch (H5::Exception e) {
    return false;
  }
}

void BaseContainer::delData( std::string name ) {
  if (hasData(name))
    h5group.unlink(name);
}

H5::DataSet BaseContainer::openData( std::string name ) const {
  H5::DataSet data = h5group.openDataSet(name);
  return data;
}

bool BaseContainer::hasGroup( std::string name ) const {
  try {
    H5::Group g = h5group.openGroup(name);
    g.close();
    return true;
  } catch (H5::Exception e) {
    return false;
  }

}

H5::Group BaseContainer::openGroup( std::string name ) const {
  H5::Group g = h5group.openGroup(name);
  return g;
}

void BaseContainer::delGroup( std::string name ) {
  if (hasGroup(name))
    h5group.unlink(name);
}

}
