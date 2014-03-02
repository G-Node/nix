// Copyright © 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <nix/hdf5/hdf5include.hpp>
#include <nix/NDSize.hpp>

#include <nix/Platform.hpp>

#ifndef NIX_DATASPACE_H
#define NIX_DATASPACE_H

namespace nix {
namespace hdf5 {
namespace DataSpace {

H5::DataSpace NIXAPI create(const NDSize &dims, const NDSize &maxdims = {});
H5::DataSpace NIXAPI create(const NDSize &dims, bool maxdimsUnlimited);

} //::nix::hdf5::DataSpace
} //::nix::hdf5
} //::nix

#endif
