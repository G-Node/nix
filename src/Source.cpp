#include <pandora/Util.hpp>
#include <pandora/Source.hpp>
#include <pandora/SourceIterator.hpp>
#include <pandora/SourceTreeIterator.hpp>

using namespace std;

namespace pandora {


Source::Source(const Source &source) :
            NamedEntity(source.file, source.group, source.entity_id)
{
  sources = source.sources;
}

Source::Source(File *file, Group group, std::string id) :
            NamedEntity(file, group, id)
{
  sources = group.openGroup("sources");
  }

void Source::parentSource(std::string parent_id){
  //if(file->hasSource(parent_id)){
      group.setAttr("parent_source", parent_id);
      forceUpdatedAt();
  //  }
  //  else{
  //    throw std::runtime_error("Source::parentSource(): You try to link to set a parent source that does not exits!");
  //}
}

std::string Source::parentSource() const{
  string parent_id;
  group.getAttr("parent_source", parent_id);
  return parent_id;
}

Source Source::addSource(std::string name, std::string type) {
  string id = util::createId("source");
  Source s(file, sources.openGroup(id,true), id);
  s.name(name);
  s.type(type);
  s.parentSource(this->id());
  return s;
}

bool Source::hasChildren() const {
  SourceIterator iter = this->children();
  return iter != iter.end();
}

SourceIterator Source::children(std::string type) const {
  SourceIterator iter(file, sources, type);
  return iter;
}

SourceTreeIterator Source::treeIterator(std::string type, uint depth) const {
  SourceTreeIterator iter(*this, type, depth);
  return iter;
}

size_t Source::childCount() const {
  size_t childCount = 0;
  for (SourceIterator iter = this->children(); iter != iter.end(); ++iter) {
    childCount++;
  }
  return childCount;
}

bool Source::hasSource(std::string source_id, std::string type, uint depth) const{
  bool found = false;
  for(SourceTreeIterator treeIter = treeIterator(type, depth); treeIter != treeIter.end(); ++treeIter){
    Source s = *treeIter;
    if(s.id().compare(source_id) == 0){
      found = true;
      break;
    }
  }
  return found;
}

Source Source::findSource(std::string source_id, std::string type, uint depth) const{
  for(SourceTreeIterator treeIter = treeIterator(type, depth); treeIter != treeIter.end(); ++treeIter){
    if((*treeIter).id().compare(source_id) == 0){
      Source found = *treeIter;
      return found;
    }
  }
  throw std::runtime_error("Requested Source does not exist! Always check with hasSource!");
}

bool Source::removeSource(std::string source_id) {
  bool success = false;
  if(sources.hasGroup(source_id)){
    sources.removeGroup(source_id);
    success = true;
  }
  return success;
}

Source::~Source() {
  //dtor
}

} // end namespace pandora
