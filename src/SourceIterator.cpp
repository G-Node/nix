#include <pandora/SourceIterator.hpp>

using namespace std;

namespace pandora {

SourceIterator::SourceIterator(File *file, Group group, std::string type) :
        file(file), group(group), type(type){
  index = 0;
  size = group.objectCount();
  index = nextIndex(0);
  first = index;
}

SourceIterator::SourceIterator(const SourceIterator &other) :
    file(other.file), group(other.group), type(other.type), index(other.index), size(other.size){
}

SourceIterator &SourceIterator::operator++() {
  index = nextIndex(index+1);
  return *this;
}

SourceIterator SourceIterator::begin() const {
  SourceIterator iter(*this);
  iter.index = first;
  return iter;
}

SourceIterator SourceIterator::end() const {
  SourceIterator iter(*this);
  iter.index = size;
  return iter;
}

size_t SourceIterator::nextIndex(size_t start) const{
  size_t idx = size;
  for(size_t i = start; i < size; i++){
    std::string name = group.objectName(i);
    Group g = group.openGroup(name);
    if(matchesType(g)){
      idx = i;
      break;
    }
  }
  return idx;
}

size_t SourceIterator::lastIndex() const{
  size_t idx = size;
  for(size_t i = size-1; (i+1) > 0; i--){
    std::string name = group.objectName(i);
    Group g = group.openGroup(name);
    if(matchesType(g)){
      idx = i;
      break;
    }
  }
  return idx;
}

bool SourceIterator::matchesType(Group group) const {
  bool hasType = group.hasAttr("type");
  if(!hasType){
    return false;
  }
  if(type.length() == 0){
    return true;
  }
  std::string value;
  if(hasType){
    group.getAttr("type",value);
  }
  return (value.compare(type) == 0);
}

Source SourceIterator::operator*() const {
  string id;
  if (index  < size) {
    id = group.objectName(index);
  } else {
    throw std::range_error("Attempt to access an element that is out of range!");
  }
  Source source(file, group.openGroup(id, false), id);
  return source;
}

void SourceIterator::operator=(const SourceIterator &other) {
  group = other.group;
  index = other.index;
  size = other.size;
}

bool SourceIterator::operator==(const SourceIterator &other) const {
  return group == other.group && index == other.index;
}

bool SourceIterator::operator!=(const SourceIterator &other) const {
  return group != other.group || index != other.index;
}

} /* namespace pandora */
