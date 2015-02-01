// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/Selection.hpp>
#include <nix/hdf5/ExceptionHDF5.hpp>

namespace nix {
namespace hdf5 {


void Selection::offset(const NDSSize &offset)
{
    herr_t status = H5Soffset_simple(space.h5id(), offset.data());
    H5Error::check(status, "Selection::offset(): Could not set offset");
}


void Selection::select(const NDSize &count, const NDSize &start, Mode mode)
{
    H5S_seloper_t op = static_cast<H5S_seloper_t>(mode);
    herr_t status = H5Sselect_hyperslab(space.h5id(), op, start.data(), nullptr, count.data(), nullptr);
    H5Error::check(status, "Selection::select(): Could not select hyperslab");
}


NDSize Selection::size() const
{
    size_t rank = this->rank();
    NDSize start(rank), end(rank);
    bounds(start, end);
    return (end - start) + 1;
}

bool Selection::isValid() const
{
    htri_t res = H5Sselect_valid(space.h5id());
    return res > 0; //FIXME: check error
}

void Selection::bounds(NDSize &start, NDSize &end) const
{
    herr_t status = H5Sget_select_bounds(space.h5id(), start.data(), end.data());
    H5Error::check(status, "Selection::bounds(): Could not get bounds");
}

size_t Selection::rank() const
{
    return static_cast<size_t>(H5Sget_simple_extent_ndims(space.h5id()));
}

} // namespace hdf5
} // namespace nix
