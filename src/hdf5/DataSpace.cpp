// Copyright © 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <nix/hdf5/DataSpace.hpp>

namespace nix {
namespace hdf5 {
namespace DataSpace {


H5::DataSpace create(const NDSize &dims, const NDSize *maxdims)
{
    H5::DataSpace space;

    if (!dims) {
        space = H5::DataSpace();
        return space;
    }

    int rank = (int) dims.size();
    if (maxdims != nullptr) {
        space = H5::DataSpace(rank, dims.data(), maxdims->data());
    } else {
        space = H5::DataSpace(rank, dims.data());
    }

    return space;
}

H5::DataSpace create(const NDSize &dims, bool maxdimsUnlimited)
{

    if (maxdimsUnlimited) {
        NDSize maxdims(dims.size(), H5S_UNLIMITED);
        return create(dims, &maxdims);
    } else {
        return create(dims, nullptr);
    }

}


} //::nix::hdf5::DataSpace
} //::nix::hdf5
} //::nix
