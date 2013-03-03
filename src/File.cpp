#include "File.hpp"

#include <iostream>

using namespace pandora;
using namespace H5;
using namespace std;

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
  this->h5group = h5file.openGroup("/");
  vector< pair<string, DataType > > attribs;
  attribs.push_back(pair<string,DataType>("format",H5::StrType()));
  attribs.push_back(pair<string,DataType>("version",H5::StrType()));
  attribs.push_back(pair<string,DataType>("created_at",H5::StrType()));
  attribs.push_back(pair<string,DataType>("updated_at",H5::StrType()));

  checkAttributes(attribs);


}


void File::checkAttributes(vector<pair<string, DataType> > attribs){
  for(int i = 0; i < (int)attribs.size(); i ++){
    if(!this->attrExists(attribs[i].first)){
      this->setAttr(attribs[i].first,"default");
    }
  }
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
