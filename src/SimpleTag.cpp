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
#include <pandora/Representation.hpp>

using namespace std;

namespace pandora {


const PSize SimpleTag::MIN_CHUNK_SIZE = {1};
const PSize SimpleTag::MAX_SIZE_1D = {H5S_UNLIMITED};


SimpleTag::SimpleTag(const SimpleTag &tag)
  : EntityWithSources(tag.file, tag.block, tag.group, tag.entity_id)
{
  representation_group = tag.representation_group;
}


SimpleTag::SimpleTag(File file, const Block block, Group group, std::string id)
  : EntityWithSources(file, block, group, id)
{
  representation_group = group.openGroup("representations");
}


SimpleTag::SimpleTag(File file, const Block block, Group group, std::string id, time_t time)
  : EntityWithSources(file, block, group, id, time)
{
  representation_group = group.openGroup("representations");
}


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
    DataSet ds = group.openData("units");
    ds.extend({units.size()});
    ds.write(units);
  } else {
    DataSet ds = DataSet::create(group.h5Group(), "units", units, &MAX_SIZE_1D, &MIN_CHUNK_SIZE);
    ds.write(units);
  }
}


vector<double> SimpleTag::position() const {
  vector<double> position;

  if (group.hasData("position")) {
    DataSet ds = group.openData("position");
    ds.read(position, true);
  }

  return position;
}


void SimpleTag::position(const vector<double> &position) {
  if (group.hasData("position")) {
    DataSet ds = group.openData("position");
    ds.extend({position.size()});
    ds.write(position);
  } else {
    DataSet ds = DataSet::create(group.h5Group(), "position", position, &MAX_SIZE_1D, &MIN_CHUNK_SIZE);
    ds.write(position);
  }
}


vector<double> SimpleTag::extent() const {
  vector<double> extent;

  if (group.hasData("extent")) {
    DataSet ds = group.openData("extent");
    ds.read(extent, true);
  }

  return extent;
}


void SimpleTag::extent(const vector<double> &extent) {
  if (group.hasData("extent")) {
    DataSet ds = group.openData("extent");
    ds.extend({extent.size()});
    ds.write(extent);
  } else {
    DataSet ds = DataSet::create(group.h5Group(), "extent", extent, &MAX_SIZE_1D, &MIN_CHUNK_SIZE);
    ds.write(extent);
  }
}


SimpleTag& SimpleTag::operator=(const SimpleTag &other) {
  if (*this != other) {
    this->file = other.file;
    this->block = other.block;
    this->group = other.group;
    this->entity_id = other.entity_id;
    this->representation_group = other.representation_group;
  }
  return *this;
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
