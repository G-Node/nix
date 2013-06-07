#include <pandora/Util.hpp>
#include <pandora/Source.hpp>
#include <pandora/SourceIterator.hpp>
#include <pandora/SourceTreeIterator.hpp>

using namespace std;

namespace pandora {


Source::Source(const Source &source) :
            PandoraEntity(source.file, source.group, source.entity_id)
{
  sources = source.sources;
}

Source::Source(File *file, Group group, std::string id) :
            PandoraEntity(file, group, id)
{
  sources = group.openGroup("sources");
  }

void Source::parentSource(std::string parentId){
  if(file->hasSource(parentId)){
      group.setAttr("parent_source", parentId);
      forceUpdatedAt();
    }
    else{
      throw std::runtime_error("Source::parentSource(): You try to link to set a parent source that does not exits!");
  }
}

std::string Source::parentSource() const{
  string parentId;
  group.getAttr("parent_source", parentId);
  return parentId;
}

void Source::metadata(std::string sectionId){
  if(file->hasSection(sectionId)){
    group.setAttr("metadata", sectionId);
    forceUpdatedAt();
  }
  else{
    throw std::runtime_error("Source::metadata(): You try to link to a metadata section that does not exist!");
  }
}

std::string Source::metadata() const{
  string sectionId;
  group.getAttr("metadata", sectionId);
  return sectionId;
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

bool Source::hasSource(std::string id, std::string type, uint depth) const{
  bool found = false;
  for(SourceTreeIterator treeIter = treeIterator(type, depth); treeIter != treeIter.end(); ++treeIter){
    Source s = *treeIter;
    if(s.id().compare(id) == 0){
      found = true;
      break;
    }
  }
  return found;
}

bool Source::removeSource(std::string id) {
  bool success = false;
  if(sources.hasGroup(id)){
    sources.removeGroup(id);
    success = true;
  }
  return success;
}

Source::~Source() {
  //dtor
}

} // end namespace pandora
