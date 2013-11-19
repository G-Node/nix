// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>

#include <nix/hdf5/ValueHDF5.hpp>

using namespace std;

namespace nix {
namespace hdf5 {
namespace hades {


template<>
char *get_inner<char*, std::string>(const std::string &outer) {
    return const_cast<char *>(outer.c_str());
}


template<>
int8_t get_inner<int8_t, bool>(const bool &outer) {
    return static_cast<int8_t>(outer);
}


} // namespace hades
} // namespace hdf5
} // namespace nix
