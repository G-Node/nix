#include <iostream>

#include <hdf5.h>

#include <pandora/Group.hpp>
#include <boost/multi_array.hpp>

namespace pandora {

  //
  
  
template<typename T>
struct TypeSpec {

  //const bool is_valid = false; //make compiler cry on unspecified types
  const H5::DataType fileType;
  const H5::DataType memType;
};
  
  
template<>
struct TypeSpec<int> {
    
  const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::STD_I32LE;
  const H5::DataType memType = H5::PredType::NATIVE_INT;
};
  
template<>
struct TypeSpec<double> {
  
  const bool is_valid = true;
  const H5::DataType fileType = H5::PredType::IEEE_F64LE;
  const H5::DataType memType = H5::PredType::NATIVE_DOUBLE;
};
  
template<>
struct TypeSpec<std::string> {
  
  const bool is_valid = true;
  const H5::DataType fileType = H5::StrType(H5::PredType::C_S1, H5T_VARIABLE);
  const H5::DataType memType = H5::StrType(H5::PredType::C_S1, H5T_VARIABLE);
};
  
  //**
  
template<typename T, typename U = T>
class Nyx {
public:
  TypeSpec<U>  m;
  T           &value;
  typedef U base_type;
  
public:
  
  Nyx(T &val) : m(TypeSpec<U>()), value(val) {
    assert(m.is_valid);
  }
  
  virtual H5::DataType getFileType() const { return m.fileType; }
  virtual H5::DataType getMemType() const { return m.memType; }
  virtual H5::DataSpace getDataSpace() const { return H5::DataSpace(); }
  
  virtual void write(H5::Attribute &attr) {
    attr.write(this->m.memType, getData());
  };
  
  virtual void read(H5::Attribute &attr) {
    attr.read(this->m.memType, getData());
  };
  
  virtual base_type* getData() { return nullptr; }; //Throw exception?
  
  virtual ~Nyx() { }
};
  
  
  
template<typename T>
class Charon: public Nyx<T> {
  using typename Nyx<T>::base_type;
  
public:
  Charon(T &val) : Nyx<T>(val) {}
  base_type* getData() { return &this->value; }
};
  
  
template<>
class Charon<std::string> : public Nyx<std::string> {
public:
  Charon(std::string &val) : Nyx<std::string>(val) {}
  
  virtual void write(H5::Attribute &attr) {
    attr.write(this->m.memType, this->value);
  };
  
  virtual void read(H5::Attribute &attr) {
    attr.read(this->m.memType, this->value);
  };
  
};
  
  
template<typename T, int N>
class Charon<boost::multi_array<T, N>> : public Nyx<boost::multi_array<T, N>, T> {
public:
  typedef boost::multi_array<T, N> array_type;
  using typename Nyx<array_type, T>::base_type;
  
  Charon(array_type &value) : Nyx<array_type, T>(value) {}
  //using Nyx<array_type, T>::Nyx;
  
  virtual H5::DataSpace getDataSpace() const {
    const typename array_type::size_type *shape = this->value.shape();
    hsize_t dims[N];
    std::copy(shape, shape + N, dims);
    
    return H5::DataSpace(N, dims, NULL);
  }
  
  base_type* getData() { return this->value.data(); }
};
  
template<int N>
class Charon<boost::multi_array<std::string, N>> :
  public Nyx<boost::multi_array<std::string, N>, std::string> {
public:
  typedef boost::multi_array<std::string, N> array_type;
  
  Charon(array_type &value) : Nyx<array_type, std::string>(value) {}
  
  virtual H5::DataSpace getDataSpace() const {
    const typename array_type::size_type *shape = this->value.shape();
    hsize_t dims[N];
    std::copy(shape, shape + N, dims);
    
    return H5::DataSpace(N, dims, NULL);
  }
  
  virtual void write(H5::Attribute &attr) {
    std::string *vptr = this->value.data();
    auto nelms = this->value.num_elements();
    char const* *data = new char const* [nelms];
    
    for (auto i = 0; i < nelms; i++) {
      data[i] = vptr[i].c_str();
      std::cout << i << " " << data[i] << std::endl;
    }
    
    attr.write(this->m.memType, data);
    delete[] data;
  }
  
  virtual void read(H5::Attribute &attr) {
    std::string *vptr = this->value.data();
    auto nelms = this->value.num_elements();
    char **data = new char *[nelms];
    
    attr.read(this->m.memType, data);
    for (auto i = 0; i < nelms; i++) {
      
      vptr[i] = data[i];
    }
    
    H5::DataSet::vlenReclaim(data, this->m.memType, attr.getSpace());
    delete[] data;
  }
};

  
  
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

  charon.write(attr);
}

template<typename T> bool Group::getAttr(std::string name, T &value) const {

  if (!hasAttr(name)) {
    return false;
  }

  H5::Attribute attr = h5group.openAttribute(name);
  Charon<T> charon = Charon<T>(value);
  charon.read(attr);
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
