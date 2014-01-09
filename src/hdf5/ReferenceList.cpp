// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/DataSet.hpp>
#include <nix/hdf5/ReferenceList.hpp>

using namespace std;

namespace nix {
namespace hdf5 {

const NDSize ReferenceList::MIN_CHUNK_SIZE = {1};
const NDSize ReferenceList::MAX_SIZE_1D = {H5S_UNLIMITED};


ReferenceList::ReferenceList(const ReferenceList &other)
    : group(other.group), ds_name(other.ds_name)
{}


ReferenceList::ReferenceList(const Group &group, const string &ds_name)
    : group(group), ds_name(ds_name)
{}


bool ReferenceList::has(const string &id) const {

    vector<string> ids = get();
    return std::find(ids.begin(), ids.end(), id) != ids.end();
}


vector<string> ReferenceList::get() const {
    vector<string> ids;

    if (group.hasData(ds_name)) {
        DataSet ds = group.openData(ds_name);
        ds.read(ids, true);
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
        DataSet ds = group.openData(ds_name);
        NDSize old_size = ds.size();
        NDSize new_size = old_size + 1;
        ds.extend(new_size);
        NDSize count = {1};
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
        ds.read(ids, true);

        for (size_t i = 0; i < ids.size(); i++) {
            if (ids[i] == id) {
                ids.erase(ids.begin() + i);
                removed = true;
                break;
            }
        }

        if (removed) {
            NDSize new_size = ds.size();
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


} // namespace hdf5
} // namespace nix
