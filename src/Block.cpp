
#include <pandora/Block.hpp>

using namespace std;

namespace pandora {


/* SEE: SimpleTag.hpp */
Block::Block(const Block &block) :
    NamedEntity(block.file, block.group, block.entity_id), source_group(block.source_group)
{
}

/* SEE: SimpleTag.hpp */
Block::Block(File *file, Group group, std::string id) :
    NamedEntity(file, group, id)
{
  source_group = group.openGroup("sources");
}

/*SEE: File.hpp*/
Source Block::createSource(string name, string type, string parent_id) {
  string id = util::createId("source");
  while(source_group.hasObject(id))
    id = util::createId("source");
  Source s(file, source_group.openGroup(id, true), id);
  s.name(name);
  s.type(type);
  if(parent_id.length() > 0){
    s.parentSource(parent_id);
  }
  return s;
}

SourceIterator Block::sources() {
  SourceIterator iter(file, source_group, "");
  return iter;
}

bool Block::hasSource(std::string id, std::string type, uint depth){
  bool found = false;
  for(SourceIterator iter = sources(); iter != iter.end(); ++iter){
    if((*iter).id().compare(id) == 0){
      found = true;
      break;
    }
  }
  if(depth == 0 || depth > 1){
    SourceIterator iter = sources();
    while(!found && iter != iter.end()){
      Source s = *iter;
      found = s.hasSource(id, type, depth - 1);
      ++iter;
    }
  }
  return found;
}

/*SEE: File.hpp*/
Source Block::findSource(std::string source_id, std::string type, uint depth) {
  if(hasSource(source_id, type, depth)){
    for(SourceIterator iter = sources(); iter != iter.end(); ++iter){
      if((*iter).id().compare(source_id) == 0){
        Source found = *iter;
        return found;
      }
    }
    SourceIterator iter = sources();
    while(iter != iter.end()){
      Source s = *iter;
      if(s.hasSource(source_id)){
        Source found = s.findSource(source_id, type, depth -1);
        return found;
      }
      ++iter;
    }
  }
  throw std::runtime_error("Requested Source does not exist! Always check with hasSource!");
}

bool Block::removeSource(std::string id){
  bool success = false;
  if(hasSource(id,"", 1)){
    source_group.removeGroup(id);
    success = true;
  }
  return success;
}

/*SEE: File.hpp*/
size_t Block::sourceCount() const {
  return source_group.objectCount();
}


Block::~Block() {
  //dtor
}

} // end namespace pandora
