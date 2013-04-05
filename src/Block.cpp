#include <pandora/Block.hpp>
#include <pandora/Util.hpp>

using namespace std;

namespace pandora {

Block::Block(const Block &block)
    : file(block.file), group(block.group), block_id(block.block_id)
{
  // nothing to do
}

Block::Block(File *file, Group group, string id)
    : file(file), group(group), block_id(id)
{
  // nothing to do
}

string Block::id() const{
 return block_id;
}

void Block::type(string type) {
  group.setAttr("type", type);
}

string Block::type() const {
  string type;
  group.getAttr("type", type);
  return type;
}

void Block::name(string name) {
  group.setAttr("name", name);
}

string Block::name() const {
  string name;
  group.getAttr("name", name);
  return name;
}

void Block::definition(string definition) {
  group.setAttr("definition", definition);
}

string Block::definition() const {
  string definition;
  group.getAttr("definition", definition);
  return definition;
}

void Block::date(time_t date) {
  group.setAttr("date", util::timeToStr(date));
}

time_t Block::date() const {
  string t;
  group.getAttr("date", t);
  return util::strToTime(t);
}

bool Block::operator==(const Block &other) const {
  return block_id ==  other.block_id;
}

bool Block::operator!=(const Block &other) const {
  return block_id != other.block_id;
}

Block::~Block() {
  //dtor
}

} // end namespace pandora
