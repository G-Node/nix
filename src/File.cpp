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


File::File(string name, FileMode mode)
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


File::File( const File &file )
  : h5file(file.h5file), metadata(file.metadata), data(file.data)
{}


bool File::hasBlock(std::string id) const {
  return data.hasGroup(id);
}


Block File::getBlock(std::string id) const {
  return Block(*this, data.openGroup(id, false), id);
}

Block File::getBlock(size_t index) const {
  string id = data.objectName(index);
  Block b(*this, data.openGroup(id), id);
  return b;
}


/** TODO implement vector<Block> File::blocks() const {} */


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
    data.removeGroup(id);
  }
}


size_t File::blockCount() const {
  return data.objectCount();
}

/*
bool File::hasSection(std::string id, std::string type, uint depth) const {
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


Section File::findSection(std::string id, std::string type, uint depth) const {
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
*/


/* TODO implement vector<Section> File::sections() const {} */

/*
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


bool File::removeSection(std::string id){
  bool success = false;
  if(hasSection(id,"", 1)){
    metadata.removeGroup(id);
    success = true;
  }
  return success;
}


size_t File::sectionCount() const {
  return metadata.objectCount();
}
*/



time_t File::updatedAt() const {
  string t;
  root.getAttr("updated_at", t);
  return util::strToTime(t);
}


time_t File::createdAt() const {
  string t;
  root.getAttr("created_at", t);
  return util::strToTime(t);
}


string File::version() const {
  string t;
  root.getAttr<std::string>("version", t);
  return t;
}


string File::format() const {
  string t;
  root.getAttr("format", t);
  return t;
}


bool File::checkHeader() const {
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


bool File::fileExists(string name) const {
  ifstream f(name.c_str());
  if (f) {
    f.close();
    return true;
  } else {
    return false;
  }
}


bool File::operator==(const File &other) const {
  return h5file.getFileName() == other.h5file.getFileName();
}


bool File::operator!=(const File &other) const {
  return h5file.getFileName() != other.h5file.getFileName();
}


File& File::operator=(const File &other) {
  if (*this != other) {
    this->h5file = other.h5file;
    this->root = other.root;
    this->metadata = other.metadata;
    this->data = other.data;
  }
  return *this;
}


File::~File() {
  h5file.close();
}


} // end namespace pandora

