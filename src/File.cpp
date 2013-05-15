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
#include <pandora/TreeIterator.hpp>

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
  metadata = root.openGroup("metadata");
  data = root.openGroup("data");

  if(!checkHeader()) {
    /// @todo throw an exception here
  }
}

/*SEE: File.hpp*/
File::File( const File &file )
: prefix(file.prefix), h5file(file.h5file), metadata(file.metadata), data(file.data)
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
  return metadata;
}

/*SEE: File.hpp*/
bool File::hasBlock(std::string id) const {
  return data.hasGroup(id);
}

/*SEE: File.hpp*/
Block File::getBlock(std::string id) {
  return Block(this, data.openGroup(id, false), id);
}

BlockIterator File::blocks(){
  BlockIterator b(this, data);
  return b;
}

/*SEE: File.hpp*/
Block File::createBlock(string name, string type) {
  string id = util::createId("block");
  while(data.hasObject(id))
    id = util::createId("block");
  Block b(this, data.openGroup(id, true), id);
  b.name(name);
  b.type(type);
  return b;
}

/*SEE: File.hpp*/
void File::deleteBlock(std::string id) {
  if (data.hasGroup(id)) {
    data.removeGroup(id);
  }
}

/*SEE: File.hpp*/
size_t File::blockCount() const {
  return data.objectCount();
}

/*SEE: File.hpp*/
Block File::getBlock(size_t index) {
  string id = data.objectName(index);
  Block b(this, data.openGroup(id), id);
  return b;
}


/*SEE: File.hpp*/
bool File::hasSection(std::string id) {
  bool found = false;
  for(SectionIterator iter = sections(); iter != iter.end(); ++iter){
    if((*iter).id().compare(id) == 0){
      found = true;
      break;
    }
  }
  SectionIterator iter = sections();
  while(!found && iter != iter.end()){
    Section s = *iter;
    found = s.hasSection(id);
    ++iter;
  }
  return found;
}

/*SEE: File.hpp*/
Section File::getSection(std::string id) {
  if(hasSection(id)){
    for(SectionIterator iter = sections(); iter != iter.end(); ++iter){
      if((*iter).id().compare(id) == 0){
        Section found = *iter;
        return found;
      }
    }
    for(SectionIterator iter = sections(); iter != iter.end(); ++iter){
      Section s = *iter;
      for(TreeIterator treeIter = s.treeIterator(); treeIter != treeIter.end(); ++treeIter){
        if((*treeIter).id().compare(id) == 0){
          Section found = *treeIter;
          return found;
        }
      }
    }
  }
  throw std::runtime_error("Requested Section does not exist! Always check with hasSection!");
}

SectionIterator File::sections(){
  SectionIterator iter(this, metadata);
  return iter;
}

/*SEE: File.hpp*/
Section File::createSection(string name, string type, string parent) {
  string id = util::createId("section");
  while(metadata.hasObject(id))
    id = util::createId("section");
  Section s(this, metadata.openGroup(id, true), id);
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
  if(hasSection(id)){
    metadataGroup().removeGroup(id);
    success = true;
  }
  return success;
}

/*SEE: File.hpp*/
size_t File::sectionCount() const {
  return metadata.objectCount();
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

