#include "Block.hpp"

using namespace pandora;
using namespace std;

Block::Block(Block &block)
: file(block.file), BaseContainer(block.h5group)
{
  
}

Block::Block(H5::H5File &file, H5::Group h5group)
: file(file), BaseContainer(h5group)
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
