#include <File.hpp>

#include <iostream>
#include <time.h>
#include <stdlib.h>

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
  vector< pair<string, string > > attribs;
  attribs.push_back(pair<string,string>("format","pandora"));
  attribs.push_back(pair<string,string>("version","0.9"));
  attribs.push_back(pair<string,string>("created_at","2013-03-05T00:00:00"));
  attribs.push_back(pair<string,string>("updated_at","2013-03-05T00:00:00"));
  vector<string > groups;
  groups.push_back("data");
  groups.push_back("metadata");

  checkAttributes(attribs);
  checkGroups(groups);

}

File::File(const File &other)
{
  //copy ctor
}

void File::checkAttributes(vector<pair<string, string> > attribs){
  for(int i = 0; i < (int)attribs.size(); i ++){
    if(!this->attrExists(attribs[i].first)){
      this->setAttr(attribs[i].first,attribs[i].second);
    }
  }
}

void File::checkGroups(vector< string > groups){
  for(int i = 0; i < (int)groups.size(); i ++){
      if(!this->objectExists(groups[i])){
        this->h5file.createGroup(groups[i],0);
      }
    }
}

void File::close(){

  h5file.close();
}


File::~File()
{

}



/*SEE: File.hpp*/
string File::createId(){
  static const char* hex = "0123456789abcdef";
  string id;
  srand(time(NULL));

  if(!this->prefix.empty()) {
    id.append(this->prefix);
    id.append("_");
    for(int i = 0; i < 64; i++) {
      char c = hex[(size_t)(((double)(rand()))/RAND_MAX*16)];
      id.push_back(c);
    }
  }
  return id;
}
