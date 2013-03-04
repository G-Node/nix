

#include "Block.hpp"
#include "File.hpp"
#include <iostream>

using namespace pandora;


int
main(int args, char **argv)
{
  std::cout << "testing Block" << std::endl;
  H5::H5File file("test_block.h5", H5F_ACC_TRUNC);
  
  H5::Exception::dontPrint();
  
  H5::Group h5group = file.createGroup("/block_test");
  
  Block block(file, h5group);

  block.name("test_name");
  std::cout << block.name() << std::endl;
  
  return 0;
}