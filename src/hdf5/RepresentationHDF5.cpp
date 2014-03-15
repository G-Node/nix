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
                                       const string &id)
    : EntityHDF5(file, group, id), block(block)
{}


RepresentationHDF5::RepresentationHDF5(const File &file, const Block &block, const Group &group,
                                       const string &id, time_t time)
    : EntityHDF5(file, group, id, time), block(block)
{}


void RepresentationHDF5::linkType(LinkType link_type) {
    group().setAttr("link_type", linkTypeToString(link_type));
    forceUpdatedAt();
}


<<<<<<< HEAD
void RepresentationHDF5::data(const DataArray &data){
    if(data == none) {
        throw runtime_error("Empty data entity (DataArray) given");
    }
    else {
        group().setAttr("data", data.id());
        forceUpdatedAt();
    }
=======
void RepresentationHDF5::data(const std::string &data_array_id){
    group().setAttr("data", data_array_id);
    forceUpdatedAt();
>>>>>>> refactoring of user interface classes.
}


DataArray RepresentationHDF5::data() const{
    string dataId;
    group().getAttr("data", dataId);
    return block.getDataArray(dataId);
}


LinkType RepresentationHDF5::linkType() const {
    string link_type;
    group().getAttr("link_type", link_type);
    return linkTypeFromString(link_type);
}


RepresentationHDF5::~RepresentationHDF5() {}


} // namespace hdf5
} // namespace nix
