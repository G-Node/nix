// Copyright © 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include "DataSpace.hpp"
#include "H5Exception.hpp"


namespace nix {
namespace hdf5 {

DataSpace DataSpace::create(const NDSize &dims, const NDSize &maxdims)
{
    DataSpace space;

    hid_t spaceId;
    if (!dims) {
        spaceId = H5Screate(H5S_SCALAR);
    } else {
        int rank = (int) dims.size();

        if (maxdims) {
            spaceId = H5Screate_simple(rank, dims.data(), maxdims.data());
        } else {
            spaceId = H5Screate_simple(rank, dims.data(), nullptr);
        }
    }

    space = DataSpace(spaceId);
    space.check("Could not create data space");
    return space;
}

DataSpace DataSpace::create(const NDSize &dims, bool maxdims_unlimited)
{

    if (maxdims_unlimited) {
        NDSize maxdims(dims.size(), H5S_UNLIMITED);
        return create(dims, maxdims);
    } else {
        return create(dims);
    }

}

NDSize DataSpace::extent() const {

    int ndims = H5Sget_simple_extent_ndims(hid);
    if (ndims < 0) {
        throw H5Exception("DataSet::size(): could not obtain number of dimensions");
    }
    size_t rank = static_cast<size_t>(ndims);
    NDSize dims(rank);
    int res = H5Sget_simple_extent_dims(hid, dims.data(), nullptr);

    if (res < 0) {
        throw H5Exception("DataSet::size(): could not obtain extents");
    }

    return dims;
}


void DataSpace::hyperslab(const NDSize &count, const NDSize &start, H5S_seloper_t op) {
    HErr status = H5Sselect_hyperslab(hid, op, start.data(), nullptr, count.data(), nullptr);
    status.check("DataSpace::hyperslab(): H5Sselect_hyperslab() failed!");
}

} //::nix::hdf5
} //::nix
