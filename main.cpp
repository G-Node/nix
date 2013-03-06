#include "Block.hpp"
#include "File.hpp"
#include <iostream>

using namespace pandora;


int main(int args, char **argv)
{
  std::cout << "testing Block" << std::endl;
  File file("test_block.h5", "org.g-node");

  H5::Exception::dontPrint();

}
