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
      try{
        hsize_t size = {1};
        hsize_t size2 = {100};
        this->h5group.createAttribute(attribs[i].first, PredType::NATIVE_CHAR,H5::DataSpace(1, &size, &size2));
        Attribute a = this->h5group.openAttribute(attribs[i].first);
        DataType dt = H5::StrType();
        string s = "default";
        a.write(PredType::NATIVE_CHAR,s);
      }
      catch (AttributeIException e) {
        std::cout << e.getDetailMsg() << std::endl;
      }
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
