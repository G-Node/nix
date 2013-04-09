#include <pandora/ValueIterator.hpp>

using namespace std;

namespace pandora {

ValueIterator::ValueIterator(const Property &property, Group group) :
    property(property), group(group) {
  index = 0;
  size = group.objectCount();
}

ValueIterator::ValueIterator(const ValueIterator &other) :
    property(other.property), group(other.group), index(other.index), size(other.size) {
}

ValueIterator &ValueIterator::operator++() {
  index++;
  return *this;
}

ValueIterator ValueIterator::begin() const {
  ValueIterator iter(*this);
  iter.index = 0;
  return iter;
}

ValueIterator ValueIterator::end() const {
  ValueIterator iter(*this);
  iter.index = size;
  return iter;
}

Value ValueIterator::operator*() const {
  string id;
  if (index < size) {
    id = group.objectName(index);
  } else {
    throw std::range_error(
        "Attempt to access an element that is out of range!");
  }
  Value value(property, group.openGroup(id, false), id);
  return value;
}

void ValueIterator::operator=(const ValueIterator &other) {
  property = other.property;
  group = other.group;
  index = other.index;
  size = other.size;
}

bool ValueIterator::operator==(const ValueIterator &other) const {
  return property.id() == other.property.id() && group == other.group && index == other.index;
}

bool ValueIterator::operator!=(const ValueIterator &other) const {
  return property.id() != other.property.id() || group != other.group || index != other.index;
}

} /* namespace pandora */
