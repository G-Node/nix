#include <pandora/File.hpp>
#include <pandora/Block.hpp>
#include <exception>

using namespace H5;
using namespace std;

namespace pandora {

const size_t File::READ_ONLY = H5F_ACC_RDONLY;
const size_t File::READ_WRITE = H5F_ACC_RDWR;
const size_t File::OVERWRITE = H5F_ACC_TRUNC;

File::File( string name, string prefix, int mode ) :
  BaseContainer() {
  this->prefix = prefix;
  srand(time(NULL));
  if (fileExists(name)) {
    openHDFFile(name, mode);
    if (!checkFormatAndVersion()) {
      std::string t;
      this->getAttr<std::string>("version",t);
      Block b = this->getBlock(this->blockName(0));
      cout << "BlockTypoe: " << b.type() << endl;
      cout << "VERSION: " << t << endl;
      cout << "VERSION exists: " << this->attrExists("version") << endl;
      cout << "FORAMT: " << this->format() << endl;
    //  h5file.close();
      //throw new runtime_error( "File/Version do not match!");
    }
    checkGroups();
  } else {
    openHDFFile(name, File::OVERWRITE);
    checkAttributes();
    checkGroups();
  }
}

File::File( const File &other ) {
  //copy ctor
  //@todo
}

bool File::fileExists( string name ) const {
  bool exists = false;
  ifstream my_file(name.c_str(), ifstream::in);
  exists = my_file.good();
  my_file.close();
  return exists;
}

bool File::checkFormatAndVersion() const {
  if (!this->attrExists("format") || !this->attrExists("version")) {
    return false;
  }
  if (this->format().compare(FORMAT) != 0) {
    return false;
  }
  return this->version().compare(VERSION) == 0;
}

void File::openHDFFile( string name, int mode ) {
  this->h5file = H5File(name.c_str(), mode);
  this->h5group = h5file.openGroup("/");
}

size_t File::blockCount() const {
  Group g = h5group.openGroup("/data");
  return g.getNumObjs();;
}

std::string File::blockId(size_t index) const{
  if(index >= 0 && index < (size_t)blockCount()){
    Group g = h5group.openGroup("/data");
    std::string name;
    name = g.getObjnameByIdx(index);
    return name;
  }
  throw new runtime_error("Error in File::blockId: Index exceeds number of blocks!");
}

std::string File::blockName( size_t index ) {
  try{
    Block b = getBlock(index);
    return b.name();
  }
  catch (Exception e) {
    throw new runtime_error("Error in File::blockName: Index exceeds number of blocks!");
  }
}

Block File::getBlock( std::string block_id ){
  if (hasBlock(block_id)) {
    std::string s("/data/");
    s.append(block_id);
    Group g = this->openGroup(s);
    Block b(*this, block_id, g);
    return b;
  }
  throw "Error in File::getBlock: File does not contain block with the specified id!";
}

Block File::getBlock( size_t index )  {
  if(index >= 0 && index < (size_t)blockCount()){
    std::string id = blockId(index);
    return getBlock(id);
  }
  throw new runtime_error("Error in File::getBlock: Index exceeds number of blocks!");
}

bool File::hasBlock( std::string block_id ) const {
  if (block_id.length() == 0) {
    return false;
  }
  std::string s("/data/");
  s.append(block_id);
  return this->hasGroup(s);
}

Block File::createBlock( std::string name, std::string type ) {
  Group g = h5group.openGroup("/data");
  std::string id= createId();
  while (hasBlock(id)){
    id = createId();
  }
  Group blockGroup = g.createGroup(id, 0);
  Block b(*this, id, blockGroup);
  b.type(type);
  b.name(name);
  return b;
}

void File::deleteBlock( std::string block_id ) {
  std::string s = "/data/";
  s.append(block_id);
  this->delGroup(s);
}

 void File::deleteBlock( Block &block ) {
   std::string s = "/data/";
   s.append(block.blockId());
   this->delGroup(s);
 }

string File::updated_at() const {
  string t;
  this->getAttr("updated_at", t);
  return t;
}

string File::created_at() const {
  string t;
  this->getAttr("created_at", t);
  return t;
}

string File::version() const {
  string t;
  this->getAttr<std::string>("version", t);
  return t;
}

void File::version( string version ) {
  this->setAttr("version", version);
}

string File::format() const {
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

H5::H5File File::getH5File() const {
  return this->h5file;
}

/*SEE: File.hpp*/
string File::createId() const {
  static const char* hex = "0123456789abcdef";
  string id;
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

string File::time_stamp() const {
  using namespace boost::posix_time;
  time_t t = time(NULL);
  ptime timetmp = from_time_t(t);
  string time_str = to_iso_string(timetmp);
  return time_str;
}

File::~File() {

}

} // end namespace pandora

