// Copyright © 2015 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <nix/hdf5/LocID.hpp>

namespace nix {

namespace hdf5 {

LocID::LocID() : BaseHDF5() {}


LocID::LocID(hid_t hid) : BaseHDF5(hid) {}


LocID::LocID(const LocID &other) : BaseHDF5(other) {}


LocID::LocID(const H5::Group &h5group) : LocID(h5group.getLocId()) {}

bool LocID::hasAttr(const std::string &name) const {
    return H5Aexists(hid, name.c_str());
}


void LocID::removeAttr(const std::string &name) const {
    H5Adelete(hid, name.c_str());
}


H5::Attribute LocID::openAttr(const std::string &name) const {
    hid_t ha = H5Aopen(hid, name.c_str(), H5P_DEFAULT);
    return H5::Attribute(ha);
}


H5::Attribute LocID::createAttr(const std::string &name, H5::DataType fileType, H5::DataSpace fileSpace) const {
    hid_t ha = H5Acreate(hid, name.c_str(), fileType.getId(), fileSpace.getId(), H5P_DEFAULT, H5P_DEFAULT);
    return H5::Attribute(ha);
}


void LocID::readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, void *data) {
    attr.read(mem_type, data);
}


void LocID::readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, std::string *data) {
    StringWriter writer(size, data);
    attr.read(mem_type, *writer);
    writer.finish();
    H5::DataSet::vlenReclaim(*writer, mem_type, attr.getSpace()); //recycle space?
}


void LocID::writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const void *data) {
    attr.write(mem_type, data);
}


void LocID::writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const std::string *data) {
    StringReader reader(size, data);
    attr.write(mem_type, *reader);
}

} // nix::hdf5

} // nix::