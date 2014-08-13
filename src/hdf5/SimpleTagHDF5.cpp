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
#include <nix/hdf5/SimpleTagHDF5.hpp>
#include <nix/hdf5/FeatureHDF5.hpp>
#include <nix/hdf5/DataSet.hpp>
#include <nix/Exception.hpp>

using namespace std;
using namespace nix;
using namespace nix::base;
using namespace nix::hdf5;


SimpleTagHDF5::SimpleTagHDF5(shared_ptr<IFile> file, shared_ptr<IBlock> block, const Group &group)
    : EntityWithSourcesHDF5(file, block, group)
{
    feature_group = group.openGroup("features", false);
    refs_group = group.openGroup("references", false);
}


SimpleTagHDF5::SimpleTagHDF5(shared_ptr<IFile> file, shared_ptr<IBlock> block, const Group &group, const string &id,
                             const string &type, const string &name, const std::vector<double> &position)
    : SimpleTagHDF5(file, block, group, id, type, name, position, util::getTime())
{
}


SimpleTagHDF5::SimpleTagHDF5(shared_ptr<IFile> file, shared_ptr<IBlock> block, const Group &group, const string &id,
                             const string &type, const string &name, const std::vector<double> &position, const time_t time)
    : EntityWithSourcesHDF5(file, block, group, id, type, name, time)
{
    feature_group = group.openGroup("features", true);
    refs_group = group.openGroup("references", true);
    this->position(position);
}


vector<string> SimpleTagHDF5::units() const {
    vector<string> units;
    group().getData("units", units);
    return units;
}


void SimpleTagHDF5::units(const vector<string> &units) {
    group().setData("units", units);
    forceUpdatedAt();
}


void SimpleTagHDF5::units(const none_t t) {
    if (group().hasData("units")) {
        group().removeData("units");
    }
    forceUpdatedAt();
}


vector<double> SimpleTagHDF5::position() const {
    vector<double> position;

    if (group().hasData("position")) {
        group().getData("position", position);
    }

    return position;
}


void SimpleTagHDF5::position(const vector<double> &position) {
    group().setData("position", position);
}


vector<double> SimpleTagHDF5::extent() const {
    vector<double> extent;
    group().getData("extent", extent);
    return extent;
}


void SimpleTagHDF5::extent(const vector<double> &extent) {
    group().setData("extent", extent);
}


void SimpleTagHDF5::extent(const none_t t) {
    if (group().hasData("extent")) {
        group().removeData("extent");
    }
    forceUpdatedAt();
}


// Methods concerning references.


bool SimpleTagHDF5::hasReference(const std::string &id) const {
    return refs_group.hasGroup(id);
}


size_t SimpleTagHDF5::referenceCount() const {
    return refs_group.objectCount();
}


shared_ptr<IDataArray> SimpleTagHDF5::getReference(const std::string &id) const {
    shared_ptr<IDataArray> da;

    if (refs_group.hasGroup(id)) {
        Group group = refs_group.openGroup(id, false);
        da = make_shared<DataArrayHDF5>(file(), block(), group);
    }

    return da;
}


shared_ptr<IDataArray> SimpleTagHDF5::getReference(size_t index) const {
    shared_ptr<IDataArray> da;

    // get reference id
    std::string id = refs_group.objectName(index);
    da = getReference(id);

    return da;
}


void SimpleTagHDF5::addReference(const std::string &id) {
    if (id.empty())
        throw EmptyString("addReference");

    if (!block()->hasDataArray(id))
        throw std::runtime_error("SimpleTagHDF5::addReference: DataArray not found in block!");
    
    auto target = dynamic_pointer_cast<DataArrayHDF5>(block()->getDataArray(id));

    refs_group.createLink(target->group(), id);
}


bool SimpleTagHDF5::removeReference(const std::string &id) {
    refs_group.removeGroup(id);
    return refs_group.hasGroup(id);
}

// TODO fix this when base entities are fixed
void SimpleTagHDF5::references(const std::vector<DataArray> &refs_new) {
    // extract vectors of ids from vectors of new & old references
    std::vector<std::string> ids_new(refs_new.size());
    transform(refs_new.begin(), refs_new.end(), ids_new.begin(), util::toId<DataArray>);
    std::vector<DataArray> refs_old(referenceCount());
    for (size_t i = 0; i < refs_old.size(); i++) refs_old[i] = getReference(i);
    std::vector<std::string> ids_old(refs_old.size());
    transform(refs_old.begin(), refs_old.end(), ids_old.begin(), util::toId<DataArray>);
    // sort them
    std::sort(ids_new.begin(), ids_new.end());
    std::sort(ids_new.begin(), ids_new.end());
    // get ids only in ids_new (add), ids only in ids_old (remove) & ignore rest
    std::vector<std::string> ids_add;
    std::vector<std::string> ids_rem;
    std::set_difference(ids_new.begin(), ids_new.end(), ids_old.begin(), ids_old.end(),
                        std::inserter(ids_add, ids_add.begin()));
    std::set_difference(ids_old.begin(), ids_old.end(), ids_new.begin(), ids_new.end(),
                        std::inserter(ids_rem, ids_rem.begin()));

    // check if all new references exist & add sources
    for (auto id : ids_add) {
        if(!block()->hasDataArray(id)) 
            throw std::runtime_error("One or more data arrays do not exist in this block!");
        addReference(id);
    }
    // remove references
    for (auto id : ids_rem) {
        removeReference(id);
    }
}

// Methods concerning features.

bool SimpleTagHDF5::hasFeature(const string &id) const {
    return feature_group.hasGroup(id);
}


size_t SimpleTagHDF5::featureCount() const {
    return feature_group.objectCount();
}


shared_ptr<IFeature> SimpleTagHDF5::getFeature(const std::string &id) const {
    shared_ptr<IFeature> feature;

    if (hasFeature(id)) {
        Group group = feature_group.openGroup(id, false);
        feature = make_shared<FeatureHDF5>(file(), block(), group);
    }

    return feature;
}


shared_ptr<IFeature> SimpleTagHDF5::getFeature(size_t index) const {
    string id = feature_group.objectName(index);

    return getFeature(id);
}


shared_ptr<IFeature> SimpleTagHDF5::createFeature(const std::string &id, LinkType link_type) {
    if (link_type == LinkType::Indexed) {
        throw std::runtime_error("LinkType 'indexed' is not valid for SimpleTag entities and can only be used for DataTag entities.");
    }

    string rep_id = util::createId("feature");
    while (feature_group.hasObject(rep_id))
        rep_id = util::createId("feature");

    Group group = feature_group.openGroup(rep_id, true);
    DataArray data = block()->getDataArray(id);
    return make_shared<FeatureHDF5>(file(), block(), group, rep_id, data, link_type);
}


bool SimpleTagHDF5::deleteFeature(const string &id) {
    if (feature_group.hasGroup(id)) {
        feature_group.removeGroup(id);
        return true;
    } else {
        return false;
    }
}

// Other methods and functions


SimpleTagHDF5::~SimpleTagHDF5()
{
}

