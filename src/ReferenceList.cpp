// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <pandora/util/ReferenceList.hpp>
#include <pandora/PSize.hpp>
#include <pandora/Selection.hpp>
#include <pandora/DataSet.hpp>

using namespace std;

namespace pandora {
namespace util {

const PSize ReferenceList::MIN_CHUNK_SIZE = {1};
const PSize ReferenceList::MAX_SIZE_1D = {H5S_UNLIMITED};

ReferenceList::ReferenceList(const ReferenceList &other)
  : group(other.group), ds_name(other.ds_name)
{}

ReferenceList::ReferenceList(const Group &group, const string &ds_name)
  : group(group), ds_name(ds_name)
{}

bool ReferenceList::has(const string &id) const {
  bool found = false;

  vector<string> ids = get();
  for (size_t i = 0; i < ids.size(); i++) {
    if (ids[i] == id) {
      found = true;
      break;
    }
  }

  return found;
}

vector<string> ReferenceList::get() const {
  vector<string> ids;

  if (group.hasData(ds_name)) {
    DataSet ds = group.openData(ds_name);
    Selection sel = ds.createSelection();
    sel.select({0}, ds.size());
    ds.read(ids, sel, true);
  }

  return ids;
}

void ReferenceList::set(const vector<string> &ids) {
  if (group.hasData(ds_name)) {
    DataSet ds = group.openData(ds_name);
    ds.extend({ids.size()});
    ds.write(ids);
  } else {
    DataSet ds = DataSet::create(group.h5Group(), ds_name, ids,
                                 &MAX_SIZE_1D, &MIN_CHUNK_SIZE);
    ds.write(ids);
  }
}


void ReferenceList::add(const string &id) {
  vector<string> new_ids = {id};

  if (group.hasData(ds_name)) {
    DataSet ds = group.openData("sources");
    PSize old_size = ds.size();
    PSize new_size = old_size + 1;
    ds.extend(new_size);
    PSize count = {1};
    Selection sel = ds.createSelection();
    sel.select(count, old_size);
    ds.write(new_ids, sel);
  } else {
    DataSet ds = DataSet::create(group.h5Group(), ds_name, new_ids,
                                 &MAX_SIZE_1D, &MIN_CHUNK_SIZE);
    ds.write(new_ids);
  }
}

bool ReferenceList::remove(const string &id) {
  bool removed = false;

  if (group.hasData(ds_name)) {
    vector<string> ids;
    DataSet ds = group.openData(ds_name);
    Selection sel = ds.createSelection();
    sel.select({0}, ds.size());
    ds.read(ids, sel, true);

    for (size_t i = 0; i < ids.size(); i++) {
      if (ids[i] == id) {
        ids.erase(ids.begin() + i);
        removed = true;
        break;
      }
    }

    if (removed) {
      PSize new_size = ds.size();
      ds.extend(--new_size);
      ds.write(ids);
    }
  }

  return removed;
}

bool ReferenceList::operator==(const ReferenceList &other) const {
  return group == other.group && ds_name == other.ds_name;
}

bool ReferenceList::operator!=(const ReferenceList &other) const {
  return !(*this == other);
}

ReferenceList& ReferenceList::operator=(const ReferenceList &other) {
  if (*this != other) {
    this->group = other.group;
    this->ds_name = other.ds_name;
  }
  return *this;
}

ReferenceList::~ReferenceList() {}

} // namespace util
} // namespace pandora
