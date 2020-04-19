// Copyright © 2015 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include "LocID.hpp"
#include "H5PList.hpp"

namespace nix {

namespace hdf5 {

LocID::LocID() : H5Object() {}


LocID::LocID(hid_t hid) : H5Object(hid) {}


LocID::LocID(const LocID &other) : H5Object(other) {}


void LocID::linkInfo(const std::string &name, H5L_info_t &info) const {
    HErr res = H5Lget_info(hid, name.c_str(), &info, H5P_DEFAULT);
    res.check("LocID::linkInfo(): H5Lget_info() failed");
}

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


Attribute LocID::createAttr(const std::string &name, h5x::DataType fileType, const DataSpace &fileSpace) const {
    PList acpl = PList::create(H5P_ATTRIBUTE_CREATE);
    acpl.charEncoding(H5T_CSET_UTF8);

    Attribute attr = H5Acreate(hid,
                               name.c_str(),
                               fileType.h5id(),
                               fileSpace.h5id(),
                               acpl.h5id(),
                               H5P_DEFAULT);
    attr.check("LocID::openAttr: Could not create attribute " + name);
    return attr;
}


void LocID::deleteLink(std::string name, hid_t plist) {
    HErr res = H5Ldelete(hid, name.c_str(), plist);
    res.check("LocIDL::deleteLink: Could not delete link: " + name);
}


unsigned int LocID::referenceCount() const {
    H5O_info_t oInfo;
    HErr res = H5Oget_info(hid, &oInfo);
    res.check("LocID:referenceCount: Coud not get object info");
    return oInfo.rc;
}
} // nix::hdf5

} // nix::
