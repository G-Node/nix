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
#include <pandora/DataArray.hpp>
#include <pandora/Representation.hpp>

using namespace std;

namespace pandora {


const PSize SimpleTag::MIN_CHUNK_SIZE = {1};
const PSize SimpleTag::MAX_SIZE_1D = {H5S_UNLIMITED};


SimpleTag::SimpleTag(const SimpleTag &tag)
  : EntityWithSources(tag.file, tag.block, tag.group, tag.entity_id),
    references_list(tag.group, "references")
{
  representation_group = tag.representation_group;
}


SimpleTag::SimpleTag(File file, const Block block, Group group, std::string id)
  : EntityWithSources(file, block, group, id), references_list(group, "references")
{
  representation_group = group.openGroup("representations");
}


SimpleTag::SimpleTag(File file, const Block block, Group group, std::string id, time_t time)
  : EntityWithSources(file, block, group, id, time), references_list(group, "references")
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

// Methods concerning references.

bool SimpleTag::hasReference(const DataArray &reference) const {
  return references_list.has(reference);
}

bool SimpleTag::hasReference(const std::string &id) const {
  return references_list.has(id);
}

size_t SimpleTag::referenceCount() const {
  return references_list.count();
}

DataArray SimpleTag::getReference(const std::string &id) const {
  if (hasReference(id)) {
    return block.getDataArray(id);
  } else {
    throw runtime_error("No reference with id: " + id);
  }
}

void SimpleTag::addReference(const DataArray &reference) {
  string reference_id = reference.id();

  if (block.hasDataArray(reference_id)) {
    throw runtime_error("Unable to find data array with reference_id " +
                        reference_id + " on block " + block.id());
  }

  references_list.add(reference_id);
}

bool SimpleTag::removeReference(const DataArray &reference) {
  return references_list.remove(reference);
}

std::vector<DataArray> SimpleTag::references() const {
  vector<string> ids = references_list.get();
  vector<DataArray> refs;

  for (size_t i = 0; i < ids.size(); i++) {
    if (block.hasDataArray(ids[i])) {
      refs.push_back(block.getDataArray(ids[i]));
    }
  }

  return refs;
}

void SimpleTag::references(const std::vector<DataArray> &references) {
  vector<string> ids(references.size());

  for (size_t i = 0; i < references.size(); i++) {
    ids[i] = references[i].id();
  }

  references_list.set(ids);
}

// Other methods and functions

SimpleTag& SimpleTag::operator=(const SimpleTag &other) {
  if (*this != other) {
    this->file = other.file;
    this->block = other.block;
    this->group = other.group;
    this->entity_id = other.entity_id;
    this->representation_group = other.representation_group;
    this->sources_refs = other.sources_refs;
    this->references_list = other.references_list;
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
