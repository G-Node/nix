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
  : EntityWithMetadata(file, group, id), block(block), sources_refs(group, "sources")
{
}


EntityWithSources::EntityWithSources(File file, Block block, Group group, string id, time_t time)
  : EntityWithMetadata(file, group, id, time), block(block), sources_refs(group, "sources")
{
}


std::vector<std::string> EntityWithSources::source_ids() const
{
  vector<string> ids;

  if (group.hasData("sources")) {
    DataSet ds = group.openData("sources");
    ds.read(ids, true);
  }

  return ids;
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
  return sources_refs.has(source);
}


bool EntityWithSources::hasSource(string id) const {
  return sources_refs.has(id);
}


vector<Source> EntityWithSources::sources() const {
  vector<string> ids = sources_refs.get();

  vector<Source> source_obj = block.findSources([&](const Source &source) {
    return std::find(ids.begin(), ids.end(), source.id()) != ids.end();
  });

  if (source_obj.size() != ids.size()) {
    // TODO What is the right thing to do here?
    throw runtime_error("Could not resolve all ids");
  }

  return source_obj;
}


void EntityWithSources::addSource(const Source &source) {
  sources_refs.add(source);
}


void EntityWithSources::sources(const vector<Source> &s) {
  vector<string> ids(s.size());
  for (size_t i = 0; i < s.size(); i++) {
    ids[i] = s[i].id();
  }
  sources_refs.set(ids);
}


bool EntityWithSources::removeSource(const Source &source){
  return sources_refs.remove(source);
}


EntityWithSources::~EntityWithSources() {

}

} //namespace
