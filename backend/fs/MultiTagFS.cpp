// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "MultiTagFS.hpp"

#include <nix/NDArray.hpp>
#include <nix/util/util.hpp>
#include <nix/DataArray.hpp>
#include "DataArrayFS.hpp"
#include "BlockFS.hpp"
#include "FeatureFS.hpp"
#include <nix/Exception.hpp>

#include <algorithm>

namespace bfs = boost::filesystem;
namespace nix {
namespace file {


MultiTagFS::MultiTagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                       const std::string &loc)
    : BaseTagFS(file, block, loc)
{
}


MultiTagFS::MultiTagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                       const std::string &loc, const std::string &id, const std::string &type, const std::string &name,
                       const DataArray &positions)
    : MultiTagFS(file, block, loc, id, type, name, positions, util::getTime())
{
}


MultiTagFS::MultiTagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                       const std::string &loc, const std::string &id, const std::string &type, const std::string &name,
                       const DataArray &positions, time_t time)
    : BaseTagFS(file, block, loc, id, type, name, time)
{
    // TODO: the line below currently throws an exception if positions is
    // not in block - to consider if we prefer copying it to the block
    this->positions(positions.id());
}


std::shared_ptr<base::IDataArray> MultiTagFS::positions() const {
    std::shared_ptr<base::IDataArray> da;
    boost::optional<bfs::path> p = findByNameOrAttribute("name", "positions");
    if (p) {
        DataArrayFS a(file(), block(), p->string());
        return std::make_shared<DataArrayFS>(a);
    } else {
        throw std::runtime_error("MultiTagFS::positions: DataArray not found!");
    }
}


void MultiTagFS::positions(const std::string &name_or_id) {
    if (name_or_id.empty())
        throw EmptyString("positions");
    if (!block()->hasEntity({name_or_id, ObjectType::DataArray}))
        throw std::runtime_error("MultiTagFS::positions: DataArray not found in block!");
    if (hasObject("positions"))
        removeObjectByNameOrAttribute("name", "positions");

    auto target = std::dynamic_pointer_cast<DataArrayFS>(block()->getEntity({name_or_id, ObjectType::DataArray}));
    bfs::path p(location()), m("positions");
    target->createLink(p / m);
    forceUpdatedAt();
}


bool MultiTagFS::hasPositions() const {
    // NOTE: other than in positions getter here we do not check that the
    // positions DataArray also exists in block - we just say it does here
    return hasObject("positions");
}


std::shared_ptr<base::IDataArray>  MultiTagFS::extents() const {
    std::shared_ptr<base::IDataArray> da;
    boost::optional<bfs::path> p = findByNameOrAttribute("name", "extents");
    if (p) {
        DataArrayFS a(file(), block(), p->string());
        return std::make_shared<DataArrayFS>(a);
    }
    return da;
}


void MultiTagFS::extents(const std::string &name_or_id) {
    if (name_or_id.empty())
        throw EmptyString("extents");
    if (!block()->hasEntity({name_or_id, ObjectType::DataArray}))
        throw std::runtime_error("MultiTagFS::extents: DataArray not found in block!");
    if (hasObject("extents"))
        removeObjectByNameOrAttribute("name", "extents");
    DataArray da = std::dynamic_pointer_cast<base::IDataArray>(block()->getEntity({name_or_id, ObjectType::DataArray}));
    if (!checkDimensions(da, positions()))
        throw std::runtime_error("MultiTagFS::extents: cannot set Extent because dimensionality of extent and position data do not match!");

    auto target = std::dynamic_pointer_cast<DataArrayFS>(block()->getEntity({name_or_id, ObjectType::DataArray}));
    bfs::path p(location()), m("extents");
    target->createLink(p / m);
    forceUpdatedAt();
}

void MultiTagFS::extents(const none_t t) {
    if (hasObject("extents")) {
        removeObjectByNameOrAttribute("name", "extents");
    }
    forceUpdatedAt();
}


std::vector<std::string> MultiTagFS::units() const {
    std::vector<std::string> units;
    getAttr("units", units);
    return units;
}


void MultiTagFS::units(const std::vector<std::string> &units) {
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


MultiTagFS::~MultiTagFS() {}

} // ns nix::file
} // ns nix
