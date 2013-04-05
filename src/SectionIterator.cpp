#include <pandora/SectionIterator.hpp>
#include <cstring>
#include <stdexcept>

using namespace std;

namespace pandora {


SectionIterator::SectionIterator(File *file, Group group, std::string parent_id)
: file(file), group(group), parent(parent_id)
{
  index = 0;
  size  = group.objectCount();
  index = nextIndex(0);
  first = index;
}

SectionIterator::SectionIterator(const SectionIterator &other)
: file(other.file), group(other.group), index(other.index), size(other.size),parent(other.parent), first(other.first)
{
}

SectionIterator &SectionIterator::operator++() {
  index = nextIndex(index+1);
  return *this;
}

SectionIterator SectionIterator::begin() const {
  SectionIterator iter(*this);
  iter.index = first;
  return iter;
}

SectionIterator SectionIterator::end() const {
  SectionIterator iter(*this);
  iter.index = size;
  return iter;
}

size_t SectionIterator::nextIndex(size_t start) const{
  size_t idx = size;
  for(size_t i = start; i < size; i++){
    std::string name = group.objectName(i);
    Group g = group.openGroup(name);
    if(matchesParent(g)){
      idx = i;
      break;
    }
  }
  return idx;
}

size_t SectionIterator::lastIndex() const{
  size_t idx = size;
  for(size_t i = size-1; (i+1) > 0; i--){
    std::string name = group.objectName(i);
    Group g = group.openGroup(name);
    if(matchesParent(g)){
      idx = i;
      break;
    }
  }
  return idx;
}

bool SectionIterator::matchesParent(Group group) const {
  bool hasParent = group.hasAttr("parent");
  if(parent.length() == 0 && !hasParent){
    return true;
  }
  std::string value;
  if(hasParent){
    group.getAttr("parent",value);
  }
  if(parent.length() == 0 && hasParent && value.length() == 0){
    return true;
  }
  if(parent.length() > 0 && value.length() > 0){
    if(value.compare(parent) == 0){
      return true;
    }
  }
  return false;
}

Section SectionIterator::operator*() const {
  string id;
  if (index  < size) {
    id = group.objectName(index);
  } else {
    throw std::range_error("Attempt to access an element that is out of range!");
   // id = group.objectName(lastIndex());
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
