// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/NDArray.hpp>
#include <nix/util/util.hpp>
#include <nix/hdf5/DataTagHDF5.hpp>
#include <nix/hdf5/FeatureHDF5.hpp>
#include <nix/Exception.hpp>

using namespace std;

namespace nix {
namespace hdf5 {


DataTagHDF5::DataTagHDF5(const DataTagHDF5 &tag)
    : EntityWithSourcesHDF5(tag.file(), tag.block(), tag.group(), tag.id(), tag.type(), tag.name()),
      reference_list(tag.reference_list)
{
    feature_group = tag.feature_group;
    positions(tag.positions().id());
}


DataTagHDF5::DataTagHDF5(const File &file, const Block &block, const Group &group, 
                         const string &id, const std::string &type, const string &name, const DataArray _positions)
    : DataTagHDF5(file, block, group, id, type, name, _positions, util::getTime())
{
}


DataTagHDF5::DataTagHDF5(const File &file, const Block &block, const Group &group,
                         const std::string &id, const std::string &type, const string &name, const DataArray _positions, time_t time)
    : EntityWithSourcesHDF5(file, block, group, id, type, name, time), reference_list(group, "references")
{
    feature_group = this->group().openGroup("features");
    // TODO: the line below currently throws an exception if positions is
    // not in block - to consider if we prefer copying it to the block
    positions(_positions.id());
}


DataArray DataTagHDF5::positions() const {
    string id;

    if(group().hasAttr("positions")) {
        group().getAttr("positions", id);
    } else {
        throw MissingAttr("positions");
    }

    if(block().hasDataArray(id)) {
        return block().getDataArray(id);
    } else {
        throw runtime_error("DataArray with positions not found in Block!");
    }
}


void DataTagHDF5::positions(const string &id) {
    if(id.empty()) {
        throw EmptyString("positions DataArray id");
    }
    else {
        if(!block().hasDataArray(id)) {
            throw runtime_error("DataTagHDF5::extents: cannot set Extent because referenced DataArray does not exist!");
        } else {
            if(extents()) {
                DataArray pos = block().getDataArray(id);
                DataArray ext = extents();
                if(!checkDimensions(ext, pos))
                    throw runtime_error("DataTagHDF5::positions: cannot set Positions because dimensionality of extent and position data do not match!");
            }
            group().setAttr("positions", id);
            forceUpdatedAt();
        }
    }
}


bool DataTagHDF5::hasPositions() const {
    std::string posId;
    group().getAttr("positions", posId);
    return (posId.length() > 0);
}


DataArray DataTagHDF5::extents() const {
    std::string extId;
    group().getAttr("extents", extId);

    // block will return empty object if entity not found
    return block().getDataArray(extId);
}


void DataTagHDF5::extents(const string &extentsId) {
    if(extentsId.empty()) {
        throw EmptyString("extentsId");
    }
    else {
        if(!block().hasDataArray(extentsId)) {
            throw runtime_error("DataTagHDF5::extents: cannot set Extent because referenced DataArray does not exist!");
        }
        else {
            if(hasPositions()) {
                DataArray ext = block().getDataArray(extentsId);
                DataArray pos = positions();
                if(!checkDimensions(ext, pos))
                    throw runtime_error("DataTagHDF5::extents: cannot set Extent because dimensionality of extent and position data do not match!");
            }
            group().setAttr("extents", extentsId);
            forceUpdatedAt();
        }
    }
}

void DataTagHDF5::extents(const none_t t) {
    if(group().hasAttr("extents")) {
        group().removeAttr("extents");
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


DataArray DataTagHDF5::getReference(const std::string &id) const {
    if (hasReference(id)) {
        // block will return empty object if entity not found
        return block().getDataArray(id);
    } else {
        return DataArray();
    }
}

DataArray DataTagHDF5::getReference(size_t index) const {
    std::vector<std::string> refs = reference_list.get();
    std::string id;

    // get reference id
    if(index < refs.size()) {
        id = refs[index];
    } else {
        throw OutOfBounds("No data array at given index", index);
    }
    // get referenced array
    if(hasReference(id) && block().hasDataArray(id)) {
        return block().getDataArray(id);
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

    for (size_t i = 0; i < references.size(); i++) {
        ids[i] = references[i].id();
    }

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


Feature DataTagHDF5::getFeature(const std::string &id) const {
    if (feature_group.hasGroup(id)) { 
        Group group = feature_group.openGroup(id, false);
        string link_type;
        group.getAttr("link_type", link_type);
        LinkType linkType = linkTypeFromString(link_type);
        string dataId;
        group.getAttr("data", dataId);
        DataArray data = block().getDataArray(dataId);
        auto tmp = make_shared<FeatureHDF5>(file(), block(), group, id, data, linkType);
        return Feature(tmp);
    } else {
        return Feature();
    }
}


Feature DataTagHDF5::getFeature(size_t index) const {
    string id = feature_group.objectName(index);
    return getFeature(id);
}


Feature DataTagHDF5::createFeature(const std::string &data_array_id, LinkType link_type) {
    string id = util::createId("feature");
    while(feature_group.hasObject(id))
        id = util::createId("feature");

    Group group = feature_group.openGroup(id, true);
    DataArray data = block().getDataArray(data_array_id);
    auto tmp = make_shared<FeatureHDF5>(file(), block(), group, id, data, link_type);

    return Feature(tmp);
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


ostream& operator<<(ostream &out, const DataTagHDF5 &ent) {
    out << "DataTag: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}


bool DataTagHDF5::checkDimensions(const DataArray &a, const DataArray &b)const {
    return a.getDataExtent() == b.getDataExtent();
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

} // namespace hdf5
} // namespace nix
