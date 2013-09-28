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
    count = size[0];
  }
  return count;
}


bool EntityWithSources::hasSource(const Source &source) const {
  return hasSource(source.id());
}


bool EntityWithSources::hasSource(string id) const {
  bool found = false;

  if (sourceCount() > 0) {
    vector<Source> s = sources();
    for (size_t i = 0; i < s.size() && !found; i++) {
      if (s[i].id().compare(id) == 0) {
        found = true;
        break;
      }
    }
  }

  return found;
}


vector<Source> EntityWithSources::sources() const {
  vector<string> ids;
  vector<Source> source_obj;

  if (group.hasData("sources")) {
    DataSet ds = group.openData("sources");
    Selection fileSel = ds.createSelection();
    fileSel.select({0}, {sourceCount()});
    ds.read(ids, fileSel, true);
  }

  source_obj = block.findSources([&](const Source &source) {
    return std::find(ids.begin(), ids.end(), source.id()) != ids.end();
  });
  
  if (source_obj.size() != ids.size()) {
    // TODO What is the right thing to do here?
    throw runtime_error("Could not resolve all ids");
  }
  
  return source_obj;
}


void EntityWithSources::addSource(const Source &source) {
  string source_id = source.id();

  if (!block.hasSource(source_id)) {
    throw runtime_error("Unable to find source with id " + source_id +
                        "on block " + block.id());
  }

  if (!hasSource(source_id)) {
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
      Charon<vector<string>> charon(vals);
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
}


void EntityWithSources::sources(vector<Source> s) {
  vector<string> ids;

  for (size_t i = 0; i < s.size(); i++) {
    string id = s[i].id();
    if (block.hasSource(id)) {
      ids.push_back(id);
    } else {
      throw runtime_error("Unable to find source with id " + id +
                              "on block " + block.id());
    }
  }

  sort(ids.begin(), ids.end());

  // TODO It would be so cool if we could avoid/reduce this boiler plate code.
  group.removeData("sources");
  PSize start;
  DataSet ds((H5::DataSet()));
  Charon<vector<string> > charon(ids);
  PSize size = { ids.size() };
  PSize maxsize = { H5S_UNLIMITED };
  PSize chunks = DataSet::guessChunking(size, DataType::Double);
  ds = DataSet::create(group.h5Group(), charon.getFileType(), "sources", size,
      &maxsize, &chunks);
  start = {0};
  Selection fileSel = ds.createSelection();
  PSize count = { ids.size() };
  fileSel.select(count, start);
  ds.write(ids, fileSel);

}


bool EntityWithSources::removeSource(const Source &source){
  bool removed = false;
  vector<Source> source_obj = sources();

  for(size_t i = 0; i < source_obj.size() && !removed; i++){
    if (source_obj[i].compare(source) == 0) {
      source_obj.erase(source_obj.begin() + i);
      removed = true;
    }
  }

  if (removed) {
    sources(source_obj);
  }

  return removed;
}


EntityWithSources::~EntityWithSources() {

}

} //namespace
