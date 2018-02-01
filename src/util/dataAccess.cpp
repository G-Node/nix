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


ndsize_t positionToIndex(double position, const string &unit, const Dimension &dimension) {
    ndsize_t pos;
    if (dimension.dimensionType() == nix::DimensionType::Sample) {
        SampledDimension dim;
        dim = dimension;
        pos = positionToIndex(position, unit, dim);
    } else if (dimension.dimensionType() == nix::DimensionType::Set) {
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


vector<pair<ndsize_t, ndsize_t>> positionToIndex(const vector<double> &start_positions, const vector<double> end_positions,
                                                 const vector<string> &units, const Dimension &dimension) {
    vector<pair<ndsize_t, ndsize_t>> indices;
    if (dimension.dimensionType() == nix::DimensionType::Sample) {
        SampledDimension dim;
        dim = dimension;
        indices = positionToIndex(start_positions, end_positions, units, dim);
    } else if (dimension.dimensionType() == nix::DimensionType::Set) {
        SetDimension dim;
        dim = dimension;
        indices = positionToIndex(start_positions, end_positions, units, dim);
    } else {
        RangeDimension dim;
        dim = dimension;
        indices = positionToIndex(start_position, end_positions, units, dim);
    }
    return indices;
}


ndsize_t positionToIndex(double position, const string &unit, const SampledDimension &dimension) {
    ndsize_t index;
    boost::optional<string> dim_unit = dimension.unit();
    double scaling = 1.0;
    if (!dim_unit && unit != "none") {
        throw nix::IncompatibleDimensions("Units of position and SampledDimension must both be given!", "nix::util::positionToIndex");
    }
    if (dim_unit && unit != "none") {
        try {
            scaling = util::getSIScaling(unit, *dim_unit);
        } catch (...) {
            throw nix::IncompatibleDimensions("Cannot apply a position with unit to a SetDimension", "nix::util::positionToIndex");
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
        throw nix::IncompatibleDimensions("Cannot apply a position with unit to a SetDimension", "nix::util::positionToIndex");
    }
    index = static_cast<ndsize_t>(round(position));
    if (dimension.labels().size() > 0 && index > dimension.labels().size()) {
        throw nix::OutOfBounds("Position is out of bounds in setDimension.", static_cast<int>(position));
    }
    return index;
}


vector<pair<ndsize_t, ndsize_t>> positionToIndex(const vector<double> &start_positions, const vector<double> &end_positions,
                                                 const vector<string> &units, const SampledDimension &dimension) {
    vector<pair<ndsize_t, ndsize_t>> indices;
    return indices;

}


vector<pair<ndsize_t, ndsize_t>> positionToIndex(const vector<double> &start_positions, const vector<double> &end_positions,
                                                 const vector<string> &units, const SetDimension &dimension) {
    vector<pair<ndsize_t, ndsize_t>> indices;
    return indices;
}


vector<pair<ndsize_t, ndsize_t>> positionToIndex(const vector<double> &start_positions, const vector<double> &end_positions,
                                                 const vector<string> &units, const RangeDimension &dimension) {
    // need to take the units into account!!!
    //TODO
    vector<pair<ndsize_t, ndsize_t>> indices;

    return dimension.indexOf(start_positions, end_positions, units);
}


ndsize_t positionToIndex(double position, const string &unit, const RangeDimension &dimension) {
    boost::optional<string> dim_unit = dimension.unit();
    double scaling = 1.0;

    if (dim_unit && unit != "none") {
        try {
            scaling = util::getSIScaling(unit, *dim_unit);
        } catch (...) {
            throw nix::IncompatibleDimensions("Provided units are not scalable!", "nix::util::positionToIndex");
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

    if (array.dimensionCount() != position.size() || (extent.size() > 0 && extent.size() != array.dimensionCount())) {
        throw std::runtime_error("Dimensionality of position or extent vector does not match dimensionality of data!");
    }
    for (size_t i = 0; i < position.size(); ++i) {
        Dimension dim = array.getDimension(i+1);
        temp_offset[i] = positionToIndex(position[i], i >= units.size() ? "none" : units[i], dim);
        if (i < extent.size()) {
            ndsize_t c = positionToIndex(position[i] + extent[i], i >= units.size() ? "none" : units[i], dim) - temp_offset[i];
            temp_count[i] = (c > 1) ? c : 1;
        }
    }
    offset = temp_offset;
    count = temp_count;
}

void getOffsetAndCount(const MultiTag &tag, const DataArray &array, const vector<ndsize_t> &indices,
                       vector<NDSize> &counts, vector<NDSize> &offsets) {
    DataArray positions = tag.positions();
    DataArray extents = tag.extents();
    NDSize position_size, extent_size;
    ndsize_t dimension_count = array.dimensionCount();
    vector<string> units = tag.units();

    if (positions) {
        position_size = positions.dataExtent();
    }
    if (extents) {
        extent_size = extents.dataExtent();
    }

    if (position_size.size() == 1 && dimension_count != 1) {
        throw nix::IncompatibleDimensions("Number of dimensions in positions does not match dimensionality of data",
                                          "util::getOffsetAndCount");
    }
    if (position_size.size() > 1 && position_size[1] > dimension_count) {
        throw nix::IncompatibleDimensions("Number of dimensions in positions does not match dimensionality of data",
                                          "util::getOffsetAndCount");
    }
    if (extents && extents != positions) {
        throw nix::IncompatibleDimensions("Number of dimensions in extents does not match dimensionality of data",
                                          "util::getOffsetAndCount");
    }
    ndsize_t max_index =*std::max_element(indices.begin(), indices.end());
    if (max_index >= positions[0] || (extents && max_index >= extents[0])) {
        throw nix::OutOfBounds("Index out of bounds of positions or extents!", 0);
    }

    for (ndsize_t index : indices) {
         NDSize temp_offset = NDSize{index, static_cast<NDSize::value_type>(0)};
         NDSize temp_count{static_cast<NDSize::value_type>(1), static_cast<NDSize::value_type>(dimension_count)};
         vector<double> offset;
         positions.getData(offset, temp_count, temp_offset);

         size_t dc_sizet = check::fits_in_size_t(dimension_count, "getOffsetAndCount() failed; dimension count > size_t.");
         NDSize data_offset(dc_sizet, static_cast<ndsize_t>(0));
         NDSize data_count(dc_sizet, static_cast<ndsize_t>(1));

         for (size_t i = 0; i < offset.size(); ++i) {
             Dimension dimension = array.getDimension(i+1);
             string unit = "none";
             if (i <= units.size() && units.size() > 0) {
                 unit = units[i];
             }
             data_offset[i] = positionToIndex(offset[i], unit, dimension);
         }

         if (extents) {
             vector<double> extent;
             extents.getData(extent, temp_count, temp_offset);
             for (size_t i = 0; i < extent.size(); ++i) {
                 Dimension dimension = array.getDimension(i+1);
                 string unit = "none";
                 if (i <= units.size() && units.size() > 0) {
                     unit = units[i];
                 }
            ndsize_t c = positionToIndex(offset[i] + extent[i], unit, dimension) - data_offset[i];
            data_count[i] = (c > 1) ? c : 1;
        }
    }

    offsets = data_offset;
    counts = data_count;

    }
}

void getOffsetAndCount(const MultiTag &tag, const DataArray &array, ndsize_t index, NDSize &offsets, NDSize &counts) {
    DataArray positions = tag.positions();
    DataArray extents = tag.extents();
    NDSize position_size, extent_size;
    ndsize_t dimension_count = array.dimensionCount();

    if (positions) {
        position_size = positions.dataExtent();
    }

    if (extents) {
        extent_size = extents.dataExtent();
    }

    if (!positions || index >= position_size[0]) {
        throw nix::OutOfBounds("Index out of bounds of positions!", 0);
    }

    if (extents && index >= extent_size[0]) {
        throw nix::OutOfBounds("Index out of bounds of positions or extents!", 0);
    }

    if (position_size.size() == 1 && dimension_count != 1) {
        throw nix::IncompatibleDimensions("Number of dimensions in positions does not match dimensionality of data",
                                          "util::getOffsetAndCount");
    }

    if (position_size.size() > 1 && position_size[1] > dimension_count) {
        throw nix::IncompatibleDimensions("Number of dimensions in positions does not match dimensionality of data",
                                          "util::getOffsetAndCount");
    }

    if (extents && extent_size.size() > 1 && extent_size[1] > dimension_count) {
        throw nix::IncompatibleDimensions("Number of dimensions in extents does not match dimensionality of data",
                                          "util::getOffsetAndCount");
    }

    NDSize temp_offset = NDSize{index, static_cast<NDSize::value_type>(0)};
    NDSize temp_count{static_cast<NDSize::value_type>(1), static_cast<NDSize::value_type>(dimension_count)};
    vector<double> offset;
    positions.getData(offset, temp_count, temp_offset);

    size_t dc_sizet = check::fits_in_size_t(dimension_count, "getOffsetAndCount() failed; dimension count > size_t.");
    NDSize data_offset(dc_sizet, static_cast<ndsize_t>(0));
    NDSize data_count(dc_sizet, static_cast<ndsize_t>(1));
    vector<string> units = tag.units();

    for (size_t i = 0; i < offset.size(); ++i) {
        Dimension dimension = array.getDimension(i+1);
        string unit = "none";
        if (i <= units.size() && units.size() > 0) {
            unit = units[i];
        }
        data_offset[i] = positionToIndex(offset[i], unit, dimension);
    }

    if (extents) {
        vector<double> extent;
        extents.getData(extent, temp_count, temp_offset);
        for (size_t i = 0; i < extent.size(); ++i) {
            Dimension dimension = array.getDimension(i+1);
            string unit = "none";
            if (i <= units.size() && units.size() > 0) {
                unit = units[i];
            }
            ndsize_t c = positionToIndex(offset[i] + extent[i], unit, dimension) - data_offset[i];
            data_count[i] = (c > 1) ? c : 1;
        }
    }

    offsets = data_offset;
    counts = data_count;
}


bool positionInData(const DataArray &data, const NDSize &position) {
    NDSize data_size = data.dataExtent();
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


DataView retrieveData(const MultiTag &tag, ndsize_t position_index, size_t reference_index) {
    vector<ndsize_t> indices(1, position_index);
    return retrieveData(tag, indices, reference_index)[0];
}


DataView retrieveData(const MultiTag &tag, ndsize_t position_index, const DataArray &array) {
    vector<ndsize_t> indices(1, position_index);
    return retrieveData(tag, indices, array)[0];
}


vector<DataView> retrieveData(const MultiTag &tag, const vector<ndsize_t> &position_indices, size_t reference_index) {
    vector<DataArray> refs = tag.references();

    if (reference_index >= tag.referenceCount()) {
        throw nix::OutOfBounds("Reference index out of bounds.", 0);
    }

    return retrieveData(tag, position_indices, refs[reference_index]);
}


vector<DataView> retrieveData(const MultiTag &tag, const vector<ndsize_t> &position_indices, const DataArray &array) {
    vector<NDSize> counts, offsets;
    vector<DataView> views;

    getOffsetAndCount(tag, array, position_indices, offsets, counts);
    for (ndsize_t i = 0; i < offsets.size(); i++) {
        if (!positionAndExtentInData(array, offsets[i], counts[i])) {
            throw nix::OutOfBounds("References data slice out of the extent of the DataArray!", 0);
        }
        DataView io = DataView(array, count[i], offset[i]);
        views.push_back(io);
    }
    return views;
}


DataView retrieveData(const Tag &tag, size_t reference_index) {
    vector<DataArray> refs = tag.references();
    if (refs.size() == 0) {
        throw nix::OutOfBounds("There are no references in this tag!", 0);
    }
    if (!(reference_index < tag.referenceCount())) {
        throw nix::OutOfBounds("Reference index out of bounds.", 0);
    }
    return retrieveData(tag, refs[reference_index]);
}


DataView retrieveData(const Tag &tag, const DataArray &array) {
    vector<double> positions = tag.position();
    vector<double> extents = tag.extent();
    ndsize_t dimension_count = array.dimensionCount();
    if (positions.size() != dimension_count || (extents.size() > 0 && extents.size() != dimension_count)) {
        throw nix::IncompatibleDimensions("Number of dimensions in position or extent do not match dimensionality of data", "util::retrieveData");
    }

    NDSize offset, count;
    getOffsetAndCount(tag, array, offset, count);
    if (!positionAndExtentInData(array, offset, count)) {
        throw nix::OutOfBounds("Referenced data slice out of the extent of the DataArray!", 0);
    }
    DataView io = DataView(array, count, offset);
    return io;
}


DataView retrieveFeatureData(const Tag &tag, const Feature &feature) {
    DataArray data = feature.data();
    if (data == nix::none) {
        throw nix::UninitializedEntity();
        //return NDArray(nix::DataType::Float,{0});
    }
    if (feature.linkType() == nix::LinkType::Tagged) {
        NDSize offset, count;
        getOffsetAndCount(tag, data, offset, count);
        if (!positionAndExtentInData(data, offset, count)) {
            throw nix::OutOfBounds("Requested data slice out of the extent of the Feature!", 0);
        }
        DataView io = DataView(data, count, offset);
        return io;
    }
    // for untagged and indexed return the full data
    NDSize offset(data.dataExtent().size(), 0);
    DataView io = DataView(data, data.dataExtent(), offset);
    return io;
}


DataView retrieveFeatureData(const Tag &tag, size_t feature_index) {
    if (tag.featureCount() == 0) {
        throw nix::OutOfBounds("There are no features associated with this tag!", 0);
    }
    if (feature_index > tag.featureCount()) {
        throw nix::OutOfBounds("Feature index out of bounds.", 0);
    }
    Feature feat = tag.getFeature(feature_index);
    return retrieveFeatureData(tag, feat);
}


DataView retrieveFeatureData(const MultiTag &tag, ndsize_t position_index, size_t feature_index) {
    if (tag.featureCount() == 0) {
       throw nix::OutOfBounds("There are no features associated with this tag!", 0);
    }
    if (feature_index >= tag.featureCount()) {
        throw nix::OutOfBounds("Feature index out of bounds.", 0);
    }
    Feature feat = tag.getFeature(feature_index);
    return retrieveFeatureData(tag, position_index, feat);
}

DataView retrieveFeatureData(const MultiTag &tag, ndsize_t position_index, const Feature &feature) {
    DataArray data = feature.data();
    if (data == nix::none) {
        throw nix::UninitializedEntity();
        //return NDArray(nix::DataType::Float,{0});
    }
    if (feature.linkType() == nix::LinkType::Tagged) {
        NDSize offset, count;
        getOffsetAndCount(tag, data, position_index, offset, count);

        if (!positionAndExtentInData(data, offset, count)) {
            throw nix::OutOfBounds("Requested data slice out of the extent of the Feature!", 0);
        }
        DataView io = DataView(data, count, offset);
        return io;
    } else if (feature.linkType() == nix::LinkType::Indexed) {
        //FIXME does the feature data to have a setdimension in the first dimension for the indexed case?
        //For now it will just be a slice across the first dim.
        if (position_index > data.dataExtent()[0]){
            throw nix::OutOfBounds("Position is larger than the data stored in the feature.", 0);
        }
        NDSize offset(data.dataExtent().size(), 0);
        offset[0] = position_index;
        NDSize count(data.dataExtent());
        count[0] = 1;

        if (!positionAndExtentInData(data, offset, count)) {
            throw nix::OutOfBounds("Requested data slice out of the extent of the Feature!", 0);
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
