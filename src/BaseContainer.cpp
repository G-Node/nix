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

bool BaseContainer::getAttr( std::string name, std::string &value ) const {
  if (!attrExists(name)) {
    return false;
  }

  H5::Attribute attr = h5group.openAttribute(name);
  H5::StrType memtype = attr.getStrType();

  attr.read(memtype, value);

  return true;
}

void BaseContainer::setAttr( std::string name, std::string value ) const {
  H5::Attribute attr;
  if (attrExists(name)) {
    attr = h5group.openAttribute(name);
  } else {
    H5::DataSpace fspace;
    H5::AtomType ftype = H5::PredType::C_S1;
    ftype.setSize(value.length());
    attr = h5group.createAttribute(name, ftype, fspace);
  }

  H5::StrType memtype = attr.getStrType();
  attr.write(memtype, value);
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
