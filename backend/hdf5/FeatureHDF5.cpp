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
    this->data(data.id());
}


void FeatureHDF5::linkType(LinkType link_type) {
    // linkTypeToString will generate an error if link_type is invalid
    group().setAttr("link_type", linkTypeToString(link_type));
    forceUpdatedAt();
}


void FeatureHDF5::data(const std::string &name_or_id) {
    std::shared_ptr<IDataArray> ida = block->getEntity<IDataArray>(name_or_id);
    if (!ida) {
        throw std::runtime_error("FeatureHDF5::data: DataArray not found in block!");
    }
    if (group().hasGroup("data")) {
        group().removeGroup("data");
    }

    auto target = dynamic_pointer_cast<DataArrayHDF5>(ida);

    group().createLink(target->group(), "data");
    forceUpdatedAt();
}


shared_ptr<IDataArray> FeatureHDF5::data() const {
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


LinkType FeatureHDF5::linkType() const {
    if (group().hasAttr("link_type")) {
        string link_type;
        group().getAttr("link_type", link_type);
        return linkTypeFromString(link_type);
    } else {
        throw MissingAttr("data");
    }
}


FeatureHDF5::~FeatureHDF5() {}

} // ns nix::hdf5
} // ns nix
