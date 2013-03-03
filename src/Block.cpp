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
  H5::Attribute attr;
  if (attrExists(name)) {
    attr = h5group.openAttribute("name");
  } else {
    H5::DataSpace fspace;
    H5::AtomType ftype = H5::PredType::C_S1;
    ftype.setSize(name.length());
    attr = h5group.createAttribute("name", ftype, fspace);
  }
  
  H5::StrType memtype = attr.getStrType();
  attr.write(memtype, name.c_str());
}

std::string Block::name() const
{
  std::string name;
  
  H5::Attribute attr = h5group.openAttribute("name");
  H5::StrType memtype = attr.getStrType();
  attr.read(memtype, &name);
  
  return name;
}
