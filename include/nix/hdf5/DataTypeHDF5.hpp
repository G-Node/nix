// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef PANDORA_DATA_TYPE_H
#define PANDORA_DATA_TYPE_H

#include <nix/DataType.hpp>
#include <nix/hdf5/hdf5include.hpp>

#include <nix/Platform.hpp>

namespace nix {
namespace hdf5 {

NIXAPI H5::DataType data_type_to_h5_filetype(DataType dtype);


NIXAPI H5::DataType data_type_to_h5_memtype(DataType dtype);

NIXAPI DataType data_type_from_h5(H5T_class_t vclass, size_t vsize, H5T_sign_t vsign);

} // namespace hdf5
} // namespace nix

#endif //PANDORA_DATA_TYPE_H
