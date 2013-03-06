#include <File.hpp>

#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace pandora;
using namespace H5;
using namespace std;

const size_t File::READ_ONLY  = H5F_ACC_RDONLY;
const size_t File::READ_WRITE = H5F_ACC_RDWR;
const size_t File::OVERWRITE  = H5F_ACC_TRUNC;

File::File( string name, string prefix, int mode ) {
  this->prefix = prefix;
  if (fileExists(name)) {
    openHDFFile(name, mode);
    if (!checkFormatAndVersion()) {
      h5file.close();
      //Throw exception
    }
    checkGroups();
  } else {
    openHDFFile(name, File::OVERWRITE);
    checkAttributes();
    checkGroups();
  }
}

bool File::fileExists( string name ) const{
  bool exists = false;
  ifstream my_file(name.c_str(), ifstream::in);
  exists = my_file.good();
  my_file.close();
  return exists;
}

bool File::checkFormatAndVersion() const{
  string value;
  if (!this->attrExists("format") || !this->attrExists("version")) {
    return false;
  }
  this->getAttr("format", value);
  if (value.compare(FORMAT) != 0) {
    return false;
  }
  this->getAttr("version", value);
  return value.compare(VERSION) == 0;
}

void File::openHDFFile( string name, int mode ) {
  this->h5file = H5File(name.c_str(), mode);
  this->h5group = h5file.openGroup("/");
}

File::File( const File &other ) {
  //copy ctor
}

string File::updated_at() const{
  string t;
  this->getAttr("updated_at", t);
  return t;
}

string File::created_at() const {
  string t;
  this->getAttr("created_at", t);
  return t;
}

string File::version() const{
  string t;
  this->getAttr("version", t);
  return t;
}

void File::version( string version ) {
  this->setAttr("version", version);
}

string File::format() const{
  string t;
  this->getAttr("format", t);
  return t;
}

void File::format( string format ) {
  this->setAttr("format", format);
}

void File::checkAttributes() {
  vector<pair<string, string> > attribs;
  attribs.push_back(pair<string, string> ("format", FORMAT));
  attribs.push_back(pair<string, string> ("version", VERSION));
  attribs.push_back(pair<string, string> ("created_at", time_stamp()));
  attribs.push_back(pair<string, string> ("updated_at", time_stamp()));
  for (int i = 0; i < (int) attribs.size(); i++) {
    if (!this->attrExists(attribs[i].first)) {
      this->setAttr(attribs[i].first, attribs[i].second);
    }
  }
}

void File::checkGroups() {
  vector<string> groups;
  groups.push_back("data");
  groups.push_back("metadata");
  for (int i = 0; i < (int) groups.size(); i++) {
    if (!this->objectExists(groups[i])) {
      this->h5file.createGroup(groups[i], 0);
    }
  }
}

void File::close() {
  this->setAttr("updated_at", time_stamp());
  h5file.close();
}

/*SEE: File.hpp*/
string File::createId() const{
  static const char* hex = "0123456789abcdef";
  string id;
  srand(time(NULL));

  if (!this->prefix.empty()) {
    id.append(this->prefix);
    id.append("_");
    for (int i = 0; i < 64; i++) {
      char c = hex[(size_t) (((double) (rand())) / RAND_MAX * 16)];
      id.push_back(c);
    }
  }
  return id;
}

string File::time_stamp() const{
  using namespace boost::posix_time;
  time_t t = time(NULL);
  ptime timetmp = from_time_t(t);
  string time_str = to_iso_string(timetmp);
  return time_str;
}

File::~File() {

}
