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
#include <nix/hdf5/DataTagHDF5.hpp>
#include <nix/hdf5/FeatureHDF5.hpp>
#include <nix/Exception.hpp>

using namespace std;
using namespace nix;
using namespace nix::base;
using namespace nix::hdf5;

// TODO unnecessary IO (see #316)
DataTagHDF5::DataTagHDF5(const DataTagHDF5 &tag)
    : EntityWithSourcesHDF5(tag.file(), tag.block(), tag.group(), tag.id(), tag.type(), tag.name()),
      reference_list(tag.reference_list)
{
    feature_group = tag.feature_group;
    positions(tag.positions()->id());
}


DataTagHDF5::DataTagHDF5(shared_ptr<IFile> file, shared_ptr<IBlock> block, const Group &group,
                         const string &id, const std::string &type, const string &name, const DataArray &positions)
    : DataTagHDF5(file, block, group, id, type, name, positions, util::getTime())
{
}


DataTagHDF5::DataTagHDF5(shared_ptr<IFile> file, shared_ptr<IBlock> block, const Group &group,
                         const std::string &id, const std::string &type, const string &name, const DataArray &positions, time_t time)
    : EntityWithSourcesHDF5(file, block, group, id, type, name, time), reference_list(group, "references")
{
    feature_group = this->group().openGroup("features");
    // TODO: the line below currently throws an exception if positions is
    // not in block - to consider if we prefer copying it to the block
    this->positions(positions.id());
}


shared_ptr<IDataArray> DataTagHDF5::positions() const {
    string id;

    if(group().hasAttr("positions")) {
        group().getAttr("positions", id);
    } else {
        throw MissingAttr("positions");
    }

    if(block()->hasDataArray(id)) {
        return block()->getDataArray(id);
    } else {
        throw runtime_error("DataArray with positions not found in Block!");
    }
}


void DataTagHDF5::positions(const string &id) {
    if(id.empty())
        throw EmptyString("positions DataArray id");

    if(!block()->hasDataArray(id))
        throw runtime_error("DataTagHDF5::extents: cannot set Extent because referenced DataArray does not exist!");

    if(extents()) {
        auto pos = block()->getDataArray(id);

        if(!checkDimensions(extents(), pos))
            throw runtime_error("DataTagHDF5::positions: cannot set Positions because dimensionality of extent and position data do not match!");
    }

    group().setAttr("positions", id);
    forceUpdatedAt();
}


bool DataTagHDF5::hasPositions() const {
    std::string posId;
    group().getAttr("positions", posId);
    return (posId.length() > 0);
}


shared_ptr<IDataArray>  DataTagHDF5::extents() const {
    std::string extId;
    group().getAttr("extents", extId);

    return block()->getDataArray(extId);
}


void DataTagHDF5::extents(const string &extentsId) {
    if(extentsId.empty())
        throw EmptyString("extentsId");

    if(!block()->hasDataArray(extentsId))
        throw runtime_error("DataTagHDF5::extents: cannot set Extent because referenced DataArray does not exist!");

    if(hasPositions()) {
        auto ext = block()->getDataArray(extentsId);

        if(!checkDimensions(ext, positions()))
            throw runtime_error("DataTagHDF5::extents: cannot set Extent because dimensionality of extent and position data do not match!");
    }

    group().setAttr("extents", extentsId);
    forceUpdatedAt();
}

void DataTagHDF5::extents(const none_t t) {
    if(group().hasAttr("extents")) {
        group().removeAttr("extents");
    }
    forceUpdatedAt();
}


vector<string> DataTagHDF5::units() const {
    vector<string> units;
    group().getData("units", units);
    return units;
}


void DataTagHDF5::units(const vector<string> &units) {
    group().setData("units", units);
    forceUpdatedAt();
}


void DataTagHDF5::units(const none_t t) {
    if(group().hasData("units")) {
        group().removeData("units");
    }
    forceUpdatedAt();
}

//--------------------------------------------------
// Methods concerning references.
//--------------------------------------------------

bool DataTagHDF5::hasReference(const std::string &id) const {
    return reference_list.has(id);
}


size_t DataTagHDF5::referenceCount() const {
    return reference_list.count();
}


shared_ptr<IDataArray>  DataTagHDF5::getReference(const std::string &id) const {
    shared_ptr<IDataArray> da;

    if (hasReference(id)) {
        da = block()->getDataArray(id);
    }

    return da;
}

shared_ptr<IDataArray>  DataTagHDF5::getReference(size_t index) const {
    std::vector<std::string> refs = reference_list.get();
    std::string id;

    if(index < refs.size()) {
        id = refs[index];
    } else {
        throw OutOfBounds("No data array at given index", index);
    }

    if(hasReference(id) && block()->hasDataArray(id)) {
        return block()->getDataArray(id);
    } else {
        throw runtime_error("No data array id: " + id);
    }
}

void DataTagHDF5::addReference(const std::string &id) {
    reference_list.add(id);
}


bool DataTagHDF5::removeReference(const std::string &id) {
    return reference_list.remove(id);
}


void DataTagHDF5::references(const std::vector<DataArray> &references) {
    vector<string> ids(references.size());
    transform(references.begin(), references.end(), ids.begin(), [](const DataArray &da) -> string { return da.id(); });

    reference_list.set(ids);
}

//--------------------------------------------------
// Methods concerning features.
//--------------------------------------------------

bool DataTagHDF5::hasFeature(const string &id) const {
    return feature_group.hasGroup(id);
}


size_t DataTagHDF5::featureCount() const {
    return feature_group.objectCount();
}


shared_ptr<IFeature>  DataTagHDF5::getFeature(const std::string &id) const {
    shared_ptr<FeatureHDF5> feature;

    if (feature_group.hasGroup(id)) {
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


shared_ptr<IFeature>  DataTagHDF5::getFeature(size_t index) const {
    string id = feature_group.objectName(index);
    return getFeature(id);
}


shared_ptr<IFeature>  DataTagHDF5::createFeature(const std::string &data_array_id, LinkType link_type) {
    string id = util::createId("feature");
    while(feature_group.hasObject(id))
        id = util::createId("feature");

    Group group = feature_group.openGroup(id, true);
    DataArray data = block()->getDataArray(data_array_id);
    return make_shared<FeatureHDF5>(file(), block(), group, id, data, link_type);
}


bool DataTagHDF5::deleteFeature(const string &id) {
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


void DataTagHDF5::swap(DataTagHDF5 &other) {
    using std::swap;

    EntityWithSourcesHDF5::swap(other);
    swap(feature_group, other.feature_group);
    swap(reference_list, other.reference_list);
}


DataTagHDF5& DataTagHDF5::operator=(const DataTagHDF5 &other) {
    if (*this != other) {
        DataTagHDF5 tmp(other);
        swap(tmp);
    }
    return *this;
}


bool DataTagHDF5::checkDimensions(const DataArray &a, const DataArray &b)const {
    return a.dataExtent() == b.dataExtent();
}


bool DataTagHDF5::checkPositionsAndExtents() const {
    bool valid = true;

    if(hasPositions() && extents()) {
        DataArray pos = positions();
        DataArray ext = extents();
        boost::multi_array<double,1> posData, extData;
        pos.getData(posData);
        ext.getData(extData);
        return checkDimensions(pos, ext);
    }

    return valid;
}


DataTagHDF5::~DataTagHDF5() {}

