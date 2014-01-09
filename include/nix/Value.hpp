// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_VALUE_H
#define NIX_VALUE_H

#include <string>

namespace nix {


template<typename T>
struct Value {

    T value;
    double uncertainty;

    std::string reference;
    std::string filename;
    std::string encoder;
    std::string checksum;

};


} // namespace nix

#endif // NIX_VALUE_H
