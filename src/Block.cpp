#include <pandora/Block.hpp>

using namespace std;

namespace pandora {

Block::Block(const Block &block)
    : BaseContainer(block.h5group), file(block.file)
{

}

Block::Block(File &file, H5::Group h5group)
    : BaseContainer(h5group), file(file)
{

}

Block::~Block() {
  //dtor
}

void Block::type(std::string type) {
  this->setAttr("type", type);
}

std::string Block::type() const {
  std::string str;
  getAttr("type", str);
  return str;
}

void Block::name(std::string name) {
  this->setAttr("name", name);
}

std::string Block::name() const {
  std::string name_out;
  getAttr("name", name_out);
  return name_out;
}

void Block::definition(std::string definition) {
  this->setAttr("definition", definition);
}

std::string Block::definition() const {
  std::string str;
  getAttr("definition", str);
  return str;
}

} // end namespace pandora
