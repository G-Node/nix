// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/RepresentationHDF5.hpp>

using namespace std;

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


RepresentationHDF5::RepresentationHDF5(const RepresentationHDF5 &representation)
    : EntityHDF5(representation.file(), representation.group(), representation.id()),
      block(representation.block)
{}


RepresentationHDF5::RepresentationHDF5(const File &file, const Block &block, const Group &group,
                                       const string &id, DataArray _data, LinkType _link_type)
    : EntityHDF5(file, group, id), block(block)
{
    linkType(_link_type);
    // TODO: the line below currently throws an exception if the DataArray
    // is not in block - to consider if we prefer copying it to the block
    data(_data.id());
}


RepresentationHDF5::RepresentationHDF5(const File &file, const Block &block, const Group &group,
                                       const string &id, DataArray _data, LinkType _link_type, time_t time)
    : EntityHDF5(file, group, id, time), block(block)
{
    linkType(_link_type);
    // TODO: the line below currently throws an exception if the DataArray
    // is not in block - to consider if we prefer copying it to the block
    data(_data.id()); 
}


void RepresentationHDF5::linkType(LinkType link_type) {
    // linkTypeToString will generate an error if link_type is invalid
    group().setAttr("link_type", linkTypeToString(link_type));
    forceUpdatedAt();
}


void RepresentationHDF5::data(const std::string &data_array_id) {
    if(data_array_id.empty()) {
        throw EmptyString("data DataArray id");
    }
    else {
        if(!block.hasDataArray(data_array_id)) {
            throw runtime_error("RepresentationHDF5::data: cannot set Representation data because referenced DataArray does not exist!");
        } else {
            group().setAttr("data", data_array_id);
            forceUpdatedAt();
        }
    }
}


DataArray RepresentationHDF5::data() const {
    if(group().hasAttr("data")) {
        string dataId;
        group().getAttr("data", dataId);
        if(block.hasDataArray(dataId)) {
            return block.getDataArray(dataId);
        } else {
            throw std::runtime_error("Data array not found by id in Block");
        }
    } else {
        throw MissingAttr("data");
    }
}


LinkType RepresentationHDF5::linkType() const {
    if(group().hasAttr("link_type")) {
        string link_type;
        group().getAttr("link_type", link_type);
        return linkTypeFromString(link_type);
    } else {
        throw MissingAttr("data");
    }
}


RepresentationHDF5::~RepresentationHDF5() {}


} // namespace hdf5
} // namespace nix
