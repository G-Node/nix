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

//TODO maybe a relaxed handling of units would be better, e.g. if none given, take the one from the dimension...
// kind of an example why an virtual method in Dimension would not be too bad... :-)
int positionToIndex(double position, const string &unit, const Dimension &dimension) {
    if (dimension.dimensionType() == nix::DimensionType::Sample) {
        SampledDimension dim;
        dim = dimension;
        return positionToIndex(position, unit, dim);
    } else if (dimension.dimensionType() == nix::DimensionType::Set) {
        SetDimension dim;
        dim = dimension;
        return positionToIndex(position, unit, dim);
    } else {
        RangeDimension dim;
        dim = dimension;
        return positionToIndex(position, unit, dim);
    }
}


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
    if ((size_t)index < dimension.labels().size()){
        return index;
    } else {
        throw nix::OutOfBounds("Position is out of bounds in setDimension.", (int)position);
    }
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


void getOffsetAndCount(const SimpleTag &tag, const DataArray &array, NDSize &offset, NDSize &count) {
    vector<double> position = tag.position();
    vector<double> extent = tag.extent();
    vector<string> units = tag.units();
    NDSize temp_offset(position.size());
    NDSize temp_count(position.size(), 1);

    if (array.dimensionCount() != position.size() || (extent.size() > 0 && extent.size() != array.dimensionCount())) {
        throw std::runtime_error("Dimensionality of position or extent vector does not match dimensionality of data!");
    }
    for (size_t i = 0; i < position.size(); ++i) {
        Dimension dim = array.getDimension(i+1);
        temp_offset[i] = positionToIndex(position[i], i > units.size() ? "" : units[i], dim);
        if (i < extent.size()) {
            temp_count[i] = 1 + positionToIndex(position[i] + extent[i], i > units.size() ? "" : units[i], dim) - temp_offset[i];
        }
    }
    offset = temp_offset;
    count = temp_count;
}

} // namespace util
} // namespace nix
