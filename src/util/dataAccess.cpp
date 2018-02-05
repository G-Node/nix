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

#include <boost/optional.hpp>

using namespace std;

namespace nix {
namespace util {


void scalePositions(const vector<double> &starts, const vector<double> &ends,
                    const vector<string> &units, const string & dim_unit,
                    vector<double> &scaled_starts, vector<double> &scaled_ends) {
    ndsize_t count = min(starts.size(), ends.size());
    if (scaled_starts.size() != count)
        scaled_starts.resize(count);
    if (scaled_ends.size() != count)
        scaled_ends.resize(count);
    double scaling= 1.0;
    for (ndsize_t i = 0; i < count; ++i) {
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


ndsize_t positionToIndex(double position, const string &unit, const Dimension &dimension) {
    ndsize_t pos;
    if (dimension.dimensionType() == DimensionType::Sample) {
        SampledDimension dim;
        dim = dimension;
        pos = positionToIndex(position, unit, dim);
    } else if (dimension.dimensionType() == DimensionType::Set) {
        SetDimension dim;
        dim = dimension;
        pos = positionToIndex(position, unit, dim);
    } else {
        RangeDimension dim;
        dim = dimension;
        pos = positionToIndex(position, unit, dim);
    }

    return pos;
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
        // TODO check here for the content
        // convert unit and the go looking for it, see range dimension
        throw IncompatibleDimensions("Cannot apply a position with unit to a SetDimension",
                                     "nix::util::positionToIndex");
    }
    index = static_cast<ndsize_t>(round(position));
    if (dimension.labels().size() > 0 && index > dimension.labels().size()) {
        throw OutOfBounds("Position is out of bounds in setDimension.", static_cast<int>(position));
    }
    return index;
}


vector<pair<ndsize_t, ndsize_t>> positionToIndex(const vector<double> &start_positions,
                                                 const vector<double> &end_positions,
                                                 const vector<string> &units,
                                                 const SampledDimension &dimension) {
    size_t count = min(start_positions.size(), end_positions.size());
    vector<double> scaled_start(count);
    vector<double> scaled_end(count);
    string dim_unit = dimension.unit() ? *dimension.unit() : "none";
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
        indices.emplace_back(start_positions[i], end_positions[i]);
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
    string dim_unit = dimension.unit() ? *dimension.unit() : "none";
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


void getOffsetAndCount(const Tag &tag, const DataArray &array, NDSize &offset, NDSize &count) {
    vector<double> position = tag.position();
    vector<double> extent = tag.extent();
    vector<string> units = tag.units();
    NDSize temp_offset(position.size());
    NDSize temp_count(position.size(), 1);
    vector<Dimension> dimensions = array.dimensions();

    if (array.dimensionCount() != position.size() || (extent.size() > 0 && extent.size() != array.dimensionCount())) {
        throw runtime_error("Dimensionality of position or extent vector does not match dimensionality of data!");
    }
    if (units.size() < position.size())
        units = vector<string>(position.size(), "none");
    if (extent.size() < position.size())
        extent = vector<double>(position.size(), 0.0);
    for (size_t i = 0; i < position.size(); ++i) {
        vector<pair<ndsize_t, ndsize_t>> indices = positionToIndex({position[i]},
                                                                   {position[i] + extent[i]},
                                                                   {units[i]}, dimensions[i]);
        temp_offset[i] = indices[0].first;
        ndsize_t count = indices[0].second - indices[0].first;
        temp_count[i] += count;
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
    vector<string> units = tag.units();
    while (units.size() < dimension_count) {
        units.push_back("none");
    }
    if (positions) {
        position_size = positions.dataExtent();
    }
    if (extents) {
        extent_size = extents.dataExtent();
    }
    if (position_size.size() == 1 && dimension_count != 1) {
        throw IncompatibleDimensions("Number of dimensions in positions does not match dimensionality of data",
                                          "util::getOffsetAndCount");
    }
    if (position_size.size() > 1 && position_size[1] > dimension_count) {
        throw IncompatibleDimensions("Number of dimensions in positions does not match dimensionality of data",
                                          "util::getOffsetAndCount");
    }
    if (extents && extent_size != position_size) {
        throw IncompatibleDimensions("Number of dimensions in extents does not match dimensionality of data",
                                          "util::getOffsetAndCount");
    }
    ndsize_t max_index =*max_element(indices.begin(), indices.end());
    if (max_index >= positions.dataExtent()[0] || (extents && max_index >= extents.dataExtent()[0])) {
        throw OutOfBounds("Index out of bounds of positions or extents!", 0);
    }

    size_t dimcount_sizet = check::fits_in_size_t(dimension_count, "getOffsetAndCount() failed; dimension count > size_t.");
    NDSize temp_offset(dimcount_sizet, static_cast<NDSize::value_type>(0));
    NDSize temp_count(dimcount_sizet, static_cast<NDSize::value_type>(1));

    temp_count[1] = static_cast<NDSize::value_type>(dimension_count);
    vector<Dimension> dimensions = array.dimensions();
    vector<vector<double>> start_positions(dimensions.size());
    vector<vector<double>> end_positions(dimensions.size());

    for (ndsize_t idx = 0; idx < indices.size(); ++idx) {
        temp_offset[0] = indices[idx];
        vector<double> offset, extent;
        positions.getData(offset, temp_count, temp_offset);
        if (extents) {
            extents.getData(extent, temp_count, temp_offset);
        }
        for (ndsize_t dim_index = 0; dim_index < dimensions.size(); ++dim_index) {
            if (idx == 0) {
                start_positions[dim_index] = vector<double>(indices.size());
                end_positions[dim_index] = vector<double>(indices.size());
            }
            start_positions[dim_index][idx] = offset[dim_index];
            end_positions[dim_index][idx] = offset[dim_index] + extent[dim_index];
        }
    }

    vector<vector<pair<ndsize_t, ndsize_t>>> data_indices;
    for (ndsize_t dim_index = 0; dim_index < dimensions.size(); ++dim_index) {
        vector<string> temp_units(start_positions.size(), units[dim_index]);
        data_indices.push_back(positionToIndex(start_positions[dim_index], end_positions[dim_index],
                                               temp_units, dimensions[dim_index]));
    }

    for (ndsize_t i = 0; i < indices.size(); ++i) {
        NDSize data_offset(dimcount_sizet, static_cast<ndsize_t>(0));
        NDSize data_count(dimcount_sizet, static_cast<ndsize_t>(1));
        for (ndsize_t dim_index =0; dim_index < dimensions.size(); ++dim_index) {
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


DataView retrieveData(const MultiTag &tag, ndsize_t position_index, size_t reference_index) {
    vector<ndsize_t> indices(1, position_index);
    return retrieveData(tag, indices, reference_index)[0];
}


DataView retrieveData(const MultiTag &tag, ndsize_t position_index, const DataArray &array) {
    vector<ndsize_t> indices(1, position_index);
    return retrieveData(tag, indices, array)[0];
}


vector<DataView> retrieveData(const MultiTag &tag, const vector<ndsize_t> &position_indices,
                              size_t reference_index) {
    vector<DataArray> refs = tag.references();
    if (reference_index >= tag.referenceCount()) {
        throw OutOfBounds("Reference index out of bounds.", 0);
    }
    return retrieveData(tag, position_indices, refs[reference_index]);
}


vector<DataView> retrieveData(const MultiTag &tag, const vector<ndsize_t> &position_indices,
                              const DataArray &array) {
    vector<NDSize> counts, offsets;
    vector<DataView> views;

    getOffsetAndCount(tag, array, position_indices, offsets, counts);
    for (ndsize_t i = 0; i < offsets.size(); ++i) {
        if (!positionAndExtentInData(array, offsets[i], counts[i])) {
            throw OutOfBounds("References data slice out of the extent of the DataArray!", 0);
        }
        DataView io = DataView(array, counts[i], offsets[i]);
        views.push_back(io);
    }
    return views;
}


DataView retrieveData(const Tag &tag, size_t reference_index) {
    vector<DataArray> refs = tag.references();
    if (refs.size() == 0) {
        throw OutOfBounds("There are no references in this tag!", 0);
    }
    if (!(reference_index < tag.referenceCount())) {
        throw OutOfBounds("Reference index out of bounds.", 0);
    }
    return retrieveData(tag, refs[reference_index]);
}


DataView retrieveData(const Tag &tag, const DataArray &array) {
    vector<double> positions = tag.position();
    vector<double> extents = tag.extent();
    ndsize_t dimension_count = array.dimensionCount();
    if (positions.size() != dimension_count || (extents.size() > 0 && extents.size() != dimension_count)) {
        throw IncompatibleDimensions("Number of dimensions in position or extent do not match dimensionality of data", "util::retrieveData");
    }

    NDSize offset, count;
    getOffsetAndCount(tag, array, offset, count);
    if (!positionAndExtentInData(array, offset, count)) {
        throw OutOfBounds("Referenced data slice out of the extent of the DataArray!", 0);
    }
    DataView io = DataView(array, count, offset);
    return io;
}


DataView retrieveFeatureData(const Tag &tag, const Feature &feature) {
    DataArray data = feature.data();
    if (data == none) {
        throw UninitializedEntity();
        //return NDArray(nix::DataType::Float,{0});
    }
    if (feature.linkType() == LinkType::Tagged) {
        return retrieveData(tag, data);
    }
    // for untagged and indexed return the full data
    NDSize offset(data.dataExtent().size(), 0);
    DataView io = DataView(data, data.dataExtent(), offset);
    return io;
}


DataView retrieveFeatureData(const Tag &tag, size_t feature_index) {
    if (tag.featureCount() == 0) {
        throw OutOfBounds("There are no features associated with this tag!", 0);
    }
    if (feature_index > tag.featureCount()) {
        throw OutOfBounds("Feature index out of bounds.", 0);
    }
    Feature feat = tag.getFeature(feature_index);
    return retrieveFeatureData(tag, feat);
}


DataView retrieveFeatureData(const MultiTag &tag, ndsize_t position_index, size_t feature_index) {
    if (tag.featureCount() == 0) {
       throw OutOfBounds("There are no features associated with this tag!", 0);
    }
    if (feature_index >= tag.featureCount()) {
        throw OutOfBounds("Feature index out of bounds.", 0);
    }
    Feature feat = tag.getFeature(feature_index);
    return retrieveFeatureData(tag, position_index, feat);
}

DataView retrieveFeatureData(const MultiTag &tag, ndsize_t position_index, const Feature &feature) {
    DataArray data = feature.data();
    if (data == nix::none) {
        throw UninitializedEntity();
        //return NDArray(nix::DataType::Float,{0});
    }
    if (feature.linkType() == LinkType::Tagged) {
        vector<ndsize_t> indices(1, position_index);
        return retrieveData(tag, indices, data)[0];
    } else if (feature.linkType() == LinkType::Indexed) {
        //FIXME does the feature data to have a setdimension in the first dimension for the indexed case?
        //For now it will just be a slice across the first dim.
        if (position_index > data.dataExtent()[0]){
            throw OutOfBounds("Position is larger than the data stored in the feature.", 0);
        }
        NDSize offset(data.dataExtent().size(), 0);
        offset[0] = position_index;
        NDSize count(data.dataExtent());
        count[0] = 1;

        if (!positionAndExtentInData(data, offset, count)) {
            throw OutOfBounds("Requested data slice out of the extent of the Feature!", 0);
        }
        DataView io = DataView(data, count, offset);
        return io;
    }
    // FIXME is this expected behavior? In the untagged case all data is returned
    NDSize offset(data.dataExtent().size(), 0);
    DataView io = DataView(data, data.dataExtent(), offset);
    return io;
}


} // namespace util
} // namespace nix
