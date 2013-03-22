#include <pandora/SectionIterator.hpp>
#include <iostream>
#include <cstring>

using namespace std;

namespace pandora {

extern "C"
herr_t section_info(hid_t loc_id, const char *name, const H5L_info_t *linfo, void *opdata)
{
  char* str;
  std::string s(*((char**)opdata));
  herr_t value = 0;
  std::string aname = "parent";
  hid_t group = H5Gopen2(loc_id, name, H5P_DEFAULT);
  htri_t exists = H5Aexists( group, aname.c_str());
  if (s.length() == 0 && !exists){
    cerr << "Iterator attrib check passed" << endl;
    value = 1;
  }
  else if(exists){
    cerr << "Iterator attrib check " << endl;
    hid_t attr_id = H5Aopen(group, aname.c_str(), H5P_DEFAULT);
    hid_t type_id = H5Aget_type(attr_id);
    hid_t class_id = H5Tget_class(type_id);
    hid_t space_id = H5Aget_space(attr_id);
    int size = H5Tget_size(type_id);
    int ndim = H5Sget_simple_extent_ndims(space_id);
    H5Tclose(type_id);
    H5Sclose(space_id);
    if(ndim == 1 && class_id == H5T_STRING) {
      str = new char[size + 1];
      type_id = H5Tcopy(H5T_C_S1);
      H5Tset_size(type_id, size + 1);
      herr_t stat = H5Aread(attr_id, type_id, str);
      if(stat < 0) {
        delete[] str;
        str = NULL;
      }
      H5Tclose(type_id);
    }
    H5Aclose(attr_id);
    std::string s2(str);
    if (s2.compare(s) == 0){
      value = 1;
      cerr << "Iterator attrib check passed" << endl;
    }
  }

  H5Gclose(group);
  return value;
}

SectionIterator::SectionIterator(File file, Group group, std::string parent_id)
: file(file), group(group), parent(parent_id)
{
  index = 0;
  size  = group.objectCount();
}

SectionIterator::SectionIterator(const SectionIterator &other)
: file(other.file), group(other.group), index(other.index), size(other.size),parent(other.parent)
{
}

SectionIterator &SectionIterator::operator++() {
  char *s = strdup(parent.c_str());
  hsize_t idx = index;
  hsize_t *pt = &idx;
  H5Literate(group.h5Group().getId(),H5_INDEX_NAME,H5_ITER_INC, pt, section_info, &s);
  index = *pt;
  return *this;
}

SectionIterator SectionIterator::begin() const {
  SectionIterator iter(*this);
  iter.index = 0;
  return iter;
}

SectionIterator SectionIterator::end() const {
  SectionIterator iter(*this);
  iter.index = size;
  return iter;
}

Section SectionIterator::operator*() const {
  string id;
  if (index  < size) {
    id = group.objectName(index);
  } else {
    id = group.objectName(size - 1);
  }
  Section section(file, group.openGroup(id, false), id);
  return section;
}

void SectionIterator::operator=(const SectionIterator &other) {
  file  = other.file;
  group = other.group;
  index = other.index;
  size  = other.size;
}

bool SectionIterator::operator==(const SectionIterator &other) const {
  return group == other.group && index == other.index;
}

bool SectionIterator::operator!=(const SectionIterator &other) const {
  return group != other.group || index != other.index;
}




} /* namespace pandora */
