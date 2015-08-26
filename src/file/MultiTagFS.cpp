// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/file/MultiTagFS.hpp>

#include <nix/NDArray.hpp>
#include <nix/util/util.hpp>
#include <nix/DataArray.hpp>
#include <nix/file/DataArrayFS.hpp>
#include <nix/file/BlockFS.hpp>
#include <nix/file/FeatureFS.hpp>
#include <nix/Exception.hpp>

#include <algorithm>

using namespace std;
using namespace nix::base;

namespace nix {
namespace file {


MultiTagFS::MultiTagFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc)
    : BaseTagFS(file, block, loc)
{
}


MultiTagFS::MultiTagFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc,
                       const string &id, const std::string &type, const string &name, const DataArray &positions)
    : MultiTagFS(file, block, loc, id, type, name, positions, util::getTime())
{
}


MultiTagFS::MultiTagFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc,
                       const std::string &id, const std::string &type, const string &name, const DataArray &positions, time_t time)
    : BaseTagFS(file, block, loc, id, type, name, time)
{
    // TODO: the line below currently throws an exception if positions is
    // not in block - to consider if we prefer copying it to the block
    this->positions(positions.id());
}


shared_ptr<IDataArray> MultiTagFS::positions() const {
    shared_ptr<IDataArray> da;
    boost::optional<boost::filesystem::path> p = findByNameOrAttribute("name", "positions");
    if (p) {
        DataArrayFS a(file(), block(), p->string());
        return make_shared<DataArrayFS>(a);
    }
    return da;
}


void MultiTagFS::positions(const string &name_or_id) {
    if (name_or_id.empty())
        throw EmptyString("positions");
    if (!block()->hasDataArray(name_or_id))
        throw std::runtime_error("MultiTagFS::positions: DataArray not found in block!");
    if (hasObject("positions"))
        removeObjectByNameOrAttribute("name", "positions");

    auto target = dynamic_pointer_cast<DataArrayFS>(block()->getDataArray(name_or_id));

    createDirectoryLink(target->location(), "positions");
    forceUpdatedAt();
}


bool MultiTagFS::hasPositions() const {
    // NOTE: other than in positions getter here we do not check that the
    // positions DataArray also exists in block - we just say it does here
    return hasObject("positions");
}


shared_ptr<IDataArray>  MultiTagFS::extents() const {
    shared_ptr<IDataArray> da;
    boost::optional<boost::filesystem::path> p = findByNameOrAttribute("name", "extents");
    if (p) {
        DataArrayFS a(file(), block(), p->string());
        return make_shared<DataArrayFS>(a);
    }
    return da;
}


void MultiTagFS::extents(const string &name_or_id) {
    if (name_or_id.empty())
        throw EmptyString("extents");
    if (!block()->hasDataArray(name_or_id))
        throw std::runtime_error("MultiTagFS::extents: DataArray not found in block!");
    if (hasObject("extents"))
        removeObjectByNameOrAttribute("name", "extents");

    auto target = dynamic_pointer_cast<DataArrayFS>(block()->getDataArray(name_or_id));
    createDirectoryLink(target->location(), "extents");
    forceUpdatedAt();
}

void MultiTagFS::extents(const none_t t) {
    if (hasObject("extents")) {
        removeObjectByNameOrAttribute("name", "extents");
    }
    forceUpdatedAt();
}


vector<string> MultiTagFS::units() const {
    vector<string> units;
    getAttr("units", units);
    return units;
}


void MultiTagFS::units(const vector<string> &units) {
    setAttr("units", units);
    forceUpdatedAt();
}


void MultiTagFS::units(const none_t t) {
    if (hasAttr("units")) {
        removeAttr("units");
    }
    forceUpdatedAt();
}

// these methods could go to the frontend...
bool MultiTagFS::checkDimensions(const DataArray &a, const DataArray &b)const {
    return a.dataExtent() == b.dataExtent();
}


bool MultiTagFS::checkPositionsAndExtents() const {
    bool valid = true;

    if (hasPositions() && extents() != nullptr) {
        DataArray pos = positions();
        DataArray ext = extents();
        return checkDimensions(pos, ext);
    }

    return valid;
}


MultiTagFS::~MultiTagFS() {}

} // ns nix::file
} // ns nix
