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

#include <nix/hdf5/ExceptionHDF5.hpp>
#include <nix/hdf5/hdf5include.hpp>

namespace nix {

namespace hdf5 {

LocID::LocID() : BaseHDF5() {}


LocID::LocID(hid_t hid) : BaseHDF5(hid) {}


LocID::LocID(const LocID &other) : BaseHDF5(other) {}


bool LocID::hasAttr(const std::string &name) const {
    HTri res = H5Aexists(hid, name.c_str());
    return res.check("LocID.hasAttr() failed");
}


void LocID::removeAttr(const std::string &name) const {
    HErr res = H5Adelete(hid, name.c_str());
    res.check("LocID::removeAttr(): could not delete attribute");
}


Attribute LocID::openAttr(const std::string &name) const {
    Attribute attr = H5Aopen(hid, name.c_str(), H5P_DEFAULT);
    attr.check("LocID::openAttr: Could not open attribute " + name);
    return attr;
}


Attribute LocID::createAttr(const std::string &name, H5::DataType fileType, const DataSpace &fileSpace) const {
    Attribute attr = H5Acreate(hid, name.c_str(), fileType.getId(), fileSpace.h5id(), H5P_DEFAULT, H5P_DEFAULT);
    attr.check("LocID::openAttr: Could not create attribute " + name);
    return attr;
}


void LocID::deleteLink(std::string name, hid_t plist) {
    HErr res = H5Ldelete(hid, name.c_str(), plist);
    res.check("LocIDL::deleteLink: Could not delete link: " + name);
}
} // nix::hdf5

} // nix::