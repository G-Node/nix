#include <exception>
#include <fstream>

#include <hdf5.h>

#include <pandora/Util.hpp>
#include <pandora/File.hpp>
#include <pandora/Block.hpp>


using namespace std;

namespace pandora {

const string File::VERSION = "1.0";
const string File::FORMAT  = "pandora";

const size_t File::READ_ONLY  = H5F_ACC_RDONLY;
const size_t File::READ_WRITE = H5F_ACC_RDWR;
const size_t File::OVERWRITE  = H5F_ACC_TRUNC;

File::File(string name, string prefix, int mode)
  : prefix(prefix)
{
  if (fileExists(name)) {
    /// @check if hdf5 file
    h5file = H5::H5File(name.c_str(), mode);
  } else {
    h5file = H5::H5File(name.c_str(), File::OVERWRITE);
  }
  root = Group(h5file.openGroup("/"));
  metadata = root.openGroup("metadata");
  data = root.openGroup("data");

  if(!checkHeader()) {
    /// @todo throw an exception here
  }
}

File::File( const File &file )
  : prefix(file.prefix), h5file(file.h5file), metadata(file.metadata), data(file.data)
{
  // nothing to do
}

bool File::fileExists(string name) const {
  ifstream f(name.c_str());
  if (f) {
    f.close();
    return true;
  } else {
    return false;
  }
}

bool File::hasBlock(std::string id) const {
  return data.hasGroup(id);
}

Block File::getBlock(std::string id) const {
  return Block(*this, data.openGroup(id, false), id);
}

Block File::createBlock(string name, string type) {
  string id = util::createId("block");
  while(data.hasObject(id))
    id = util::createId("block");
  Block b(*this, data.openGroup(id, true), id);
  b.name(name);
  b.type(type);
  return b;
}

void File::deleteBlock(std::string id) {
  if (data.hasGroup(id)) {
    data.delGroup(id);
  }
}

size_t File::blockCount() const {
  return data.objectCount();
}

Block File::getBlock(size_t index) const {
  string id = data.objectName(index);
  Block b(*this, data.openGroup(id), id);
  return b;
}

//
//size_t File::blockCount() const {
//  Group g = h5group.openGroup("/data");
//  return g.getNumObjs();;
//}
//
//std::string File::blockId(size_t index) const{
//  if(index >= 0 && index < (size_t)blockCount()){
//    Group g = h5group.openGroup("/data");
//    std::string name;
//    name = g.getObjnameByIdx(index);
//    return name;
//  }
//  throw new runtime_error("Error in File::blockId: Index exceeds number of blocks!");
//}
//
//std::string File::blockName( size_t index ) {
//  try{
//    Block b = getBlock(index);
//    return b.name();
//  }
//  catch (Exception e) {
//    throw new runtime_error("Error in File::blockName: Index exceeds number of blocks!");
//  }
//}
//
//Block File::getBlock( std::string block_id ){
//  if (hasBlock(block_id)) {
//    std::string s("/data/");
//    s.append(block_id);
//    Group g = this->openGroup(s);
//    Block b(*this, block_id, g);
//    return b;
//  }
//  throw "Error in File::getBlock: File does not contain block with the specified id!";
//}
//
//Block File::getBlock( size_t index )  {
//  if(index >= 0 && index < (size_t)blockCount()){
//    std::string id = blockId(index);
//    return getBlock(id);
//  }
//  throw new runtime_error("Error in File::getBlock: Index exceeds number of blocks!");
//}
//
//bool File::hasBlock( std::string block_id ) const {
//  if (block_id.length() == 0) {
//    return false;
//  }
//  std::string s("/data/");
//  s.append(block_id);
//  return this->hasGroup(s);
//}
//
//Block File::createBlock( std::string name, std::string type ) {
//  Group g = h5group.openGroup("/data");
//  std::string id= createId();
//  while (hasBlock(id)){
//    id = createId();
//  }
//  Group blockGroup = g.createGroup(id, 0);
//  Block b(*this, id, blockGroup);
//  b.type(type);
//  b.name(name);
//  return b;
//}
//
//void File::deleteBlock( std::string block_id ) {
//  std::string s = "/data/";
//  s.append(block_id);
//  this->delGroup(s);
//}
//
// void File::deleteBlock( Block &block ) {
//   std::string s = "/data/";
//   s.append(block.blockId());
//   this->delGroup(s);
// }
//

//
//void File::checkAttributes() {
//  vector<pair<string, string> > attribs;
//  attribs.push_back(pair<string, string> ("format", FORMAT));
//  attribs.push_back(pair<string, string> ("version", VERSION));
//  attribs.push_back(pair<string, string> ("created_at", time_stamp()));
//  attribs.push_back(pair<string, string> ("updated_at", time_stamp()));
//  for (int i = 0; i < (int) attribs.size(); i++) {
//    if (!this->attrExists(attribs[i].first)) {
//      this->setAttr(attribs[i].first, attribs[i].second);
//    }
//  }
//}
//
//
//
//H5::H5File File::getH5File() const {
//  return this->h5file;
//}
//
//
//string File::time_stamp() const {
//  using namespace boost::posix_time;
//  time_t t = time(NULL);
//  ptime timetmp = from_time_t(t);
//  string time_str = to_iso_string(timetmp);
//  return time_str;
//}
//

//void File::close() {
//  this->setAttr("updated_at", time_stamp());
//  h5file.close();
//}


/*SEE: File.hpp*/
time_t File::updatedAt() const {
  string t;
  root.getAttr("updated_at", t);
  return util::strToTime(t);
}

/*SEE: File.hpp*/
time_t File::createdAt() const {
  string t;
  root.getAttr("created_at", t);
  return util::strToTime(t);
}

/*SEE: File.hpp*/
string File::version() const {
  string t;
  root.getAttr<std::string>("version", t);
  return t;
}

/*SEE: File.hpp*/
string File::format() const {
  string t;
  root.getAttr("format", t);
  return t;
}

/*SEE: File.hpp*/
string File::createId() const {
  return util::createId(prefix);
}

/*SEE: File.hpp*/
bool File::checkHeader() {
  bool check = true;
  string str;
  // check format
  if (root.hasAttr("format")) {
    if (!root.getAttr("format", str) || str != FORMAT) {
      check = false;
    }
  } else {
    root.setAttr("format", FORMAT);
  }
  // check version
  if (root.hasAttr("version")) {
    if (!root.getAttr("version", str) || str != VERSION) {
      check = false;
    }
  } else {
    root.setAttr("version", VERSION);
  }
  // check created_at
  if (!root.hasAttr("created_at")) {
    root.setAttr("created_at", util::timeToStr(time(NULL)));
  }
  // check updated_at
  if (!root.hasAttr("updated_at")) {
    root.setAttr("updated_at", util::timeToStr(time(NULL)));
  }
  return check;
}

/*SEE: File.hpp*/
bool File::operator==(const File &other) const {
  return h5file.getFileName() == other.h5file.getFileName();
}

/*SEE: File.hpp*/
bool File::operator!=(const File &other) const {
  return h5file.getFileName() != other.h5file.getFileName();
}

/*SEE: File.hpp*/
File::~File() {
//  root.setAttr("updated_at", util::timeToStr(time(NULL)));
  h5file.close();
}

} // end namespace pandora

