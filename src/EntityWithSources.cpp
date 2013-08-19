// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file EntityWithSources.cpp
 * @brief Implementation of all methods of the class EntityWithSources.
 */


#include <pandora/Util.hpp>
#include <pandora/Group.hpp>
#include <pandora/DataSet.hpp>
#include <pandora/PSize.hpp>
#include <pandora/File.hpp>
#include <pandora/Block.hpp>
#include <pandora/Source.hpp>
#include <pandora/EntityWithSources.hpp>

using namespace std;

namespace pandora {


EntityWithSources::EntityWithSources(File file, Block block, Group group, string id)
  : EntityWithMetadata(file, group, id), block(block)
{

}

EntityWithSources::EntityWithSources(File file, Block block, Group group, string id, time_t time)
  : EntityWithMetadata(file, group, id, time), block(block)
{

}


size_t EntityWithSources::sourceCount() const {
  size_t count = 0;
  if (group.hasData("sources")) {
    DataSet dataset = group.openData("sources");
    PSize size = dataset.size();
    return size[0];
  }
  return count;
}

/*
bool EntityWithSources::hasSource(string source_id) const {
  if (group.hasData("sources")) {
    if (sourceCount() == 0)
      return false;
    vector<string> s = sources();
    for (size_t i = 0; i < s.size(); i++) {
      if (s[i].compare(source_id) == 0) {
        return true;
      }
    }
  } else {
    return false;
  }
  return false;
}


vector<string> EntityWithSources::sources() const {
  vector<string> s;
  if (group.hasData("sources")) {
    DataSet ds = group.openData("sources");
    Selection fileSel = ds.createSelection();
    PSize start = { 0 };
    PSize count = { sourceCount() };
    fileSel.select(count, start);
    ds.read(s, fileSel, true);
  }
  return s;
}


void EntityWithSources::addSource(const Source &source) {
  addSource(source.id());
}


void EntityWithSources::addSource(string source_id) {
  if(hasSource(source_id)){
    return;
  }
  vector<string> vals;
  vals.push_back(source_id);
  PSize start;
  DataSet ds((H5::DataSet()));
  if (group.hasData("sources")) {
    ds = group.openData("sources");
    PSize size = ds.size();
    PSize newSize = size + 1;
    ds.extend(newSize);
    start = size;
  } else {
    Charon<vector<string> > charon(vals);
    PSize size = { 1 };
    PSize maxsize = { H5S_UNLIMITED };
    PSize chunks = DataSet::guessChunking(size, DataType::Double);
    ds = DataSet::create(group.h5Group(), charon.getFileType(), "sources", size,
        &maxsize, &chunks);
    start = {0};
  }
  Selection fileSel = ds.createSelection();
  PSize count = { 1 };
  fileSel.select(count, start);
  ds.write(vals, fileSel);
}


void EntityWithSources::sources(vector<string> s){
  group.removeData("sources");
  PSize start;
  DataSet ds((H5::DataSet()));
  Charon<vector<string> > charon(s);
  PSize size = { s.size() };
  PSize maxsize = { H5S_UNLIMITED };
  PSize chunks = DataSet::guessChunking(size, DataType::Double);
  ds = DataSet::create(group.h5Group(), charon.getFileType(), "sources", size,
      &maxsize, &chunks);
  start = {0};
  Selection fileSel = ds.createSelection();
  PSize count = { s.size() };
  fileSel.select(count, start);
  ds.write(s, fileSel);
}


void EntityWithSources::removeSource(string source_id){
  vector<string> s = sources();
  for(size_t i = 0; i < s.size(); i++){
    if (s[i].compare(source_id) == 0) {
      s.erase(s.begin()+i);
      return;
    }
  }
}
*/

  EntityWithSources::~EntityWithSources() {

  }

} //namespace
