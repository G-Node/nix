// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>

#include <pandora/Value.hpp>
#include <pandora/Property.hpp>

using namespace std;

namespace pandora {
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
} // namespace pandora
