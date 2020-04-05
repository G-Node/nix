// Copyright © 2020 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <christian@kellner.me>

#include "H5PList.hpp"

namespace nix {
namespace hdf5 {

PList::PList() : H5Object() { }

PList::PList(hid_t hid) : H5Object(hid) { }

PList::PList(const PList &other) : H5Object(other) { }

PList PList::create(hid_t cls_id) {
    PList pl = H5Pcreate(cls_id);
    pl.check("H5Pcreate: could not create property list");
    return pl;
}

void PList::charEncoding(H5T_cset_t encoding) {
    HErr res = H5Pset_char_encoding(hid, encoding);
    res.check("Could not set character encoding on Property List");
}

H5T_cset_t PList::charEncoding() const {
    H5T_cset_t encoding;
    HErr res = H5Pget_char_encoding(hid, &encoding);
    res.check("Could not get character encoding on Property List");
    return encoding;
}

PList PList::linkUTF8() {
    PList pl = PList::create(H5P_LINK_CREATE);
    pl.charEncoding(H5T_CSET_UTF8);
    return pl;
}


} // nix::hdf5
} // nix::
