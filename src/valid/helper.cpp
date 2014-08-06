// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/valid/helper.hpp>

#include <nix.hpp>

namespace nix {
namespace valid {

std::vector<std::string> getDimensionsUnits(DataArray darray) {
    std::vector<std::string> units;

    for(auto &dim : darray.dimensions()) {
        if(dim.dimensionType() == DimensionType::Range) {
            auto d = dim.asRangeDimension();
            units.push_back(d.unit() ? *(d.unit()) : std::string());
        }
        if(dim.dimensionType() == DimensionType::Sample) {
            auto d = dim.asSampledDimension();
            units.push_back(d.unit() ? *(d.unit()) : std::string());
        }
        if(dim.dimensionType() == DimensionType::Set) {
            units.push_back(std::string());
        }
    }

    return units;
}

} // namespace valid
} // namespace nix
