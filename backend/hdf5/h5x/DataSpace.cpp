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
#include <vector>


namespace nix {
namespace hdf5 {

static std::vector<hsize_t> toHSize(const NDSize &ndsize) {
    std::vector<hsize_t> hsizes(ndsize.size());
    for (size_t i = 0; i < ndsize.size(); ++i) {
        hsizes[i] = static_cast<hsize_t>(ndsize[i]);
    }
    return hsizes;
}

DataSpace DataSpace::create(const NDSize &dims, const NDSize &maxdims)
{
    DataSpace space;

    hid_t spaceId;
    if (!dims) {
        spaceId = H5Screate(H5S_SCALAR);
    } else {
        int rank = (int) dims.size();

        if (maxdims) {
            std::vector<hsize_t> h_dims = toHSize(dims);
            std::vector<hsize_t> h_maxdims = toHSize(maxdims);
            spaceId = H5Screate_simple(rank, h_dims.data(), h_maxdims.data());
        } else {
            std::vector<hsize_t> h_dims = toHSize(dims);
            spaceId = H5Screate_simple(rank, h_dims.data(), nullptr);
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
    std::vector<hsize_t> h_dims(rank);
    int res = H5Sget_simple_extent_dims(hid, h_dims.data(), nullptr);

    if (res < 0) {
        throw H5Exception("DataSet::size(): could not obtain extents");
    }

    NDSize dims(rank);
    for (size_t i = 0; i < rank; ++i) {
        dims[i] = static_cast<ndsize_t>(h_dims[i]);
    }
    return dims;
}


void DataSpace::hyperslab(const NDSize &count, const NDSize &start, H5S_seloper_t op) {
    std::vector<hsize_t> h_start = toHSize(start);
    std::vector<hsize_t> h_count = toHSize(count);
    HErr status = H5Sselect_hyperslab(hid, op, h_start.data(), nullptr, h_count.data(), nullptr);
    status.check("DataSpace::hyperslab(): H5Sselect_hyperslab() failed!");
}

} //::nix::hdf5
} //::nix
