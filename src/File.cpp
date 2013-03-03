#include "File.hpp"



using namespace pandora;
using namespace H5;

File::File(std::string name, std::string prefix, std::string mode)
{
  if(mode == "r")
	{
	  this->h5file = H5File(name.c_str(), H5F_ACC_RDONLY);
	}
	else{
	  this->h5file = H5File(name.c_str(), H5F_ACC_TRUNC);
	}
	this->prefix = prefix;
//	Group g = h5file.openGroup("/");



}

void File::close(){
  h5file.close();
}


File::~File()
{

}

File::File(const File &other)
{
  //copy ctor
}
