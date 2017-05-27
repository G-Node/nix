// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "MultiTagHDF5.hpp"

#include <nix/NDArray.hpp>
#include <nix/util/util.hpp>
#include "DataArrayHDF5.hpp"
#include "BlockHDF5.hpp"
#include "FeatureHDF5.hpp"

using namespace nix::base;

namespace nix {
namespace hdf5 {


MultiTagHDF5::MultiTagHDF5(const std::shared_ptr<IFile> &file, const std::shared_ptr<IBlock> &block, const H5Group &group)
    : BaseTagHDF5(file, block, group)
{
}


MultiTagHDF5::MultiTagHDF5(const std::shared_ptr<IFile> &file, const std::shared_ptr<IBlock> &block, const H5Group &group,
                           const std::string &id, const std::string &type, const std::string &name, const DataArray &positions)
    : MultiTagHDF5(file, block, group, id, type, name, positions, util::getTime())
{
}


MultiTagHDF5::MultiTagHDF5(const std::shared_ptr<IFile> &file, const std::shared_ptr<IBlock> &block, const H5Group &group,
                           const std::string &id, const std::string &type, const std::string &name,
                           const DataArray &positions, time_t time)
    : BaseTagHDF5(file, block, group, id, type, name, time)
{
    // TODO: the line below currently throws an exception if positions is
    // not in block - to consider if we prefer copying it to the block
    this->positions(positions.id());
}


std::shared_ptr<IDataArray> MultiTagHDF5::positions() const {
    std::shared_ptr<IDataArray> da;
    bool error = false;

    if (group().hasGroup("positions")) {
        H5Group other_group = group().openGroup("positions", false);
        da = std::make_shared<DataArrayHDF5>(file(), block(), other_group);
        if (!block()->getEntity(da))
            error = true;
    }
    else error = true;

    // NOTE: we check that link exists in both places, here & in entity
    // if error = true it was missing in one of the two
    if (error)
        throw std::runtime_error("MultiTagHDF5::positions: DataArray not found!");

    return da;
}


void MultiTagHDF5::positions(const std::string &name_or_id) {
    std::shared_ptr<IDataArray> ida = block()->getEntity<IDataArray>(name_or_id);
    if (!ida)
        throw std::runtime_error("MultiTagHDF5::positions: DataArray not found in block!");
    if (group().hasGroup("positions"))
        group().removeGroup("positions");

    auto target = std::dynamic_pointer_cast<DataArrayHDF5>(ida);

    group().createLink(target->group(), "positions");
    forceUpdatedAt();
}


bool MultiTagHDF5::hasPositions() const {
    // NOTE: other than in positions getter here we do not check that the
    // positions DataArray also exists in block - we just say it does here
    return group().hasGroup("positions");
}


std::shared_ptr<IDataArray>  MultiTagHDF5::extents() const {
    std::shared_ptr<IDataArray> da;
    bool error = false;

    if (group().hasGroup("extents")) {
        H5Group other_group = group().openGroup("extents", false);
        da = std::make_shared<DataArrayHDF5>(file(), block(), other_group);
        if (!block()->hasEntity(da))
            error = true;
    }

    // NOTE: we check that link exists in parent entity: if error, it was missing there
    if (error)
        throw std::runtime_error("MultiTagHDF5::extents: DataArray not found!");

    return da;
}


void MultiTagHDF5::extents(const std::string &name_or_id) {
    std::shared_ptr<IDataArray> ida = block()->getEntity<IDataArray>(name_or_id);

    if (!ida)
        throw std::runtime_error("MultiTagHDF5::extents: DataArray not found in block!");
    if (group().hasGroup("extents"))
        group().removeGroup("extents");

    if (!checkDimensions(ida, positions()))
        throw std::runtime_error("MultiTagHDF5::extents: cannot set Extent because dimensionality of extent and position data do not match!");
    auto target = std::dynamic_pointer_cast<DataArrayHDF5>(ida);

    group().createLink(target->group(), "extents");
    forceUpdatedAt();
}

void MultiTagHDF5::extents(const none_t t) {
    if (group().hasGroup("extents")) {
        group().removeGroup("extents");
    }
    forceUpdatedAt();
}


std::vector<std::string> MultiTagHDF5::units() const {
    std::vector<std::string> units;
    group().getData("units", units);
    return units;
}


void MultiTagHDF5::units(const std::vector<std::string> &units) {
    group().setData("units", units);
    forceUpdatedAt();
}


void MultiTagHDF5::units(const none_t t) {
    if (group().hasData("units")) {
        group().removeData("units");
    }
    forceUpdatedAt();
}


bool MultiTagHDF5::checkDimensions(const DataArray &a, const DataArray &b)const {
    return a.dataExtent() == b.dataExtent();
}


MultiTagHDF5::~MultiTagHDF5() {}

} // ns nix::hdf5
} // ns nix
