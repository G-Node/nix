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

  setCreatedAt();
  setUpdatedAt();

  if(!checkHeader()) {
    /// TODO throw an exception here
  }
}


File::File(const File &file)
: h5file(file.h5file), root(file.root), metadata(file.metadata), data(file.data)
{}


bool File::hasBlock(const std::string &id) const {
	return data.hasGroup(id);
}

Block File::getBlock(const std::string &id) const {
	return Block(*this, data.openGroup(id, false), id);
}

Block File::getBlock(size_t index) const {
  string id = data.objectName(index);
  Block b(*this, data.openGroup(id), id);
  return b;
}


vector<Block> File::blocks() const {
  vector<Block>  block_obj;

  size_t block_count = data.objectCount();
  for (size_t i = 0; i < block_count; i++) {
    string id = data.objectName(i);
    Block b(*this, data.openGroup(id, false), id);
    block_obj.push_back(b);
  }

  return block_obj;
}


Block File::createBlock(const std::string &name, string type) {
	string id = util::createId("block");
	while(data.hasObject(id))
		id = util::createId("block");
	Block b(*this, data.openGroup(id, true), id);
	b.name(name);
	b.type(type);
	return b;
}


bool File::removeBlock(const std::string &id) {
	if (data.hasGroup(id)) {
		data.removeGroup(id);
		return true;
	} else {
		return false;
	}
}


size_t File::blockCount() const {
  return data.objectCount();
}

std::vector<Section> File::sections()const{
	vector<Section>  section_obj;
	size_t section_count = metadata.objectCount();
	for (size_t i = 0; i < section_count; i++) {
		string id = metadata.objectName(i);
		Section s(*this,metadata.openGroup(id,false), id);
		section_obj.push_back(s);
	}
	return section_obj;
}

bool File::hasSection(const std::string &id) const{
	return metadata.hasGroup(id);
}

Section File::getSection(const std::string &id) const{
	return Section(*this, metadata.openGroup(id, false), id);
}

Section File::getSection(size_t index) const{
	string id = data.objectName(index);
	Section s(*this, metadata.openGroup(id), id);
	return s;
}

std::vector<Section> File::findSection(const std::string &id) const{
	vector<Section> s = sections();
	vector<Section> sects;
	for(size_t i = 0; i < s.size(); i++){
		if(s[i].id().compare(id)==0){
			sects.push_back(s[i]);
			return sects;
		}
	}
	for(size_t i = 0; i < s.size(); i++){
		sects = s[i].findSections([&](const Section &section) {
			bool found = section.id() == id;
			return found;
		});
		if (sects.size() > 0){
			return sects;
		}
	}
	return sects;
}

Section File::createSection(const string &name, const  string &type) {
	string id = util::createId("section");
	while(metadata.hasObject(id))
		id = util::createId("section");
	Section s(*this, metadata.openGroup(id, true), id);
	s.name(name);
	s.type(type);
	return s;
}


bool File::removeSection(const std::string &id){
	bool success = false;

	std::vector<Section> sects = findSection(id);
	if(!sects.empty()){
		metadata.removeGroup(id);
		success = true;
	}
	return success;
}

size_t File::sectionCount() const {
	return metadata.objectCount();
}

time_t File::updatedAt() const {
  string t;
  root.getAttr("updated_at", t);
  return util::strToTime(t);
}

void File::setUpdatedAt() {
  if (!root.hasAttr("updated_at")) {
    time_t t = time(NULL);
    root.setAttr("updated_at", util::timeToStr(t));
  }
}


void File::forceUpdatedAt() {
  time_t t = time(NULL);
  root.setAttr("updated_at", util::timeToStr(t));
}


time_t File::createdAt() const {
  string t;
  root.getAttr("created_at", t);
  return util::strToTime(t);
}


void File::setCreatedAt() {
  if (!root.hasAttr("created_at")) {
    time_t t = time(NULL);
    root.setAttr("created_at", util::timeToStr(t));
  }
}


void File::forceCreatedAt(time_t t) {
  root.setAttr("created_at", util::timeToStr(t));
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
  setUpdatedAt();
  h5file.close();
}


} // end namespace pandora

