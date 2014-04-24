// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <string>
#include <cstdlib>
#include <cmath>

#include <nix/util/util.hpp>
#include <nix.hpp>

using namespace std;

namespace nix {
namespace util {


int positionToIndex(double position, const string &unit, const Dimension &dimension) {
    int index = -1;
    if (dimension.dimensionType() == nix::DimensionType::Set) {
        index = (int) position;
        SetDimension dim;
        dim = dimension;
        if ((size_t)index < dim.labels().size()){
            return index;
        } else {
            throw nix::OutOfBounds("Position is out of bounds in setDimension.", index);
        }
    } else if (dimension.dimensionType() == nix::DimensionType::Sample) {
        SampledDimension dim;
        dim = dimension;
        if (dim.unit()) {
            double scaling = util::getSIScaling(unit, *dim.unit());
            index = (int)round(position * scaling / dim.samplingInterval());
        }
        return index;
    } else if (dimension.dimensionType() == nix::DimensionType::Range) {

    }
    return index;
}


} // namespace util
} // namespace nix
