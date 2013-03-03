#include "File.hpp"

#include <string>

using namespace pandora;

File::File(std::string name, std::string prefix, std::string mode)
{
	  this->h5file = H5::H5File(name.c_str(), 0);
}

File::~File()
{
  //dtor
}

File::File(const File &other)
{
  //copy ctor
}
