// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/file/TagFS.hpp>

#include <nix/util/util.hpp>
#include <nix/DataArray.hpp>
#include <nix/file/DataArrayFS.hpp>
#include <nix/file/BlockFS.hpp>
#include <nix/file/FeatureFS.hpp>
#include <nix/Exception.hpp>

using namespace std;
using namespace nix::base;

namespace nix {
namespace file {


TagFS::TagFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc)
    : BaseTagFS(file, block, loc)
{
}


TagFS::TagFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc, const string &id,
             const string &type, const string &name, const std::vector<double> &position)
    : TagFS(file, block, loc, id, type, name, position, util::getTime())
{
}


TagFS::TagFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc, const string &id,
             const string &type, const string &name, const std::vector<double> &position, const time_t time)
    : BaseTagFS(file, block, loc, id, type, name, time)
{
    this->position(position);
}


vector<string> TagFS::units() const {
    vector<string> units;
    getAttr("units", units);
    return units;
}


void TagFS::units(const vector<string> &units) {
    setAttr("units", units);
    forceUpdatedAt();
}


void TagFS::units(const none_t t) {
    if (hasAttr("units")) {
        removeAttr("units");
    }
    forceUpdatedAt();
}


vector<double> TagFS::position() const {
    vector<double> position;
    if (hasAttr("position")) {
        getAttr("position", position);
    }
    return position;
}


void TagFS::position(const vector<double> &position) {
    setAttr("position", position);
}


vector<double> TagFS::extent() const {
    vector<double> extent;
    getAttr("extent", extent);
    return extent;
}


void TagFS::extent(const vector<double> &extent) {
    setAttr("extent", extent);
}


void TagFS::extent(const none_t t) {
    if (hasAttr("extent")) {
        removeAttr("extent");
    }
    forceUpdatedAt();
}


// Other methods and functions


TagFS::~TagFS()
{
}

} // ns nix::file
} // ns nix
