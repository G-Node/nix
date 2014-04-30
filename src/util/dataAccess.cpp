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
#include <boost/optional.hpp>
#include <nix/util/dataAccess.hpp>
#include <algorithm>

using namespace std;

namespace nix {
namespace util {


size_t positionToIndex(double position, const string &unit, const SampledDimension &dimension) {
    size_t index;
    boost::optional<string> dim_unit = dimension.unit();
    double scaling = 1.0;
    if ((!dim_unit && unit.length() > 0) || (dim_unit && unit.length() == 0)) {
        throw nix::IncompatibleDimensions("Units of position and SampledDimension must both be given!", "nix::util::positionToIndex");
    }
    if ((dimension.offset() && position < *dimension.offset()) || (!dimension.offset() && position < 0.0)) {
        throw nix::OutOfBounds("Position is out of bounds in SampledDimension.", (int)position);
    }
    if (dim_unit) {
        try {
            scaling = util::getSIScaling(unit, *dim_unit);
        } catch (...) {
            throw nix::IncompatibleDimensions("Cannot apply a position with unit to a SetDimension", "nix::util::positionToIndex");
        }
    }
    index = (size_t)round(position * scaling / dimension.samplingInterval());
    return index;
}


size_t positionToIndex(double position, const string &unit, const SetDimension &dimension) {
    size_t index;
    index = (size_t) round(position);
    if (unit.length() > 0) {
        throw nix::IncompatibleDimensions("Cannot apply a position with unit to a SetDimension", "nix::util::positionToIndex");
    }
    if (index >= 0 && (size_t)index < dimension.labels().size()){
        return index;
    } else {
        throw nix::OutOfBounds("Position is out of bounds in setDimension.", (int)position);
    }
    return index;
}


size_t positionToIndex(double position, const string &unit, const RangeDimension &dimension) {
    size_t index;
    boost::optional<string> dim_unit = dimension.unit();
    double scaling = 1.0;

    if ((!dim_unit && unit.length() > 0) || (dim_unit && unit.length() == 0)) {
        throw nix::IncompatibleDimensions("Units of position and RangeDimension must both be given!", "nix::util::positionToIndex");
    }
    if (dim_unit) {
        try {
            scaling = util::getSIScaling(unit, *dim_unit);
        } catch (...) {
            throw nix::IncompatibleDimensions("Cannot apply a position with unit to a SetDimension", "nix::util::positionToIndex");
        }
    }
    vector<double> ticks = dimension.ticks();
    if (position*scaling < *ticks.begin()) {
        return 0;
    } else if (position*scaling > *prev(ticks.end())) {
        return prev(ticks.end()) - ticks.begin();
    }
    vector<double>::iterator low = std::lower_bound (ticks.begin(), ticks.end(), position * scaling);
    if (*low == position) {
        return low - ticks.begin();
    }
    if (low != ticks.begin() && *low != position * scaling) {
        double diff_low, diff_before;
        diff_low = fabs(*low - position);
        diff_before = fabs(*(std::prev(low)) - position * scaling);
        if (diff_low < diff_before) {
            index = low - ticks.begin();
        } else {
            index = low - ticks.begin() - 1;
        }
        return index;
    } else {
        return low - ticks.begin();
    }
}

} // namespace util
} // namespace nix
