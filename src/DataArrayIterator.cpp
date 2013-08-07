#include "pandora/DataArrayIterator.hpp"

using namespace std;

namespace pandora {


DataArrayIterator::DataArrayIterator(File *file, Group group)
  : file(file), group(group)
{
  index = 0;
  size  = group.objectCount();
}

DataArrayIterator::DataArrayIterator(const DataArrayIterator &other)
  : file(other.file), group(other.group), index(other.index), size(other.size)
{
}

DataArrayIterator &DataArrayIterator::operator++() {
  index++;
  return *this;
}

DataArrayIterator DataArrayIterator::begin() const {
  DataArrayIterator iter(*this);
  iter.index = 0;
  return iter;
}

DataArrayIterator DataArrayIterator::end() const {
  DataArrayIterator iter(*this);
  iter.index = size;
  return iter;
}

DataArray DataArrayIterator::operator*() const {
  string id;
  if (index  < size) {
    id = group.objectName(index);
  } else {
    id = group.objectName(size - 1);
  }

  DataArray dataArray(*file, group.openGroup(id, false), id);
  return dataArray;
}

void DataArrayIterator::operator=(const DataArrayIterator &other) {
  file  = other.file;
  group = other.group;
  index = other.index;
  size  = other.size;
}

bool DataArrayIterator::operator==(const DataArrayIterator &other) const {
  return group == other.group && index == other.index;
}

bool DataArrayIterator::operator!=(const DataArrayIterator &other) const {
  return group != other.group || index != other.index;
}


} /* namespace pandora */
