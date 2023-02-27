// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "FeatureHDF5.hpp"

#include <nix/util/util.hpp>
#include <nix/DataArray.hpp>
#include "DataArrayHDF5.hpp"
#include "DataFrameHDF5.hpp"


using namespace std;
using namespace nix::base;

namespace nix {
namespace hdf5 {


string linkTypeToString(LinkType link_type) {
    static vector<string> type_names = {"tagged", "untagged", "indexed"};
    return type_names[static_cast<int>(link_type)];
}


LinkType linkTypeFromString(const string &str) {
    if (str == "tagged")
        return LinkType::Tagged;
    else if (str == "untagged")
        return LinkType::Untagged;
    else if (str == "indexed")
        return LinkType::Indexed;
    else
        throw runtime_error("Unable to create a LinkType from the string: " + str);
}


string targetTypeToString(TargetType target_type) {
    static vector<string> type_names = {"DataArray", "DataFrame"};
    return type_names[static_cast<int>(target_type)];
}


TargetType targetTypeFromString(const string &str) {
    if (str == "DataArray")
        return TargetType::DataArray;
    else if (str == "DataFrame")
        return TargetType::DataFrame;
    else
        throw runtime_error("Unable to create a TargetType from the string: " + str);
}


FeatureHDF5::FeatureHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const H5Group &group)
    : EntityHDF5(file, group), block(block)
{
}

FeatureHDF5::FeatureHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const H5Group &group,
                         const string &id, DataArray data, LinkType link_type)
    : FeatureHDF5(file, block, group, id, data, link_type, util::getTime())
{
}


FeatureHDF5::FeatureHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const H5Group &group,
                         const string &id, DataArray data, LinkType link_type, time_t time)
    : EntityHDF5(file, group, id, time), block(block)
{
    linkType(link_type);
    // TODO: the line below currently throws an exception if the DataArray
    // is not in block - to consider if we prefer copying it to the block
    this->data(data);
}


FeatureHDF5::FeatureHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const H5Group &group,
                         const string &id, DataFrame data, LinkType link_type)
    : FeatureHDF5(file, block, group, id, data, link_type, util::getTime())
{
}


FeatureHDF5::FeatureHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const H5Group &group,
                         const string &id, DataFrame data, LinkType link_type, time_t time)
    : EntityHDF5(file, group, id, time), block(block)
{
    linkType(link_type);
    // TODO: the line below currently throws an exception if the DataArray
    // is not in block - to consider if we prefer copying it to the block
    this->data(data);
}


void FeatureHDF5::linkType(LinkType link_type) {
    // linkTypeToString will generate an error if link_type is invalid
    group().setAttr("link_type", linkTypeToString(link_type));
    forceUpdatedAt();
}


void FeatureHDF5::targetType(TargetType ttype) {
    group().setAttr("target_type", targetTypeToString(ttype));
    forceUpdatedAt();
}


void FeatureHDF5::data(const DataArray &data) {
    std::shared_ptr<IDataArray> ida = block->getEntity<IDataArray>(data.name());
    if (!ida) {
        throw std::runtime_error("FeatureHDF5::data: DataArray not found in block!");
    }
    if (group().hasGroup("data")) {
        group().removeGroup("data");
    }

    auto target = dynamic_pointer_cast<DataArrayHDF5>(ida);

    group().createLink(target->group(), "data");
    targetType(TargetType::DataArray);

    forceUpdatedAt();
}


void FeatureHDF5::data(const DataFrame &data) {
    std::shared_ptr<IDataFrame> idf = block->getEntity<IDataFrame>(data.name());
    if (!idf) {
        throw std::runtime_error("FeatureHDF5::data: DataFrame not found in block!");
    }
    if (group().hasGroup("data")) {
        group().removeGroup("data");
    }

    auto target = dynamic_pointer_cast<DataFrameHDF5>(idf);

    group().createLink(target->group(), "data");
    targetType(TargetType::DataFrame);
    forceUpdatedAt();
}


void FeatureHDF5::data(const std::string &name_or_id) {
    TargetType tt = TargetType::DataArray;
    if (group().hasGroup("data")) {
        group().removeGroup("data");
    }
    std::shared_ptr<IDataArray> ida = block->getEntity<IDataArray>(name_or_id);
    if (!ida) {
        std::shared_ptr<IDataFrame> idf = block->getEntity<IDataFrame>(name_or_id);
        if (!idf) {
            throw std::runtime_error("FeatureHDF5::data: entity is not found in block, neither DataArray nor DataFrame!");
        }
        tt = TargetType::DataFrame;
        auto target = dynamic_pointer_cast<DataFrameHDF5>(idf);
        group().createLink(target->group(), "data");
    } else {
        auto target = dynamic_pointer_cast<DataArrayHDF5>(ida);
        group().createLink(target->group(), "data");
    }
    targetType(tt);
    forceUpdatedAt();
}


shared_ptr<IDataArray> FeatureHDF5::dataArray() const {
    if (targetType() != TargetType::DataArray) {
        throw std::runtime_error("Cannot convert Feature data to DataArray! Feature target is of type DataFrame!");
    }
    shared_ptr<IDataArray> da;
    if (group().hasGroup("data")) {
        H5Group other_group = group().openGroup("data", false);
        da = make_shared<DataArrayHDF5>(file(), block, other_group);
        if (!block->hasEntity(da)) {
            throw std::runtime_error("FeatureHDF5::data: DataArray not found!");
        }
    }
    return da;
}


shared_ptr<IDataFrame> FeatureHDF5::dataFrame() const {
    if (targetType() != TargetType::DataFrame) {
        throw std::runtime_error("Cannot convert Feature data to DataFrame! Feature target is of type DataArray!");
    }
    shared_ptr<IDataFrame> df;
    if (group().hasGroup("data")) {
        H5Group other_group = group().openGroup("data", false);
        df = make_shared<DataFrameHDF5>(file(), block, other_group);

        if (!block->hasEntity(df)) {
            throw std::runtime_error("FeatureHDF5::data: DataFrame not found!");
        }
    }
    return df;
}


LinkType FeatureHDF5::linkType() const {
    if (group().hasAttr("link_type")) {
        string link_type;
        group().getAttr("link_type", link_type);
        return linkTypeFromString(link_type);
    } else {
        throw MissingAttr("link_type");
    }
}

TargetType FeatureHDF5::targetType() const {
    if (group().hasAttr("target_type")) {
        string target_type;
        group().getAttr("target_type", target_type);
        return targetTypeFromString(target_type);
    } else {
        return TargetType::DataArray;
    }
}

FeatureHDF5::~FeatureHDF5() {}

} // ns nix::hdf5
} // ns nix
