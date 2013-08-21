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


const PSize SimpleTag::MIN_CHUNK_SIZE = {1};
const PSize SimpleTag::MAX_SIZE_1D = {H5S_UNLIMITED};


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
  vector<string> units;

  if (group.hasData("units")) {
    DataSet ds = group.openData("units");
    ds.read(units, true);
  }

  return units;
}


void SimpleTag::units(vector<string> &units) {
  if (group.hasData("units")) {
    DataSet ds(group.openData("units"));
    ds.extend({units.size()});
    ds.write(units);
  } else {
    DataSet ds(DataSet::create(group.h5Group(), "units", units, &MAX_SIZE_1D, &MIN_CHUNK_SIZE));
    ds.write(units);
  }
}


ostream& operator<<(ostream &out, const SimpleTag &ent) {
  out << "SimpleTag: {name = " << ent.name();
  out << ", type = " << ent.type();
  out << ", id = " << ent.id() << "}";
  return out;
}


SimpleTag::~SimpleTag()
{
  // TODO Auto-generated destructor stub
}


} /* namespace pandora */
