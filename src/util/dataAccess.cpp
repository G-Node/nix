// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/dataAccess.hpp>

#include <nix/util/util.hpp>

#include <string>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <cfloat>

#include <boost/optional.hpp>

using namespace std;

namespace nix {
namespace util {


void scalePositions(const vector<double> &starts, const vector<double> &ends,
                    const vector<string> &units, const string & dim_unit,
                    vector<double> &scaled_starts, vector<double> &scaled_ends) {
    size_t count = min(starts.size(), ends.size());
    if (scaled_starts.size() != count)
        scaled_starts.resize(count);
    if (scaled_ends.size() != count)
        scaled_ends.resize(count);
    double scaling= 1.0;
    for (size_t i = 0; i < count; ++i) {
        if (i < units.size() && units[i] != "none" && dim_unit != "none") {
            try {
                scaling = util::getSIScaling(units[i], dim_unit);
            } catch (...) {
                throw nix::IncompatibleDimensions("Provided units are not scalable!",
                                                  "nix::util::positionToIndex");
            }
        }
        scaled_starts[i] = starts[i] * scaling;
        scaled_ends[i] = ends[i] * scaling;
    }
}


vector<pair<ndsize_t, ndsize_t>> positionToIndex(const vector<double> &start_positions,
                                                 const vector<double> end_positions,
                                                 const vector<string> &units,
                                                 const Dimension &dimension) {
    vector<pair<ndsize_t, ndsize_t>> indices;
    if (dimension.dimensionType() == DimensionType::Sample) {
        SampledDimension dim;
        dim = dimension;
        indices = positionToIndex(start_positions, end_positions, units, dim);
    } else if (dimension.dimensionType() == DimensionType::Set) {
        SetDimension dim;
        dim = dimension;
        indices = positionToIndex(start_positions, end_positions, units, dim);
    } else if (dimension.dimensionType() == DimensionType::DataFrame) {
        DataFrameDimension dim;
        dim = dimension;
        indices = positionToIndex(start_positions, end_positions, units, dim);
    } else {
        RangeDimension dim;
        dim = dimension;
        indices = positionToIndex(start_positions, end_positions, units, dim);
    }
    return indices;
}


ndsize_t positionToIndex(double position, const string &unit, const SampledDimension &dimension) {
    ndsize_t index;
    boost::optional<string> dim_unit = dimension.unit();
    double scaling = 1.0;
    if (!dim_unit && unit != "none") {
        throw IncompatibleDimensions("Units of position and SampledDimension must both be given!",
                                     "util::positionToIndex");
    }
    if (dim_unit && unit != "none") {
        try {
            scaling = util::getSIScaling(unit, *dim_unit);
        } catch (...) {
            throw IncompatibleDimensions("Cannot apply a position with unit to a SetDimension",
                                         "nix::util::positionToIndex");
        }
    }
    index = dimension.indexOf(position * scaling);
    return index;
}


ndsize_t positionToIndex(double position, const string &unit, const SetDimension &dimension) {
    ndsize_t index;
    if (unit.length() > 0 && unit != "none") {
        throw IncompatibleDimensions("Cannot apply a position with unit to a SetDimension",
                                     "nix::util::positionToIndex");
    }
    index = static_cast<ndsize_t>(round(position));
    if (round(position) < 0.0 || (dimension.labels().size() > 0 && index >= dimension.labels().size())) {
        throw OutOfBounds("Position is out of bounds of the given SetDimension.", static_cast<int>(position));
    }
    return index;
}


ndsize_t positionToIndex(double position, const string &unit, const DataFrameDimension &dimension) {
    ndsize_t index;
    if (unit.length() > 0 && unit != "none") {
        throw IncompatibleDimensions("Cannot apply a position with unit to a DataFrameDimension",
                                     "nix::util::positionToIndex");
    }
    index = static_cast<ndsize_t>(round(position));
    if (round(position) < 0 || index >= dimension.size()) {
        throw OutOfBounds("Position is out of bounds of the given DataFrameDimension.", static_cast<int>(position));
    }
    return index;
}

vector<pair<ndsize_t, ndsize_t>> positionToIndex(const vector<double> &start_positions,
                                                 const vector<double> &end_positions,
                                                 const vector<string> &units,
                                                 const SampledDimension &dimension) {
    ndsize_t count = min(start_positions.size(), end_positions.size());
    vector<double> scaled_start(static_cast<size_t>(count));
    vector<double> scaled_end(static_cast<size_t>(count));
    string dim_unit = getDimensionUnit(dimension);
    scalePositions(start_positions, end_positions, units, dim_unit, scaled_start, scaled_end);
    return dimension.indexOf(scaled_start, scaled_end);
}


vector<pair<ndsize_t, ndsize_t>> positionToIndex(const vector<double> &start_positions,
                                                 const vector<double> &end_positions,
                                                 const vector<string> &units,
                                                 const SetDimension &dimension) {
    vector<pair<ndsize_t, ndsize_t>> indices;

    for (size_t i = 0; i < (min(start_positions.size(), end_positions.size())); ++i) {
        if (start_positions[i] > end_positions[i] ) {
            continue;
        }
        indices.emplace_back(static_cast<ndsize_t>(round(start_positions[i])),
                             static_cast<ndsize_t>(end_positions[i]));
    }
    return indices;
}


vector<pair<ndsize_t, ndsize_t>> positionToIndex(const vector<double> &start_positions,
                                                 const vector<double> &end_positions,
                                                 const vector<string> &units,
                                                 const DataFrameDimension &dimension) {
    vector<pair<ndsize_t, ndsize_t>> indices;
    ndsize_t df_size = dimension.size();
    double min_start = *std::min_element(start_positions.begin(), start_positions.end());
    double min_end = *std::min_element(end_positions.begin(), end_positions.end());
    double max_start = *std::max_element(start_positions.begin(), start_positions.end());
    double max_end = *std::max_element(end_positions.begin(), end_positions.end());

    if (round(min_start) < 0 || round(min_end) < 0) {
        throw nix::OutOfBounds("dataAccess::positionToIndex: min start or end index < 0");
    }
    if (static_cast<ndsize_t>(round(max_start)) >= df_size || static_cast<ndsize_t>(round(max_end)) >= df_size) {
        throw nix::OutOfBounds("dataAccess::positionToIndex: max start or end index >= size of DataFrame");
    }
    for (size_t i = 0; i < (min(start_positions.size(), end_positions.size())); ++i) {
        if (start_positions[i] > end_positions[i] ) {
            continue;
        }
        ndsize_t start = static_cast<ndsize_t>(round(start_positions[i]));
        ndsize_t end = static_cast<ndsize_t>(round(end_positions[i]));
        indices.emplace_back(start, end);
    }
    return indices;
}


vector<pair<ndsize_t, ndsize_t>> positionToIndex(const vector<double> &start_positions,
                                                 const vector<double> &end_positions,
                                                 const vector<string> &units,
                                                 const RangeDimension &dimension) {
    size_t count = min(start_positions.size(), end_positions.size());
    vector<double> scaled_start(count);
    vector<double> scaled_end(count);
    string dim_unit = getDimensionUnit(dimension);
    scalePositions(start_positions, end_positions, units, dim_unit, scaled_start, scaled_end);
    return dimension.indexOf(scaled_start, scaled_end);
}


ndsize_t positionToIndex(double position, const string &unit, const RangeDimension &dimension) {
    string dim_unit = dimension.unit() ? *dimension.unit() : "none";
    double scaling = 1.0;
    if (unit != "none") {
        try {
            scaling = util::getSIScaling(unit, dim_unit);
        } catch (...) {
            throw IncompatibleDimensions("Provided units are not scalable!",
                                         "nix::util::positionToIndex");
        }
    }
    return dimension.indexOf(position * scaling);
}


void getMaxExtent(const Dimension &dim, ndsize_t max_index, double &pos, double &ext) {
    DimensionType dt = dim.dimensionType();
    std::string double_fail_msg("dataAccess::fillPositionsExtents: shape cannot be cast to double without loss of resolution. Please open an issue on github!");
    if (dt == DimensionType::Sample) {
        SampledDimension sd = dim.asSampledDimension();
        pos = sd.positionAt(0);
        ext = sd.positionAt(max_index);
    } else if (dt == DimensionType::Range) {
        RangeDimension rd = dim.asRangeDimension();
        pos = rd.tickAt(0);
        ext = rd.tickAt(max_index);
    } else if (dt == DimensionType::Set || dt == DimensionType::DataFrame) {
        pos = 0.0;
        ext = check::converts_to_double(max_index, double_fail_msg);
    }
}


vector<pair<double, double>> maximumExtents(const DataArray &array) {
    vector<Dimension> dimensions = array.dimensions();
    vector<pair<double, double>> max_extents;
    NDSize shape = array.dataExtent();
    double pos, ext;
    for (size_t i = 0; i < dimensions.size(); ++i) {
        getMaxExtent(dimensions[i], shape[i]-1, pos, ext);
        max_extents.emplace_back(pos, ext);
    }
    return max_extents;
}


string getDimensionUnit(const Dimension &dim) {
    DimensionType dim_type = dim.dimensionType();
    string unit;
    switch (dim_type) {
    case DimensionType::Set:
        unit = "none";
        break;
    case DimensionType::DataFrame: {
        DataFrameDimension df_dim = dim.asDataFrameDimension();
        unit = "none";
        if (df_dim.columnIndex()) {
            unit = df_dim.unit();
        }
        if (unit.empty()) {
            unit = "none";
        }
        break;
    }
    case DimensionType::Sample: {
        SampledDimension sd = dim.asSampledDimension();
        unit = sd.unit().value_or("none");
        break;
    }
    case DimensionType::Range: {
        RangeDimension rd = dim.asRangeDimension();
        unit = rd.unit().value_or("none");
        break;
    }
    default:
       throw std::invalid_argument("Unkown DimensionType");
    }
    return unit;
}


void getOffsetAndCount(const Tag &tag, const DataArray &array, NDSize &offset, NDSize &count) {
    vector<double> position = tag.position();
    vector<double> extent = tag.extent();
    vector<string> units = tag.units();
    vector<Dimension> dimensions = array.dimensions();
    size_t dim_count = dimensions.size();
    NDSize shape = array.dataExtent();

    if (extent.size() > 0 && (extent.size() != position.size())) {
        throw IncompatibleDimensions("Size of position and extent do not match!",
                                     "nix::util::getOffsetAndCount");
    }
    vector<pair<double, double>> max_extents;
    if (position.size() < dim_count) {
        max_extents = maximumExtents(array);
    }
    if (extent.size() == 0) {
        extent.resize(position.size(), 0.0);
    }
    while (position.size() > dim_count) {
        position.pop_back();
        extent.pop_back();
    }
    while (position.size() < dim_count) {
        position.push_back(get<0>(max_extents[position.size()]));
        extent.push_back(get<1>(max_extents[extent.size()]));
    }

    if (units.size() == 0) {
        units = std::vector<std::string>(position.size(), "none");
    }
    while (units.size() > position.size()) {
        units.pop_back();
    }
    while (units.size() < position.size()) {
        units.push_back(getDimensionUnit(dimensions[units.size()]));
    }

    NDSize temp_offset(position.size());
    NDSize temp_count(position.size(), 1);
    for (size_t i = 0; i < position.size(); ++i) {
        vector<pair<ndsize_t, ndsize_t>> indices = positionToIndex({position[i]},
                                                                   {position[i] + extent[i]},
                                                                   {units[i]}, dimensions[i]);
        temp_offset[i] = indices[0].first;
        ndsize_t c = indices[0].second - indices[0].first;
        temp_count[i] += c;
    }
    offset = temp_offset;
    count = temp_count;
}


void getOffsetAndCount(const MultiTag &tag, const DataArray &array, const vector<ndsize_t> &indices,
                       vector<NDSize> &offsets, vector<NDSize> &counts) {
    DataArray positions = tag.positions();
    DataArray extents = tag.extents();
    NDSize position_size, extent_size;
    ndsize_t dimension_count = array.dimensionCount();
    vector<Dimension> dimensions = array.dimensions();
    vector<string> units = tag.units();

    while (units.size() < dimension_count) {
        units.push_back("none");
    }
    vector<pair<double, double>> max_extents;
    if (position_size.size() < dimension_count) {
        max_extents = maximumExtents(array);
    }
    if (positions) {
        position_size = positions.dataExtent();
    }
    if (extents) {
        extent_size = extents.dataExtent();
    }
    ndsize_t max_index = *max_element(indices.begin(), indices.end());
    if (max_index >= positions.dataExtent()[0] || (extents && max_index >= extents.dataExtent()[0])) {
        throw OutOfBounds("Index out of bounds of positions or extents!", 0);
    }

    size_t dimcount_sizet = check::fits_in_size_t(dimension_count, "getOffsetAndCount() failed; dimension count > size_t.");

    NDSize temp_offset(positions.dataExtent().size(), static_cast<NDSize::value_type>(0));
    NDSize temp_count(positions.dataExtent().size(), static_cast<NDSize::value_type>(1));

    int dim_index = dimension_count > 1 ? 1 : 0;
    ndsize_t count = dimension_count > 1 ? position_size[dim_index] : 1;
    temp_count[dim_index] = static_cast<NDSize::value_type>(count);

    vector<vector<double>> start_positions(dimension_count);
    vector<vector<double>> end_positions(dimension_count);
    vector<double> offset, extent;
    for (size_t idx = 0; idx < indices.size(); ++idx) {
        temp_offset[0] = indices[idx];
        positions.getData(offset, temp_count, temp_offset);
        if (extents) {
            extents.getData(extent, temp_count, temp_offset);
        } else {
            extent.resize(offset.size(), 0.0);
        }
        // add pos/extents if missing
        while (offset.size() < dimensions.size()) {
            offset.push_back(get<0>(max_extents[offset.size()]));
            extent.push_back(get<1>(max_extents[extent.size()]));
        }
        // throw away info, if not needed
        while (offset.size() > dimensions.size()) {
            offset.pop_back();
            extent.pop_back();
        }

        for (size_t dim_index = 0; dim_index < dimensions.size(); ++dim_index) {
            if (idx == 0) {
                start_positions[dim_index] = vector<double>(indices.size());
                end_positions[dim_index] = vector<double>(indices.size());
            }
            start_positions[dim_index][idx] = offset[dim_index];
            end_positions[dim_index][idx] = offset[dim_index] + extent[dim_index];
        }
    }

    vector<vector<pair<ndsize_t, ndsize_t>>> data_indices;
    for (size_t dim_index = 0; dim_index < dimensions.size(); ++dim_index) {
        vector<string> temp_units(start_positions.size(), units[dim_index]);
        data_indices.push_back(positionToIndex(start_positions[dim_index], end_positions[dim_index],
                                               temp_units, dimensions[dim_index]));
    }

    for (size_t i = 0; i < indices.size(); ++i) {
        NDSize data_offset(dimcount_sizet, 0);
        NDSize data_count(dimcount_sizet, 1);
        for (size_t dim_index =0; dim_index < dimensions.size(); ++dim_index) {
            data_offset[dim_index] = data_indices[dim_index][i].first;
            ndsize_t count =  data_indices[dim_index][i].second - data_indices[dim_index][i].first;
            data_count[dim_index] += count;
        }
        offsets.push_back(data_offset);
        counts.push_back(data_count);
    }
}

void getOffsetAndCount(const MultiTag &tag, const DataArray &array, ndsize_t index, NDSize &offsets, NDSize &counts) {
    vector<NDSize> temp_offsets, temp_counts;
    getOffsetAndCount(tag, array, {index}, temp_offsets, temp_counts);
    offsets = temp_offsets[0];
    counts = temp_counts[0];
 }


bool positionInData(const DataArray &data, const NDSize &position) {
    NDSize data_size = data.dataExtent();
    bool valid = true;

    if (!(data_size.size() == position.size())) {
        return false;
    }
    for (size_t i = 0; i < data_size.size(); ++i) {
        valid &= position[i] < data_size[i];
    }
    return valid;
}


bool positionAndExtentInData(const DataArray &data, const NDSize &position, const NDSize &count) {
    NDSize pos = position + count;
    pos -= 1;
    return positionInData(data, pos);
}


void fillPositionsExtentsAndUnits(const DataArray &array,
                                  std::vector<double> &starts,
                                  std::vector<double> &ends,
                                  std::vector<std::string> &units) {
    std::vector<nix::Dimension> dims = array.dimensions();
    NDSize shape = array.dataExtent();
    std::string double_fail_msg("dataAccess::fillPositionsExtents: shape cannot be cast to double without loss of resolution. Please open an issue on github!");
    for (size_t i = 0; i < dims.size(); ++i) {
        Dimension dim = dims[i];
        DimensionType dt = dim.dimensionType();
        if (i >= units.size()) {
            units.push_back(getDimensionUnit(dim));
        }
        if (dt == DimensionType::Sample) {
            SampledDimension sd = dim.asSampledDimension();
            if (i >= starts.size()) {
                starts.push_back(sd.offset() ? *sd.offset() : 0.0);
            }
            if (i >= ends.size()) {
                ends.push_back(sd[shape[i]-1]);
            }
        } else if (dt == DimensionType::Range) {
            RangeDimension rd = dim.asRangeDimension();
            if (i >= starts.size()) {
                starts.push_back(rd.axis(1, 0)[0]);
            }
            if (i >= ends.size()) {
                ends.push_back(rd.axis(1, shape[i]-1)[0]);
            }
        } else if (dt == DimensionType::Set) {
            SetDimension sd = dim.asSetDimension();
            if (i >= starts.size()) {
                starts.push_back(0.0);
            }
            if (i >= ends.size()) {
                double end = check::converts_to_double(shape[i] - 1, double_fail_msg);
                ends.push_back(end);
            }
        } else if (dt == DimensionType::DataFrame) {
            DataFrameDimension dfdim = dim.asDataFrameDimension();
            if (i >= starts.size()) {
                starts.push_back(0.0);
            }
            if (i >= ends.size()) {
                double end = check::converts_to_double(shape[i] - 1, double_fail_msg);
                ends.push_back(end);
            }
        }
    }
}


DataView dataSlice(const DataArray &array,
                   const std::vector<double> &start,
                   const std::vector<double> &end,
                   const std::vector<std::string> &units) {
    std::vector<double> my_start(start);
    std::vector<double> my_end(end);
    std::vector<std::string> my_units(units);

    if (array == nix::none) {
        throw UninitializedEntity();
    }
    ndsize_t dim_count = array.dimensionCount();
    if (start.size() > dim_count || end.size() > dim_count || units.size() > dim_count) {
        throw std::invalid_argument("More start/end/unit entries given than number of dimensions in the data!");
    }
    if (start.size() < dim_count || end.size() < dim_count || units.size() < dim_count) {
        fillPositionsExtentsAndUnits(array, my_start, my_end, my_units);
    }

    NDSize count(my_start.size(), 1), offset(my_start.size(), 0);
    for (size_t i = 0; i < my_start.size(); i++) {
        Dimension dim = array.getDimension(i+1);
        if (my_start[i] > my_end[i]) {
            throw std::invalid_argument("Start position must not be larger than end position.");
        }
        std::vector<std::pair<ndsize_t, ndsize_t>> indices = positionToIndex({my_start[i]},
                                                                             {my_end[i]},
                                                                             {my_units[i]},
                                                                             dim);
        offset[i] = indices[0].first;
        count[i] += indices[0].second - indices[0].first;
    }
    if (!positionAndExtentInData(array, offset, count)) {
        throw OutOfBounds("Selected data slice is out of the extent of the DataArray!", 0);
    }
    DataView slice = DataView(array, count, offset);
    return slice;
}


vector<DataView> retrieveData(const MultiTag &tag, vector<ndsize_t> &position_indices, const DataArray &array) {
    return taggedData(tag, position_indices, array);
}


vector<DataView> retrieveData(const MultiTag &tag, vector<ndsize_t> &position_indices, ndsize_t reference_index) {
    return taggedData(tag, position_indices, reference_index);
}


DataView retrieveData(const MultiTag &tag, ndsize_t position_index, const DataArray &array) {
    vector<ndsize_t> indices(1, position_index);
    return taggedData(tag, indices, array)[0];
}


DataView retrieveData(const MultiTag &tag, ndsize_t position_index, ndsize_t reference_index) {
    vector<ndsize_t> indices(1, position_index);
    return taggedData(tag, indices, reference_index)[0];
}


vector<DataView> taggedData(const MultiTag &tag,
                            vector<ndsize_t> &position_indices,
                            ndsize_t reference_index) {
    vector<DataArray> refs = tag.references();
    size_t ref_idx = check::fits_in_size_t(reference_index, "taggedData() failed; reference_index > size_t.");

    if (reference_index >= tag.referenceCount()) {
        throw OutOfBounds("Reference index out of bounds.", 0);
    }
    return taggedData(tag, position_indices, refs[ref_idx]);
}


DataView taggedData(const MultiTag &tag,
                    ndsize_t position_index,
                    ndsize_t reference_index) {
    std::vector<ndsize_t> position_indices(1, position_index);
    return taggedData(tag, position_indices, reference_index)[0];
}


vector<DataView> taggedData(const MultiTag &tag,
                            vector<ndsize_t> &position_indices,
                            const DataArray &array) {
    vector<NDSize> counts, offsets;
    vector<DataView> views;

    if (position_indices.size() < 1) {
        size_t pos_count = check::fits_in_size_t(tag.positions().dataExtent()[0],
                                                 "Number of positions > size_t.");
        position_indices.resize(pos_count);
        std::iota(position_indices.begin(), position_indices.end(), 0);
    }

    getOffsetAndCount(tag, array, position_indices, offsets, counts);

    for (size_t i = 0; i < offsets.size(); ++i) {
        if (!positionAndExtentInData(array, offsets[i], counts[i])) {
            throw OutOfBounds("References data slice out of the extent of the DataArray!", 0);
        }
        DataView io = DataView(array, counts[i], offsets[i]);
        views.push_back(io);
    }
    return views;
}


DataView retrieveData(const Tag &tag, ndsize_t reference_index) {
    return taggedData(tag, reference_index);
}


DataView retrieveData(const Tag &tag, const DataArray &array) {
    return taggedData(tag, array);
}


DataView taggedData(const Tag &tag, ndsize_t reference_index) {
    vector<DataArray> refs = tag.references();
    size_t ref_idx = check::fits_in_size_t(reference_index, "taggedData() failed; reference_index > size_t.");

    if (refs.size() == 0) {
        throw OutOfBounds("There are no references in this tag!", 0);
    }
    if (!(ref_idx < tag.referenceCount())) {
        throw OutOfBounds("Reference index out of bounds.", 0);
    }
    return taggedData(tag, refs[ref_idx]);
}


DataView taggedData(const Tag &tag, const DataArray &array) {
    vector<double> positions = tag.position();
    vector<double> extents = tag.extent();

    NDSize offset, count;
    getOffsetAndCount(tag, array, offset, count);
    if (!positionAndExtentInData(array, offset, count)) {
        throw OutOfBounds("Referenced data slice out of the extent of the DataArray!", 0);
    }
    DataView io = DataView(array, count, offset);
    return io;
}


DataView featureData(const Tag &tag, const Feature &feature) {
    DataArray data = feature.data();
    if (data == none) {
        throw UninitializedEntity();
    }
    if (feature.linkType() == LinkType::Tagged) {
        return taggedData(tag, data);
    }
    // for untagged and indexed return the full data
    NDSize offset(data.dataExtent().size(), 0);
    DataView io = DataView(data, data.dataExtent(), offset);
    return io;
}


DataView retrieveFeatureData(const Tag &tag, const Feature &feature) {
    return featureData(tag, feature);
}


DataView featureData(const Tag &tag, ndsize_t feature_index) {
    if (tag.featureCount() == 0) {
        throw OutOfBounds("There are no features associated with this tag!", 0);
    }
    if (feature_index > tag.featureCount()) {
        throw OutOfBounds("Feature index out of bounds.", 0);
    }
    Feature feat = tag.getFeature(feature_index);
    return featureData(tag, feat);
}


DataView retrieveFeatureData(const Tag &tag, ndsize_t feature_index) {
    return featureData(tag, feature_index);
}


DataView featureData(const MultiTag &tag, ndsize_t position_index, ndsize_t feature_index) {
    size_t feat_idx = check::fits_in_size_t(feature_index, "retrieveFeatureData() failed; feaure_index > size_t.");
    if (feat_idx >= tag.featureCount()) {
        throw OutOfBounds("Feature index out of bounds.", 0);
    }

    Feature feat = tag.getFeature(feat_idx);
    std::vector<ndsize_t> indices(1, position_index);
    return featureData(tag, indices, feat)[0];
}


DataView retrieveFeatureData(const MultiTag &tag, ndsize_t position_index, ndsize_t feature_index) {
    return featureData(tag, position_index, feature_index);
}


DataView featureData(const MultiTag &tag, ndsize_t position_index, const Feature &feature) {
    std::vector<ndsize_t> indices(1, position_index);
    std::vector<DataView> views = featureData(tag, indices, feature);
    return views[0];
}


DataView retrieveFeatureData(const MultiTag &tag, ndsize_t position_index, const Feature &feature) {
    return featureData(tag, position_index, feature);
}


std::vector<DataView> featureData(const MultiTag &tag,
                                  std::vector<ndsize_t> position_indices,
                                  ndsize_t feature_index) {
    size_t feat_idx = check::fits_in_size_t(feature_index,
                                            "featureData() failed; feaure_index > size_t.");
    if (feat_idx >= tag.featureCount()) {
        throw OutOfBounds("Feature index out of bounds.", 0);
    }
    Feature feat = tag.getFeature(feat_idx);
    return featureData(tag, position_indices, feat);
}


std::vector<DataView> retrieveFeatureData(const MultiTag &tag,
                                          std::vector<ndsize_t> position_indices,
                                          ndsize_t feature_index) {
     return featureData(tag, position_indices, feature_index);
}


std::vector<DataView> featureData(const MultiTag &tag,
                                  std::vector<ndsize_t> position_indices,
                                  const Feature &feature) {
    std::vector<DataView> views;
    DataArray data = feature.data();
    if (data == nix::none) {
        throw UninitializedEntity();
    }
    if (position_indices.size() < 1) {
        size_t pos_count = check::fits_in_size_t(tag.positions().dataExtent()[0],
                                                 "Number of positions > size_t.");
        position_indices.resize(pos_count);
        std::iota(position_indices.begin(), position_indices.end(), 0);
    }
    if (feature.linkType() == LinkType::Tagged) {
        views = taggedData(tag, position_indices, data);
        return views;
    }

    ndsize_t max_index = *max_element(position_indices.begin(), position_indices.end());
    if (max_index >= tag.positions().dataExtent()[0]) {
        throw OutOfBounds("Index out of bounds of positions!", 0);
    }

    if (feature.linkType() == LinkType::Indexed) {
        // For now we return slices across the first dim.
        for (size_t idx = 0; idx < position_indices.size(); ++idx) {
            NDSize offset(data.dataExtent().size(), 0);
            offset[0] = position_indices[idx];
            NDSize count(data.dataExtent());
            count[0] = 1;
            if (!positionAndExtentInData(data, offset, count)) {
                throw OutOfBounds("Requested data slice out of the extent of the Feature!",
                                  position_indices[idx]);
            }
            DataView io = DataView(data, count, offset);
            views.push_back(io);
        }
    } else {
        for (size_t idx = 0; idx < position_indices.size(); ++idx){
            // In the untagged case all data is returned for each position
            NDSize offset(data.dataExtent().size(), 0);
            DataView io = DataView(data, data.dataExtent(), offset);
            views.push_back(io);
        }
    }
    return views;
}


std::vector<DataView> retrieveFeatureData(const MultiTag &tag,
                                          std::vector<ndsize_t> position_indices,
                                          const Feature &feature) {
    return featureData(tag, position_indices, feature);
}

} // namespace util
} // namespace nix
