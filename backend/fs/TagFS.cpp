// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TagFS.hpp"

namespace nix {
namespace file {


TagFS::TagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc)
    : BaseTagFS(file, block, loc)
{
}


TagFS::TagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
             const std::string &loc, const std::string &id, const std::string &type, const std::string &name,
             const std::vector<double> &position)
    : TagFS(file, block, loc, id, type, name, position, util::getTime())
{
}


TagFS::TagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
             const std::string &loc, const std::string &id, const std::string &type, const std::string &name,
             const std::vector<double> &position, const time_t time)
    : BaseTagFS(file, block, loc, id, type, name, time)
{
    this->position(position);
}


std::vector<std::string> TagFS::units() const {
    std::vector<std::string> units;
    getAttr("units", units);
    return units;
}


void TagFS::units(const std::vector<std::string> &units) {
    setAttr("units", units);
    forceUpdatedAt();
}


void TagFS::units(const none_t t) {
    if (hasAttr("units")) {
        removeAttr("units");
    }
    forceUpdatedAt();
}


std::vector<double> TagFS::position() const {
    std::vector<double> position;
    if (hasAttr("position")) {
        getAttr("position", position);
    }
    return position;
}


void TagFS::position(const std::vector<double> &position) {
    setAttr("position", position);
}


std::vector<double> TagFS::extent() const {
    std::vector<double> extent;
    getAttr("extent", extent);
    return extent;
}


void TagFS::extent(const std::vector<double> &extent) {
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
