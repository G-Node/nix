// Copyright © 2020 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <christian@kellner.me>

#pragma once

#include "H5Object.hpp"

namespace nix {
namespace hdf5 {

class NIXAPI PList : public H5Object {
public:

    PList();
    PList(hid_t hid);
    PList(hid_t hid, bool is_copy) : H5Object(hid, is_copy) { }
    PList(const PList &other);

    PList &operator=(const PList &other) {
        H5Object::operator= (other);
        return *this;
    }

    // creators
    static PList create(hid_t cls_id);

    void charEncoding(H5T_cset_t encoding);
    H5T_cset_t charEncoding() const;

    // helper
    static PList linkUTF8();
};

}
}
