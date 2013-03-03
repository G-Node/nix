#include "BaseContainer.hpp"

#include "hdf5.h"

using namespace pandora;

bool BaseContainer::attrExists(std::string name)
{
  return H5Aexists(h5group.getId(), name.c_str());
}

bool BaseContainer::objectExists(std::string path)
{
  htri_t ret = H5Lexists(h5group.getId(), path.c_str(), H5P_DEFAULT);
  if (!ret)
    return false;
  
  ret = H5Oexists_by_name(h5group.getId(), path.c_str(), H5P_DEFAULT);
  return ret;
}

bool BaseContainer::getAttr(std::string name, std::string &value)
{
  if (! attrExists(name)) {
    return false;
  }
  
  H5::Attribute attr = h5group.openAttribute(name);
  H5::StrType memtype = attr.getStrType();
  
  attr.read(memtype, value);
  
  return true;
}

void BaseContainer::setAttr(std::string name, std::string  value)
{
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
  attr.write(memtype, name.c_str());
}