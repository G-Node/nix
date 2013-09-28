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

// Methods concerning references.

bool SimpleTag::hasReference(const DataArray &reference) const {
  return hasReference(reference.id());
}

bool SimpleTag::hasReference(const std::string &id) const {
  bool found = false;

  if (referenceCount() > 0) {
    vector<DataArray> refs = references();
    for (size_t i = 0; i < refs.size(); i++) {
      if (refs[i].id().compare(id) == 0) {
        found = true;
        break;
      }
    }
  }

  return found;
}

size_t SimpleTag::referenceCount() const {
  size_t count = 0;
  if (group.hasData("references")) {
    DataSet dataset = group.openData("references");
    PSize size = dataset.size();
    count=  size[0];
  }
  return count;
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

  if (!hasReference(reference)) {
    vector<DataArray> refs = references();
    refs.push_back(reference);
    references(refs);
  }
}

bool SimpleTag::removeReference(const DataArray &reference) {
  bool removed = false;
  vector<DataArray> data_obj = references();

  for (size_t i = 0; i < data_obj.size(); i++) {
    if (data_obj[i].compare(reference) == 0) {
      data_obj.erase(data_obj.begin() + i);
      removed = true;
      break;
    }
  }

  if (removed) {
    references(data_obj);
  }

  return removed;
}

std::vector<DataArray> SimpleTag::references() const {
  vector<string> ids;
  vector<DataArray> data_obj;

  if (group.hasData("references")) {
    DataSet ds = group.openData("references");
    Selection sel = ds.createSelection();
    sel.select({0}, {referenceCount()});
    ds.read(ids, sel, true);
  }

  for (size_t i = 0; i < ids.size(); i++) {
    if (block.hasDataArray(ids[i])) {
      data_obj.push_back(block.getDataArray(ids[i]));
    } else {
      // TODO ???
    }
  }

  return data_obj;
}

void SimpleTag::references(const std::vector<DataArray> &references) {
  vector<string> ids(references.size());

  for (size_t i = 0; i < references.size(); i++) {
    ids[i] = references[i].id();
  }

  if (group.hasData("references")) {
    DataSet ds = group.openData("references");
    ds.extend({ids.size()});
    ds.write(ids);
  } else {
    DataSet ds = DataSet::create(group.h5Group(), "references", ids, &MAX_SIZE_1D, &MIN_CHUNK_SIZE);
    ds.write(ids);
  }
}

// Other methods and functions

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
