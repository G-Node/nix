// Copyright © 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <nix/NDSize.hpp>
#include <nix/Platform.hpp>

#include "H5Object.hpp"

#ifndef NIX_DATASPACE_H
#define NIX_DATASPACE_H

namespace nix {
namespace hdf5 {

class NIXAPI DataSpace : public H5Object {
public:

    DataSpace() : H5Object(H5S_ALL) { }
    DataSpace(hid_t space) : H5Object(space) { }
    DataSpace(const DataSpace &other) : H5Object(other) { }

    NDSize extent() const;

    static DataSpace create(const NDSize &dims, const NDSize &maxdims = {});
    static DataSpace create(const NDSize &dims, bool maxdims_unlimited);

};

} //::nix::hdf5
} //::nix

#endif
