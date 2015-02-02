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
#include <H5Ipublic.h>
#include <nix/hdf5/ExceptionHDF5.hpp>

namespace nix {

namespace hdf5 {

LocID::LocID() : BaseHDF5() {}


LocID::LocID(hid_t hid) : BaseHDF5(hid) {}


LocID::LocID(const LocID &other) : BaseHDF5(other) {}


bool LocID::hasAttr(const std::string &name) const {
    return H5Aexists(hid, name.c_str());
}


void LocID::removeAttr(const std::string &name) const {
    H5Adelete(hid, name.c_str());
}


Attribute LocID::openAttr(const std::string &name) const {
    hid_t ha = H5Aopen(hid, name.c_str(), H5P_DEFAULT);
    Attribute attr = Attribute(ha);
    H5Idec_ref(ha);
    return attr;
}


Attribute LocID::createAttr(const std::string &name, H5::DataType fileType, const DataSpace &fileSpace) const {
    hid_t ha = H5Acreate(hid, name.c_str(), fileType.getId(), fileSpace.h5id(), H5P_DEFAULT, H5P_DEFAULT);
    Attribute attr = Attribute(ha);
    H5Idec_ref(ha);
    return attr;
}


void LocID::deleteLink(std::string name, hid_t plist) {
    herr_t res = H5Ldelete(hid, name.c_str(), plist);
    H5Error::check(res, "LocIDL::deleteLink: Could not delete link: " + name);
}
} // nix::hdf5

} // nix::