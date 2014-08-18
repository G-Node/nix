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
using namespace nix;
using namespace nix::base;
using namespace nix::hdf5;


MultiTagHDF5::MultiTagHDF5(shared_ptr<IFile> file, shared_ptr<IBlock> block, const Group &group)
    : EntityWithSourcesHDF5(file, block, group)
{
    feature_group = group.openGroup("features", false);
    refs_group = group.openGroup("references", false);
}


MultiTagHDF5::MultiTagHDF5(shared_ptr<IFile> file, shared_ptr<IBlock> block, const Group &group,
                         const string &id, const std::string &type, const string &name, const DataArray &positions)
    : MultiTagHDF5(file, block, group, id, type, name, positions, util::getTime())
{
}


MultiTagHDF5::MultiTagHDF5(shared_ptr<IFile> file, shared_ptr<IBlock> block, const Group &group,
                         const std::string &id, const std::string &type, const string &name, const DataArray &positions, time_t time)
    : EntityWithSourcesHDF5(file, block, group, id, type, name, time)
{
    feature_group = group.openGroup("features", true);
    refs_group = group.openGroup("references", true);
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


void MultiTagHDF5::positions(const string &id) {
    if (id.empty())
        throw EmptyString("positions");
    if (!block()->hasDataArray(id))
        throw std::runtime_error("MultiTagHDF5::positions: DataArray not found in block!");
    if (group().hasGroup("positions"))
        group().removeGroup("positions");
    
    auto target = dynamic_pointer_cast<DataArrayHDF5>(block()->getDataArray(id));

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


void MultiTagHDF5::extents(const string &id) {
    if (id.empty())
        throw EmptyString("extents");
    if (!block()->hasDataArray(id))
        throw std::runtime_error("MultiTagHDF5::extents: DataArray not found in block!");
    if (group().hasGroup("extents"))
        group().removeGroup("extents");

    auto da = block()->getDataArray(id);
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

//--------------------------------------------------
// Methods concerning references.
//--------------------------------------------------

bool MultiTagHDF5::hasReference(const std::string &name) const {
    return refs_group.hasGroup(name);
}


size_t MultiTagHDF5::referenceCount() const {
    return refs_group.objectCount();
}


shared_ptr<IDataArray>  MultiTagHDF5::getReference(const std::string &id) const {
    shared_ptr<IDataArray> da;

    if (refs_group.hasGroup(id)) {
        Group group = refs_group.openGroup(id, false);
        da = make_shared<DataArrayHDF5>(file(), block(), group);
    }

    return da;
}

shared_ptr<IDataArray>  MultiTagHDF5::getReference(size_t index) const {
    shared_ptr<IDataArray> da;

    // get reference name
    std::string name = refs_group.objectName(index);
    da = getReference(name);

    return da;
}

void MultiTagHDF5::addReference(const std::string &id) {
    if (id.empty())
        throw EmptyString("addReference");

    if (!block()->hasDataArray(id))
        throw std::runtime_error("MultiTagHDF5::addReference: DataArray not found in block!");
    
    auto target = dynamic_pointer_cast<DataArrayHDF5>(block()->getDataArray(id));

    refs_group.createLink(target->group(), target->id());
}


bool MultiTagHDF5::removeReference(const std::string &id) {
    refs_group.removeGroup(id);
    return refs_group.hasGroup(id);
}


void MultiTagHDF5::references(const std::vector<DataArray> &refs_new) {
    // extract vectors of names from vectors of new & old references
    std::vector<std::string> names_new(refs_new.size());
    transform(refs_new.begin(), refs_new.end(), names_new.begin(), util::toName<DataArray>);
    std::vector<DataArray> refs_old(referenceCount());
    for (size_t i = 0; i < refs_old.size(); i++) refs_old[i] = getReference(i);
    std::vector<std::string> names_old(refs_old.size());
    transform(refs_old.begin(), refs_old.end(), names_old.begin(), util::toName<DataArray>);
    // sort them
    std::sort(names_new.begin(), names_new.end());
    std::sort(names_new.begin(), names_new.end());
    // get names only in names_new (add), names only in names_old (remove) & ignore rest
    std::vector<std::string> names_add;
    std::vector<std::string> names_rem;
    std::set_difference(names_new.begin(), names_new.end(), names_old.begin(), names_old.end(),
                        std::inserter(names_add, names_add.begin()));
    std::set_difference(names_old.begin(), names_old.end(), names_new.begin(), names_new.end(),
                        std::inserter(names_rem, names_rem.begin()));

    // check if all new references exist & add sources
    auto blck = dynamic_pointer_cast<BlockHDF5>(block());
    for (auto name : names_add) {
        if (!blck->hasDataArrayByName(name)) 
            throw std::runtime_error("One or more data arrays do not exist in this block!");
        addReference(blck->getDataArrayByName(name)->id());
    }
    // remove references
    for (auto name : names_rem) {
        if (!blck->hasDataArrayByName(name))
            removeReference(blck->getDataArrayByName(name)->id());
    }
}

//--------------------------------------------------
// Methods concerning features.
//--------------------------------------------------

bool MultiTagHDF5::hasFeature(const string &id) const {
    return feature_group.hasGroup(id);
}


size_t MultiTagHDF5::featureCount() const {
    return feature_group.objectCount();
}


shared_ptr<IFeature>  MultiTagHDF5::getFeature(const std::string &id) const {
    shared_ptr<FeatureHDF5> feature;

    if (feature_group.hasGroup(id)) {
        Group group = feature_group.openGroup(id, false);
        feature = make_shared<FeatureHDF5>(file(), block(), group);
    }

    return feature;
}


shared_ptr<IFeature>  MultiTagHDF5::getFeature(size_t index) const {
    string id = feature_group.objectName(index);
    return getFeature(id);
}


shared_ptr<IFeature>  MultiTagHDF5::createFeature(const std::string &name, LinkType link_type) {
    string rep_id = util::createId("feature");
    while (feature_group.hasObject(rep_id))
        rep_id = util::createId("feature");

    Group group = feature_group.openGroup(rep_id, true);
    DataArray data = block()->getDataArray(name);
    return make_shared<FeatureHDF5>(file(), block(), group, rep_id, data, link_type);
}


bool MultiTagHDF5::deleteFeature(const string &id) {
    if (feature_group.hasGroup(id)) {
        feature_group.removeGroup(id);
        return true;
    } else {
        return false;
    }
}

//--------------------------------------------------
// Other methods and functions
//--------------------------------------------------


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

