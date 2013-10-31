#include <iostream>

#include <hdf5.h>

#include <pandora/Group.hpp>
#include <pandora/DataSet.hpp>
#include <boost/multi_array.hpp>

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

Group& Group::operator=(const Group &group)
{
  h5group = group.h5group;
  return *this;
}

bool Group::hasAttr(const std::string &name) const {
  return H5Aexists(h5group.getId(), name.c_str());
}

void Group::removeAttr(const std::string &name) const {
  h5group.removeAttr(name);
}


bool Group::hasObject(const std::string &name) const {
  htri_t res = H5Lexists(h5group.getLocId(), name.c_str(), H5P_DEFAULT);
  return res;
}

size_t Group::objectCount() const {
  return h5group.getNumObjs();
}

std::string Group::objectName(size_t index) const {
  return h5group.getObjnameByIdx(index);
}

bool Group::hasData(const std::string &name) const {
  if (hasObject(name)) {
    H5G_stat_t info;
    h5group.getObjinfo(name, info);
    if (info.type == H5G_DATASET) {
      return true;
    }
  }
  return false;
}

void Group::removeData(const std::string &name) {
  if (hasData(name))
    h5group.unlink(name);
}

DataSet Group::openData(const std::string &name) const {
  H5::DataSet ds5 = h5group.openDataSet(name);
  return DataSet(ds5);
}

bool Group::hasGroup(const std::string &name) const {
  if (hasObject(name)) {
    H5G_stat_t info;
    h5group.getObjinfo(name, info);
    if (info.type == H5G_GROUP) {
      return true;
    }
  }
  return false;
}

Group Group::openGroup(const std::string &name, bool create) const {
  Group g;
  if (hasGroup(name)) {
    g = Group(h5group.openGroup(name));
  } else if (create) {
    g = Group(h5group.createGroup(name));
  } else {
    throw std::runtime_error("Unable to open group with name '" + name + "'!");
  }
  return g;
}

void Group::removeGroup(const std::string &name) {
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

Group::~Group() {
  h5group.close();
}
}
