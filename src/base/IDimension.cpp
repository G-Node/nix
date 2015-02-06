// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_DIMENSIONS_CPP
#define NIX_I_DIMENSIONS_CPP

#include <nix/base/IDimensions.hpp>

namespace nix {

std::ostream& operator<<(std::ostream &out, const DimensionType &dtype) {
    if (dtype == DimensionType::Sample) {
        out << "Sample";
    }
    if (dtype == DimensionType::Set) {
        out << "Set";
    }
    if (dtype == DimensionType::Range) {
        out << "Range";
    }
    
    return out;
}

}

#endif
