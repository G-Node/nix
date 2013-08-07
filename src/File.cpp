// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file File.cpp
 * @brief Implementation of all methods from the class File.
 */

#include <fstream>
#include <vector>

#include <hdf5.h>

#include <pandora/Util.hpp>
#include <pandora/File.hpp>
#include <pandora/Block.hpp>
#include <pandora/BlockIterator.hpp>
#include <pandora/Section.hpp>
#include <pandora/SectionIterator.hpp>
#include <pandora/SectionTreeIterator.hpp>

using namespace std;

namespace pandora {

// Format definition
const string File::VERSION = "1.0";
const string File::FORMAT  = "pandora";


static unsigned int map_file_mode(FileMode mode) {
  switch (mode) {
  case FileMode::ReadWrite:
    return H5F_ACC_RDWR;

  case FileMode::ReadOnly:
    return H5F_ACC_RDONLY;

  case FileMode::Overwrite:
    return H5F_ACC_TRUNC;

  default:
    return H5F_ACC_DEFAULT;
  }

}

/*SEE: File.hpp*/
File::File(string name, string prefix, FileMode mode)
: prefix(prefix)
{
  if (!fileExists(name)) {
    mode = FileMode::Overwrite;
  }

  unsigned int h5mode =  map_file_mode(mode);
  h5file = H5::H5File(name.c_str(), h5mode);

  root = Group(h5file.openGroup("/"));
  metadata_group = root.openGroup("metadata");
  data_group = root.openGroup("data");

  if(!checkHeader()) {
    /// @todo throw an exception here
  }
}

/*SEE: File.hpp*/
File::File( const File &file )
: prefix(file.prefix), h5file(file.h5file), metadata_group(file.metadata_group), data_group(file.data_group)
{
  // nothing to do
}

/*SEE: File.hpp*/
bool File::fileExists(string name) const {
  ifstream f(name.c_str());
  if (f) {
    f.close();
    return true;
  } else {
    return false;
  }
}

Group File::metadataGroup() const{
  return metadata_group;
}

/*SEE: File.hpp*/
bool File::hasBlock(std::string id) const {
  return data_group.hasGroup(id);
}

/*SEE: File.hpp*/
Block File::getBlock(std::string id) {
  return Block(this, data_group.openGroup(id, false), id);
}

BlockIterator File::blocks(){
  BlockIterator b(this, data_group);
  return b;
}

/*SEE: File.hpp*/
Block File::createBlock(string name, string type) {
  string id = util::createId("block");
  while(data_group.hasObject(id))
    id = util::createId("block");
  Block b(this, data_group.openGroup(id, true), id);
  b.name(name);
  b.type(type);
  return b;
}

/*SEE: File.hpp*/
void File::deleteBlock(std::string id) {
  if (data_group.hasGroup(id)) {
    data_group.removeGroup(id);
  }
}

/*SEE: File.hpp*/
size_t File::blockCount() const {
  return data_group.objectCount();
}

/*SEE: File.hpp*/
Block File::getBlock(size_t index) {
  string id = data_group.objectName(index);
  Block b(this, data_group.openGroup(id), id);
  return b;
}


/*SEE: File.hpp*/
bool File::hasSection(std::string id, std::string type, uint depth) {
  bool found = false;
  for(SectionIterator iter = sections(); iter != iter.end(); ++iter){
    if((*iter).id().compare(id) == 0){
      found = true;
      break;
    }
  }
  if(depth == 0 || depth > 1){
    SectionIterator iter = sections();
    while(!found && iter != iter.end()){
      Section s = *iter;
      found = s.hasSection(id, type, depth - 1);
      ++iter;
    }
  }
  return found;
}

/*SEE: File.hpp*/
Section File::findSection(std::string id, std::string type, uint depth) {
  if(hasSection(id, type, depth)){
    for(SectionIterator iter = sections(); iter != iter.end(); ++iter){
      if((*iter).id().compare(id) == 0){
        Section found = *iter;
        return found;
      }
    }
    SectionIterator iter = sections();
    while(iter != iter.end()){
      Section s = *iter;
      if(s.hasSection(id)){
        Section found = s.findSection(id, type, depth -1);
        return found;
      }
      ++iter;
    }
  }
  throw std::runtime_error("Requested Section does not exist! Always check with hasSection!");
}

SectionIterator File::sections(){
  SectionIterator iter(this, metadata_group, "");
  return iter;
}

/*SEE: File.hpp*/
Section File::createSection(string name, string type, string parent) {
  string id = util::createId("section");
  while(metadata_group.hasObject(id))
    id = util::createId("section");
  Section s(this, metadata_group.openGroup(id, true), id);
  s.name(name);
  s.type(type);
  if(parent.length() > 0){
    s.parent(parent);
  }
  return s;
}

/*SEE: File.hpp*/
bool File::removeSection(std::string id){
  bool success = false;
  if(hasSection(id,"", 1)){
    metadata_group.removeGroup(id);
    success = true;
  }
  return success;
}

/*SEE: File.hpp*/
size_t File::sectionCount() const {
  return metadata_group.objectCount();
}



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

