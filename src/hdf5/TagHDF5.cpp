// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#include <nix/util/util.hpp>
#include <nix/DataArray.hpp>
#include <nix/hdf5/DataArrayHDF5.hpp>
#include <nix/hdf5/BlockHDF5.hpp>
#include <nix/hdf5/TagHDF5.hpp>
#include <nix/hdf5/FeatureHDF5.hpp>
#include <nix/hdf5/DataSetHDF5.hpp>
#include <nix/Exception.hpp>

using namespace std;
using namespace nix::base;

namespace nix {
namespace hdf5 {


TagHDF5::TagHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const Group &group)
    : BaseTagHDF5(file, block, group)
{
}


TagHDF5::TagHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const Group &group, const string &id,
                             const string &type, const string &name, const std::vector<double> &position)
    : TagHDF5(file, block, group, id, type, name, position, util::getTime())
{
}


TagHDF5::TagHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const Group &group, const string &id,
                             const string &type, const string &name, const std::vector<double> &position, const time_t time)
    : BaseTagHDF5(file, block, group, id, type, name, time)
{
    this->position(position);
}


vector<string> TagHDF5::units() const {
    vector<string> units;
    group().getData("units", units);
    return units;
}


void TagHDF5::units(const vector<string> &units) {
    group().setData("units", units);
    forceUpdatedAt();
}


void TagHDF5::units(const none_t t) {
    if (group().hasData("units")) {
        group().removeData("units");
    }
    forceUpdatedAt();
}


vector<double> TagHDF5::position() const {
    vector<double> position;

    if (group().hasData("position")) {
        group().getData("position", position);
    }

    return position;
}


void TagHDF5::position(const vector<double> &position) {
    group().setData("position", position);
}


vector<double> TagHDF5::extent() const {
    vector<double> extent;
    group().getData("extent", extent);
    return extent;
}


void TagHDF5::extent(const vector<double> &extent) {
    group().setData("extent", extent);
}


void TagHDF5::extent(const none_t t) {
    if (group().hasData("extent")) {
        group().removeData("extent");
    }
    forceUpdatedAt();
}


// Other methods and functions


TagHDF5::~TagHDF5()
{
}

} // ns nix::hdf5
} // ns nix