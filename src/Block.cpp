// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file src/Block.cpp
 * @brief Implementation of methods and functions related to the class Block.
 */

#include <pandora/Util.hpp>
#include <pandora/Group.hpp>
#include <pandora/File.hpp>
#include <pandora/Block.hpp>
#include <pandora/Source.hpp>
#include <pandora/SimpleTag.hpp>

using namespace std;

namespace pandora {


Block::Block(const Block &block)
  : EntityWithMetadata(block.file, block.group, block.entity_id),
    source_group(block.source_group), data_array_group(block.data_array_group),
    simple_tag_group(block.simple_tag_group), data_tag_group(block.data_tag_group)
{
  // nothing to do
}


Block::Block(File file, Group group, string id)
  : EntityWithMetadata(file, group, id)
{
  source_group = group.openGroup("sources");
  data_array_group = group.openGroup("data_arrays");
  simple_tag_group = group.openGroup("simple_tags");
  data_tag_group = group.openGroup("data_tags");
}


Block::Block(File file, Group group, string id, time_t time)
  : EntityWithMetadata(file, group, id, time)
{
  source_group = group.openGroup("sources");
  data_array_group = group.openGroup("data_arrays");
  simple_tag_group = group.openGroup("simple_tags");
  data_tag_group = group.openGroup("data_tags");
}


bool Block::hasSource(string id) const {
  return source_group.hasGroup(id);
}


Source Block::getSource(string id) const {
  return Source(file, source_group.openGroup(id, false), id);
}


Source Block::getSource(size_t index) const {
  string id = source_group.objectName(index);
  return Source(file, source_group.openGroup(id, false), id);
}


bool Block::existsSource(string id) const {

  if (hasSource(id)) {
    return true;
  } else {
    vector<Source> stack;
    vector<Source> tmp(sources());
    stack.insert(stack.end(), tmp.begin(), tmp.end());

    bool found = false;
    while(!found && stack.size() > 0) {
      Source s(stack.back());
      stack.pop_back();

      if (s.hasSource(id)) {
        found = true;
      } else {
        vector<Source> tmp = s.sources();
        stack.insert(stack.end(), tmp.begin(), tmp.end());
      }
    }

    return found;
  }
}


Source Block::findSource(string id) const {

  if (sourceCount() == 0) {
    throw runtime_error("Unable to find the source with id " + id + "!");
  }

  if (hasSource(id)) {
    return getSource(id);
  } else {
    vector<Source> stack;
    vector<Source> tmp(sources());
    stack.insert(stack.end(), tmp.begin(), tmp.end());
    Source result(stack[0]);

    bool found = false;
    while(!found && stack.size() > 0) {
      Source s(stack.back());
      stack.pop_back();

      if (s.hasSource(id)) {
        found = true;
        result = s.getSource(id);
      } else {
        vector<Source> tmp(s.sources());
        stack.insert(stack.end(), tmp.begin(), tmp.end());
      }
    }

    if (!found) {
      throw runtime_error("Unable to find the source with id " + id + "!");
    }

    return result;
  }
}


size_t Block::sourceCount() const {
  return source_group.objectCount();
}


std::vector<Source> Block::sources() const {
  vector<Source> source_obj;

  size_t source_count = source_group.objectCount();
  for (size_t i = 0; i < source_count; i++) {
    string id = source_group.objectName(i);
    Source s(file, source_group.openGroup(id, false), id);
    source_obj.push_back(s);
  }

  return source_obj;
}


Source Block::createSource(string name, string type) {
  string id = util::createId("source");

  while(source_group.hasObject(id)) {
    id = util::createId("source");
  }

  Source s(file, source_group.openGroup(id, true), id);
  s.name(name);
  s.type(type);

  return s;
}

SimpleTag Block::createSimpleTag(string name, string type) {
  string id = util::createId("simple_tag");

  while(simple_tag_group.hasObject(id)) {
    id = util::createId("simple_tag");
  }

  SimpleTag st(file, *this, simple_tag_group.openGroup(id, true), id);
  st.name(name);
  st.type(type);

  return st;
}
/*
size_t Block::dataArrayCount()const {
  return data_group.objectCount();
}

DataArray Block::createDataArray(std::string name, std::string type){
  string id = util::createId("data_array");
  while(data_group.hasObject(id))
    id = util::createId("data_array");
  DataArray da(*file, data_group.openGroup(id, true), id);
  da.name(name);
  da.type(type);
  return da;
}

DataArrayIterator Block::dataArrays(){
  DataArrayIterator iter(file, data_group);
  return iter;
}

bool Block::hasDataArray(std::string data_array_id){
  for(DataArrayIterator iter = dataArrays(); iter != iter.end(); ++iter){
    if((*iter).id().compare(data_array_id) == 0){
      return true;
    }
  }
  return false;
}

DataArray Block::getDataArray(std::string data_array_id){
  for(DataArrayIterator iter = dataArrays(); iter != iter.end(); ++iter){
    if((*iter).id().compare(data_array_id) == 0){
      DataArray found = *iter;
      return found;
    }
  }
  throw std::runtime_error("Requested DataArray does not exist! Always check with hasDataArray!");
}

void Block::removeDataArray(std::string data_array_id){
  if(hasDataArray(data_array_id)){
     data_group.removeGroup(data_array_id);
   }
}
*/

Block& Block::operator=(const Block &other) {
  if (*this != other) {
      this->file = other.file;
      this->group = other.group;
      this->entity_id = other.entity_id;
      this->source_group = other.source_group;
      this->data_array_group = other.data_array_group;
      this->simple_tag_group = other.simple_tag_group;
      this->data_tag_group = other.data_tag_group;
    }
    return *this;
}

ostream& operator<<(ostream &out, const Block &ent) {
  out << "Block: {name = " << ent.name();
  out << ", type = " << ent.type();
  out << ", id = " << ent.id() << "}";
  return out;
}


Block::~Block() {}

} // end namespace pandora
