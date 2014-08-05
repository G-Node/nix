// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#include <nix/util/util.hpp>
#include <nix/hdf5/DataArrayHDF5.hpp>
#include <nix/hdf5/SimpleTagHDF5.hpp>
#include <nix/hdf5/FeatureHDF5.hpp>
#include <nix/hdf5/DataSet.hpp>
#include <nix/Exception.hpp>

using namespace std;
using namespace nix;
using namespace nix::base;
using namespace nix::hdf5;


SimpleTagHDF5::SimpleTagHDF5(const SimpleTagHDF5 &tag)
    : EntityWithSourcesHDF5(tag.file(), tag.block(), tag.group(), tag.id(), tag.type(), tag.name()),
      references_list(tag.group(), "references")
{
    feature_group = tag.feature_group;
}


SimpleTagHDF5::SimpleTagHDF5(shared_ptr<IFile> file, shared_ptr<IBlock> block, const Group &group, const string &id,
                             const string &type, const string &name, const std::vector<DataArray> &refs)
    : SimpleTagHDF5(file, block, group, id, type, name, refs, util::getTime())
{
}


SimpleTagHDF5::SimpleTagHDF5(shared_ptr<IFile> file, shared_ptr<IBlock> block, const Group &group, const string &id,
                             const string &type, const string &name, const std::vector<DataArray> &refs, const time_t time)
    : EntityWithSourcesHDF5(file, block, group, id, type, name, time), references_list(group, "references")
{
    feature_group = group.openGroup("features");

    bool valid = false;
    for(auto it = refs.begin(); it != refs.end(); ++it) {
        // NOTE: arrays might be empty - we drop them & but require one valid!
        //       arrays not found in block => addReference: o_O => ERROR
        if(*it) {
            addReference((*it).id());
            valid = true;
        }
    }
    if(!valid) {
        throw std::runtime_error("SimpleTag requires at least one valid referenced DataArray to be constructed!");
    }
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
    if(group().hasData("units")) {
        group().removeData("units");
    }
    forceUpdatedAt();
}


vector<double> SimpleTagHDF5::position() const {
    vector<double> position;

    if(group().hasData("position")) {
        group().getData("position", position);
    }

    return position;
}


void SimpleTagHDF5::position(const vector<double> &position) {
    group().setData("position", position);
}


void SimpleTagHDF5::position(const none_t t) {
    if(group().hasData("position")) {
        group().removeData("position");
    }
    forceUpdatedAt();
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
    if(group().hasData("extent")) {
        group().removeData("extent");
    }
    forceUpdatedAt();
}


// Methods concerning references.


bool SimpleTagHDF5::hasReference(const std::string &id) const {
    return references_list.has(id);
}


size_t SimpleTagHDF5::referenceCount() const {
    return references_list.count();
}


shared_ptr<IDataArray> SimpleTagHDF5::getReference(const std::string &id) const {
    shared_ptr<IDataArray> da;

    if (hasReference(id)) {
        da = block()->getDataArray(id);
    }

    return da;
}


shared_ptr<IDataArray> SimpleTagHDF5::getReference(size_t index) const {
    shared_ptr<IDataArray> da;

    std::vector<std::string> refs = references_list.get();

    if(index < refs.size()) {
        string id = refs[index];
        da = getReference(id);
    } else {
        throw OutOfBounds("No data array at given index", index);
    }

    return da;
}


void SimpleTagHDF5::addReference(const std::string &id) {
    if (!block()->hasDataArray(id)) {
        throw runtime_error("Unable to find data array with reference_id " +
                            id + " on block " + block()->id());
    }

    references_list.add(id);
}


bool SimpleTagHDF5::removeReference(const std::string &id) {
    return references_list.remove(id);
}

// TODO fix this when base entities are fixed
void SimpleTagHDF5::references(const std::vector<DataArray> &references) {
    vector<string> ids(references.size());

    for (size_t i = 0; i < references.size(); i++) {
        ids[i] = references[i].id();
    }

    references_list.set(ids);
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

    if(hasFeature(id)) {
        Group group = feature_group.openGroup(id, false);
        // TODO unnecessary IO (see #316)
        string link_type;
        group.getAttr("link_type", link_type);
        LinkType linkType = linkTypeFromString(link_type);
        string dataId;
        group.getAttr("data", dataId);
        DataArray data = block()->getDataArray(dataId);
        feature = make_shared<FeatureHDF5>(file(), block(), group, id, data, linkType);
    }

    return feature;
}


shared_ptr<IFeature> SimpleTagHDF5::getFeature(size_t index) const {
    string id = feature_group.objectName(index);

    return getFeature(id);
}


shared_ptr<IFeature> SimpleTagHDF5::createFeature(const std::string &data_array_id, LinkType link_type) {
    if(link_type == LinkType::Indexed) {
        throw std::runtime_error("LinkType 'indexed' is not valid for SimpleTag entities and can only be used for DataTag entities.");
    }

    string rep_id = util::createId("feature");
    while(feature_group.hasObject(rep_id))
        rep_id = util::createId("feature");

    Group group = feature_group.openGroup(rep_id, true);
    DataArray data = block()->getDataArray(data_array_id);
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


void SimpleTagHDF5::swap(SimpleTagHDF5 &other) {
    using std::swap;

    EntityWithSourcesHDF5::swap(other);
    swap(feature_group, other.feature_group);
    swap(references_list, other.references_list);
}


SimpleTagHDF5& SimpleTagHDF5::operator=(const SimpleTagHDF5 &other) {
    if (*this != other) {
        SimpleTagHDF5 tmp(other);
        swap(tmp);
    }
    return *this;
}


SimpleTagHDF5::~SimpleTagHDF5()
{
}

