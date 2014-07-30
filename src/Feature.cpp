// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/Feature.hpp>

namespace nix {

void Feature::data(const DataArray &data) {
    if(data == none) {
        throw std::runtime_error("Empty data entity (DataArray) given");
    }
    else {
        backend()->data(data.id());
    }
}

}
