// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/Selection.hpp>

namespace nix {
namespace hdf5 {


void Selection::offset(const NDSSize &offset)
{
    space.offsetSimple(offset.data());
}


void Selection::select(const NDSize &count, const NDSize &start, Mode mode)
{
    H5S_seloper_t op = static_cast<H5S_seloper_t>(mode);
    space.selectHyperslab(op, count.data(), start.data());
}


void Selection::select(Preset set)
{

    switch(set) {
        case Preset::None:
            space.selectNone();
            return;
        case Preset::All:
            space.selectAll();
            return;
    }
}


NDSize Selection::size() const
{
    size_t rank = this->rank();
    NDSize start(rank), end(rank);
    bounds(start, end);
    return (end - start) + 1;
}


} // namespace hdf5
} // namespace nix
