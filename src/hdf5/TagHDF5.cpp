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
    : EntityWithSourcesHDF5(file, block, group)
{
    feature_group = this->group().openOptGroup("features");
    refs_group = this->group().openOptGroup("references");
}


TagHDF5::TagHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const Group &group, const string &id,
                             const string &type, const string &name, const std::vector<double> &position)
    : TagHDF5(file, block, group, id, type, name, position, util::getTime())
{
}


TagHDF5::TagHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const Group &group, const string &id,
                             const string &type, const string &name, const std::vector<double> &position, const time_t time)
    : EntityWithSourcesHDF5(file, block, group, id, type, name, time)
{
    feature_group = this->group().openOptGroup("features");
    refs_group = this->group().openOptGroup("references");
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


// Methods concerning references.


bool TagHDF5::hasReference(const std::string &id) const {
    return refs_group() ? refs_group()->hasGroup(id) : false;
}


size_t TagHDF5::referenceCount() const {
    boost::optional<Group> g = refs_group();
    return g ? g->objectCount() : size_t(0);
}


shared_ptr<IDataArray> TagHDF5::getReference(const std::string &name_or_id) const {
    shared_ptr<IDataArray> da;
    boost::optional<Group> g = refs_group();

    std::string id = name_or_id;

    if (!util::looksLikeUUID(name_or_id)) {
        auto blck = dynamic_pointer_cast<BlockHDF5>(block());

        if (blck->hasDataArray(name_or_id)) {
            id = blck->getDataArray(name_or_id)->id();
        }
    }

    if (g && hasReference(id)) {
        Group group = g->openGroup(id);
        da = make_shared<DataArrayHDF5>(file(), block(), group);
    }

    return da;
}


shared_ptr<IDataArray> TagHDF5::getReference(size_t index) const {
    // check if index valid
    if(index > referenceCount()) {
        throw OutOfBounds("No reference at given index", index);
    }

    boost::optional<Group> g = refs_group();
    string id = g ? g->objectName(index) : "";
    return getReference(id);
}


void TagHDF5::addReference(const std::string &id) {
    if (id.empty())
        throw EmptyString("addReference");
    boost::optional<Group> g = refs_group(true);

    if (!block()->hasDataArray(id))
        throw std::runtime_error("MultiTagHDF5::addReference: DataArray not found in block!");
    
    auto target = dynamic_pointer_cast<DataArrayHDF5>(block()->getDataArray(id));

    g->createLink(target->group(), target->id());
}


bool TagHDF5::removeReference(const std::string &id) {
    boost::optional<Group> g = refs_group();
    bool removed = false;

    if (g) {
        g->removeGroup(id);
        removed = true;
    }

    return removed;
}

// TODO fix this when base entities are fixed
void TagHDF5::references(const std::vector<DataArray> &refs_new) {
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

// Methods concerning features.

bool TagHDF5::hasFeature(const string &id) const {
    // let getFeature try to look up object by id
    return getFeature(id) != nullptr;
}


size_t TagHDF5::featureCount() const {
    boost::optional<Group> g = feature_group();
    return g ? g->objectCount() : size_t(0);
}


shared_ptr<IFeature> TagHDF5::getFeature(const std::string &id) const {
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


shared_ptr<IFeature> TagHDF5::getFeature(size_t index) const {
    boost::optional<Group> g = feature_group();
    string id = g->objectName(index);
    return getFeature(id);
}


shared_ptr<IFeature> TagHDF5::createFeature(const std::string &id, LinkType link_type) {
    if (link_type == LinkType::Indexed) {
        throw std::runtime_error("LinkType 'indexed' is not valid for Tag entities and can only be used for MultiTag entities.");
    }
    if(!block()->hasDataArray(id)) {
        throw std::runtime_error("DataArray not found in Block!");
    }
    string rep_id = util::createId();
    boost::optional<Group> g = feature_group(true);

    Group group = g->openGroup(rep_id, true);
    DataArray data = block()->getDataArray(id);
    return make_shared<FeatureHDF5>(file(), block(), group, rep_id, data, link_type);
}


bool TagHDF5::deleteFeature(const string &id) {
    boost::optional<Group> g = feature_group();
    bool deleted = false;
    
    if (g && hasFeature(id)) {
        g->removeGroup(id);
        deleted = true;
    }
    
    return deleted;
}

// Other methods and functions


TagHDF5::~TagHDF5()
{
}

} // ns nix::hdf5
} // ns nix