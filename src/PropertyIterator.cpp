#include <pandora/PropertyIterator.hpp>

using namespace std;

namespace pandora {

PropertyIterator::PropertyIterator(File *file, Group group) :
    file(file), group(group) {
  index = 0;
  size = group.objectCount();
}

PropertyIterator::PropertyIterator(const PropertyIterator &other) :
    file(other.file), group(other.group), index(other.index), size(other.size) {
}

PropertyIterator &PropertyIterator::operator++() {
  index++;
  return *this;
}

PropertyIterator PropertyIterator::begin() const {
  PropertyIterator iter(*this);
  iter.index = 0;
  return iter;
}

PropertyIterator PropertyIterator::end() const {
  PropertyIterator iter(*this);
  iter.index = size;
  return iter;
}

Property PropertyIterator::operator*() const {
  string id;
  if (index < size) {
    id = group.objectName(index);
  } else {
    throw std::range_error(
        "Attempt to access an element that is out of range!");
  }
  Property property(file, group.openGroup(id, false), id);
  return property;
}

void PropertyIterator::operator=(const PropertyIterator &other) {
  file = other.file;
  group = other.group;
  index = other.index;
  size = other.size;
}

bool PropertyIterator::operator==(const PropertyIterator &other) const {
  return group == other.group && index == other.index;
}

bool PropertyIterator::operator!=(const PropertyIterator &other) const {
  return group != other.group || index != other.index;
}

} /* namespace pandora */
