// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <algorithm>

#include <nix/NDArray.hpp>
#include <nix/util/util.hpp>
#include <nix/DataArray.hpp>
#include <nix/hdf5/DataArrayHDF5.hpp>
#include <nix/hdf5/BlockHDF5.hpp>
#include <nix/hdf5/MultiTagHDF5.hpp>
#include <nix/hdf5/FeatureHDF5.hpp>
#include <nix/Exception.hpp>

using namespace std;
using namespace nix::base;

namespace nix {
namespace hdf5 {


MultiTagHDF5::MultiTagHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const Group &group)
    : BaseTagHDF5(file, block, group)
{
}


MultiTagHDF5::MultiTagHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const Group &group,
                         const string &id, const std::string &type, const string &name, const DataArray &positions)
    : MultiTagHDF5(file, block, group, id, type, name, positions, util::getTime())
{
}


MultiTagHDF5::MultiTagHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const Group &group,
                         const std::string &id, const std::string &type, const string &name, const DataArray &positions, time_t time)
    : BaseTagHDF5(file, block, group, id, type, name, time)
{
    // TODO: the line below currently throws an exception if positions is
    // not in block - to consider if we prefer copying it to the block
    this->positions(positions.id());
}


shared_ptr<IDataArray> MultiTagHDF5::positions() const {
    shared_ptr<IDataArray> da;
    bool error = false;

    if (group().hasGroup("positions")) {
        Group other_group = group().openGroup("positions", false);
        da = make_shared<DataArrayHDF5>(file(), block(), other_group);
        if (!block()->hasDataArray(da->id())) 
            error = true;
    }
    else error = true;
    
    // NOTE: we check that link exists in both places, here & in entity
    // if error = true it was missing in one of the two
    if (error) 
        throw std::runtime_error("MultiTagHDF5::positions: DataArray not found!");

    return da;
}


void MultiTagHDF5::positions(const string &name_or_id) {
    if (name_or_id.empty())
        throw EmptyString("positions");
    if (!block()->hasDataArray(name_or_id))
        throw std::runtime_error("MultiTagHDF5::positions: DataArray not found in block!");
    if (group().hasGroup("positions"))
        group().removeGroup("positions");
    
    auto target = dynamic_pointer_cast<DataArrayHDF5>(block()->getDataArray(name_or_id));

    group().createLink(target->group(), "positions");
    forceUpdatedAt();
}


bool MultiTagHDF5::hasPositions() const {
    // NOTE: other than in positions getter here we do not check that the 
    // positions DataArray also exists in block - we just say it does here
    return group().hasGroup("positions");
}


shared_ptr<IDataArray>  MultiTagHDF5::extents() const {
    shared_ptr<IDataArray> da;
    bool error = false;

    if (group().hasGroup("extents")) {
        Group other_group = group().openGroup("extents", false);
        da = make_shared<DataArrayHDF5>(file(), block(), other_group);
        if (!block()->hasDataArray(da->id())) 
            error = true;
    }
    
    // NOTE: we check that link exists in parent entity: if error, it was missing there
    if (error) 
        throw std::runtime_error("MultiTagHDF5::extents: DataArray not found!");

    return da;
}


void MultiTagHDF5::extents(const string &name_or_id) {
    if (name_or_id.empty())
        throw EmptyString("extents");
    if (!block()->hasDataArray(name_or_id))
        throw std::runtime_error("MultiTagHDF5::extents: DataArray not found in block!");
    if (group().hasGroup("extents"))
        group().removeGroup("extents");

    auto da = block()->getDataArray(name_or_id);
    if (!checkDimensions(da, positions()))
        throw runtime_error("MultiTagHDF5::extents: cannot set Extent because dimensionality of extent and position data do not match!");
    auto target = dynamic_pointer_cast<DataArrayHDF5>(da);

    group().createLink(target->group(), "extents");
    forceUpdatedAt();
}

void MultiTagHDF5::extents(const none_t t) {
    if (group().hasGroup("extents")) {
        group().removeGroup("extents");
    }
    forceUpdatedAt();
}


vector<string> MultiTagHDF5::units() const {
    vector<string> units;
    group().getData("units", units);
    return units;
}


void MultiTagHDF5::units(const vector<string> &units) {
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


bool MultiTagHDF5::checkPositionsAndExtents() const {
    bool valid = true;

    if (hasPositions() && extents()) {
        DataArray pos = positions();
        DataArray ext = extents();
        boost::multi_array<double,1> posData, extData;
        pos.getData(posData);
        ext.getData(extData);
        return checkDimensions(pos, ext);
    }

    return valid;
}


MultiTagHDF5::~MultiTagHDF5() {}

} // ns nix::hdf5
} // ns nix