#include <pandora/BlockIterator.hpp>

using namespace std;

namespace pandora {


BlockIterator::BlockIterator(File *file, Group group)
  : file(file), group(group)
{
  index = 0;
  size  = group.objectCount();
}

BlockIterator::BlockIterator(const BlockIterator &other)
  : file(other.file), group(other.group), index(other.index), size(other.size)
{
}

BlockIterator &BlockIterator::operator++() {
  index++;
  return *this;
}

BlockIterator BlockIterator::begin() const {
  BlockIterator iter(*this);
  iter.index = 0;
  return iter;
}

BlockIterator BlockIterator::end() const {
  BlockIterator iter(*this);
  iter.index = size;
  return iter;
}

Block BlockIterator::operator*() const {
  string id;
  if (index  < size) {
    id = group.objectName(index);
  } else {
    id = group.objectName(size - 1);
  }
  Block block(file, group.openGroup(id, false), id);
  return block;
}

void BlockIterator::operator=(const BlockIterator &other) {
  file  = other.file;
  group = other.group;
  index = other.index;
  size  = other.size;
}

bool BlockIterator::operator==(const BlockIterator &other) const {
  return group == other.group && index == other.index;
}

bool BlockIterator::operator!=(const BlockIterator &other) const {
  return group != other.group || index != other.index;
}


} /* namespace pandora */
