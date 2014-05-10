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
#include <nix/NDArray.hpp>
#include <algorithm>

using namespace std;

namespace nix {
namespace util {

//TODO maybe a relaxed handling of units would be better, e.g. if none given, take the one from the dimension...
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
        throw nix::OutOfBounds("Position is out of bounds in SampledDimension.", static_cast<int>(position));
    }
    if (dim_unit) {
        try {
            scaling = util::getSIScaling(unit, *dim_unit);
        } catch (...) {
            throw nix::IncompatibleDimensions("Cannot apply a position with unit to a SetDimension", "nix::util::positionToIndex");
        }
    }
    index = static_cast<size_t>(round(position * scaling / dimension.samplingInterval()));
    return index;
}


size_t positionToIndex(double position, const string &unit, const SetDimension &dimension) {
    size_t index;
    index = static_cast<size_t>(round(position));
    if (unit.length() > 0) {
        throw nix::IncompatibleDimensions("Cannot apply a position with unit to a SetDimension", "nix::util::positionToIndex");
    }
    if (static_cast<size_t>(index) < dimension.labels().size()){
        return index;
    } else {
        throw nix::OutOfBounds("Position is out of bounds in setDimension.", static_cast<int>(position));
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


void getOffsetAndCount(const DataTag &tag, const DataArray &array, size_t index, NDSize &offsets, NDSize &counts) {
    DataArray positions = tag.positions();
    DataArray extents = tag.extents();
    NDSize position_extent = positions.getDataExtent();
    NDSize extent_extent = extents.getDataExtent();
    size_t dimension_count = array.dimensionCount();

    if (index >= position_extent[0] || index >= extent_extent[0]) {
        throw nix::OutOfBounds("Index out of bounds of positions or extents!", 0);
    }
    if (position_extent[1] > dimension_count || extent_extent[1] > dimension_count) {
        throw nix::IncompatibleDimensions("Number of dimensions in position or extent do not match dimensionality of data","util::getOffsetAndCount");
    }
    NDSize temp_offset = NDSize{static_cast<NDSize::value_type>(index), static_cast<NDSize::value_type>(0)};
    NDSize temp_count{static_cast<NDSize::value_type>(1), static_cast<NDSize::value_type>(dimension_count)};
    NDArray offset(positions.getDataType(), temp_count);
    NDArray extent(extents.getDataType(), temp_count);
    positions.getData(offset, temp_count, temp_offset);
    extents.getData(extent, temp_count, temp_offset);

    NDSize data_offset(dimension_count, static_cast<size_t>(0));
    NDSize data_count(dimension_count, static_cast<size_t>(0));

    for (size_t i = 0; i < offset.num_elements(); ++i) {
        Dimension dimension = array.getDimension(i+1);
        string unit = "";
        if (dimension.dimensionType() == nix::DimensionType::Sample) {
            SampledDimension dim;
            dim = dimension;
            unit = dim.unit() ? *dim.unit() : "";
        } else if (dimension.dimensionType() == nix::DimensionType::Range) {
            RangeDimension dim;
            dim = dimension;
            unit = dim.unit() ? *dim.unit() : "";
        }
        data_offset[i] = positionToIndex(offset.get<double>(i), unit, dimension);

        if (i < extent.num_elements()) {
            data_count[i] = 1 + positionToIndex(offset.get<double>(i) + extent.get<double>(i), unit, dimension) - data_offset[i];
        }
    }
    offsets = data_offset;
    counts = data_count;
}


bool positionInData(const DataArray &data, const NDSize &position) {
    NDSize data_size = data.getDataExtent();
    bool valid = true;

    if (!(data_size.size() == position.size())) {
        return false;
    }
    for (size_t i = 0; i < data_size.size(); i++) {
        valid &= position[i] < data_size[i];
    }
    return valid;
}


bool positionAndExtentInData(const DataArray &data, const NDSize &position, const NDSize &count) {
    NDSize pos = position + count;
    pos -= 1;
    return positionInData(data, pos);
}

} // namespace util
} // namespace nix
