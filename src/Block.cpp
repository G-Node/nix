#include "Block.hpp"

using namespace pandora;
using namespace std;

Block::Block(const Block &block)
: BaseContainer(block.h5group), file(block.file)
{
  
}

Block::Block(File &file, H5::Group h5group)
: BaseContainer(h5group), file(file)
{
  
}

Block::~Block()
{
  //dtor
}

void Block::name(std::string name)
{
  this->setAttr("name", name);
}

std::string Block::name() const
{
  std::string name_out;
  getAttr("name", name_out);
  return name_out;
}
