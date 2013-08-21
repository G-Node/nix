// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file SimpleTag.cpp
 * @brief Implementations of the class SimpleTag.
 */

#include <pandora/Group.hpp>
#include <pandora/File.hpp>
#include <pandora/Block.hpp>
#include <pandora/DataSet.hpp>
#include <pandora/SimpleTag.hpp>

using namespace std;

namespace pandora {


SimpleTag::SimpleTag(const SimpleTag &tag)
  : EntityWithSources(tag.file, tag.block, tag.group, tag.entity_id)
{}


SimpleTag::SimpleTag(File file, const Block block, Group group, std::string id)
  : EntityWithSources(file, block, group, id)
{}


SimpleTag::SimpleTag(File file, const Block block, Group group, std::string id, time_t time)
  : EntityWithSources(file, block, group, id, time)
{}


vector<string> SimpleTag::units() const {
  vector<string> u;

  if (group.hasData("units")) {
    DataSet ds = group.openData("units");
    ds.read(u, true);
  }

  return u;
}


void SimpleTag::units(vector<string> &u) {
  vector<string> s = {"foo", "bar"};
  DataSet ds((H5::DataSet()));

  if (group.hasData("units")) {
    ds = group.openData("units");
    ds.extend({u.size()});
  } else {
    PSize chunks = {1};
    ds = DataSet::create(group, "units", u, nullptr, &chunks);
  }
}


SimpleTag::~SimpleTag()
{
  // TODO Auto-generated destructor stub
}


} /* namespace pandora */
