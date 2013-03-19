#include <pandora/SectionIterator.hpp>

using namespace std;

namespace pandora {


SectionIterator::SectionIterator(File file, Group group)
  : file(file), group(group)
{
  index = 0;
  size  = group.objectCount();
}

SectionIterator::SectionIterator(const SectionIterator &other)
  : file(other.file), group(other.group), index(other.index), size(other.size)
{
}

SectionIterator &SectionIterator::operator++() {
  index++;
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
