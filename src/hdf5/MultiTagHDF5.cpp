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
    : EntityWithSourcesHDF5(file, block, group)
{
    feature_group = this->group().openOptGroup("features");
    refs_group = this->group().openOptGroup("references");
}


MultiTagHDF5::MultiTagHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const Group &group,
                         const string &id, const std::string &type, const string &name, const DataArray &positions)
    : MultiTagHDF5(file, block, group, id, type, name, positions, util::getTime())
{
}


MultiTagHDF5::MultiTagHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const Group &group,
                         const std::string &id, const std::string &type, const string &name, const DataArray &positions, time_t time)
    : EntityWithSourcesHDF5(file, block, group, id, type, name, time)
{
    feature_group = this->group().openOptGroup("features");
    refs_group = this->group().openOptGroup("references");
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

bool MultiTagHDF5::hasReference(const std::string &id) const {
    return refs_group() ? refs_group()->hasGroup(id) : false;
}


size_t MultiTagHDF5::referenceCount() const {
    boost::optional<Group> g = refs_group();
    return g ? g->objectCount() : size_t(0);
}


shared_ptr<IDataArray>  MultiTagHDF5::getReference(const std::string &name_or_id) const {
    shared_ptr<IDataArray> da;
    boost::optional<Group> g = refs_group();

    std::string id = name_or_id;

    if (!util::looksLikeUUID(name_or_id) && block()->hasDataArray(name_or_id)) {
        id = block()->getDataArray(name_or_id)->id();
    }

    if (g && hasReference(id)) {
        Group group = g->openGroup(id);
        da = make_shared<DataArrayHDF5>(file(), block(), group);
    }

    return da;
}

shared_ptr<IDataArray>  MultiTagHDF5::getReference(size_t index) const {
    // check if index valid
    if(index > referenceCount()) {
        throw OutOfBounds("No reference at given index", index);
    }

    boost::optional<Group> g = refs_group();
    string id = g ? g->objectName(index) : "";
    return getReference(id);
}

void MultiTagHDF5::addReference(const std::string &id) {
    if (id.empty())
        throw EmptyString("addReference");
    boost::optional<Group> g = refs_group(true);

    if (!block()->hasDataArray(id))
        throw std::runtime_error("MultiTagHDF5::addReference: DataArray not found in block!");
    
    auto target = dynamic_pointer_cast<DataArrayHDF5>(block()->getDataArray(id));

    g->createLink(target->group(), target->id());
}


bool MultiTagHDF5::removeReference(const std::string &id) {
    boost::optional<Group> g = refs_group();
    bool removed = false;

    if (g) {
        g->removeGroup(id);
        removed = true;
    }

    return removed;
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
        if (!blck->hasDataArray(name))
            throw std::runtime_error("One or more data arrays do not exist in this block!");
        addReference(blck->getDataArray(name)->id());
    }
    // remove references
    for (auto name : names_rem) {
        if (!blck->hasDataArray(name))
            removeReference(blck->getDataArray(name)->id());
    }
}

//--------------------------------------------------
// Methods concerning features.
//--------------------------------------------------

bool MultiTagHDF5::hasFeature(const string &id) const {
    // let getFeature try to look up object by id
    return getFeature(id) != nullptr;
}


size_t MultiTagHDF5::featureCount() const {
    boost::optional<Group> g = feature_group();
    return g ? g->objectCount() : size_t(0);
}


shared_ptr<IFeature> MultiTagHDF5::getFeature(const std::string &id) const {
    shared_ptr<FeatureHDF5> feature;
    boost::optional<Group> g = feature_group();

    if (g) {
        if(g->hasGroup(id)) {
            Group group = g->openGroup(id, false);
            feature = make_shared<FeatureHDF5>(file(), block(), group);
        }
    }

    return feature;
}


shared_ptr<IFeature>  MultiTagHDF5::getFeature(size_t index) const {
    boost::optional<Group> g = feature_group();
    string id = g->objectName(index);
    return getFeature(id);
}


shared_ptr<IFeature>  MultiTagHDF5::createFeature(const std::string &id, LinkType link_type) {
    if(!block()->hasDataArray(id)) {
        throw std::runtime_error("DataArray not found in Block!");
    }
    string rep_id = util::createId();
    boost::optional<Group> g = feature_group(true);

    Group group = g->openGroup(rep_id, true);
    DataArray data = block()->getDataArray(id);
    return make_shared<FeatureHDF5>(file(), block(), group, rep_id, data, link_type);
}


bool MultiTagHDF5::deleteFeature(const string &id) {
    boost::optional<Group> g = feature_group();
    bool deleted = false;
    
    if (g && hasFeature(id)) {
        g->removeGroup(id);
        deleted = true;
    }
    
    return deleted;
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

} // ns nix::hdf5
} // ns nix