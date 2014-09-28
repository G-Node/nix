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
#include <algorithm>

#include <boost/optional.hpp>

#include <nix/util/util.hpp>
#include <nix/util/dataAccess.hpp>

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

    if (!dim_unit && unit != "none") {
        throw nix::IncompatibleDimensions("Units of position and SampledDimension must both be given!", "nix::util::positionToIndex");
    }
    if ((dimension.offset() && position < *dimension.offset()) || (!dimension.offset() && position < 0.0)) {
        throw nix::OutOfBounds("Position is out of bounds in SampledDimension.", static_cast<int>(position));
    }
    if (dim_unit && unit != "none") {
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
    if (unit.length() > 0 && unit != "none") {
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

    if (dim_unit && unit != "none") {
        try {
            scaling = util::getSIScaling(unit, *dim_unit);
        } catch (...) {
            throw nix::IncompatibleDimensions("Provided units are not scalable!", "nix::util::positionToIndex");
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
        temp_offset[i] = positionToIndex(position[i], i > units.size() ? "none" : units[i], dim);
        if (i < extent.size()) {
            temp_count[i] = 1 + positionToIndex(position[i] + extent[i], i > units.size() ? "none" : units[i], dim) - temp_offset[i];
        }
    }
    offset = temp_offset;
    count = temp_count;
}


void getOffsetAndCount(const MultiTag &tag, const DataArray &array, size_t index, NDSize &offsets, NDSize &counts) {
    DataArray positions = tag.positions();
    DataArray extents = tag.extents();
    NDSize position_size, extent_size;
    size_t dimension_count = array.dimensionCount();

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
    if (position_size[1] > dimension_count) {
        throw nix::IncompatibleDimensions("Number of dimensions in positions does not match dimensionality of data","util::getOffsetAndCount");
    }
    if (extents && extent_size[1] > dimension_count) {
        throw nix::IncompatibleDimensions("Number of dimensions in extents does not match dimensionality of data","util::getOffsetAndCount");
    }

    NDSize temp_offset = NDSize{static_cast<NDSize::value_type>(index), static_cast<NDSize::value_type>(0)};
    NDSize temp_count{static_cast<NDSize::value_type>(1), static_cast<NDSize::value_type>(dimension_count)};
    NDArray offset(positions.dataType(), temp_count);
    positions.getData(offset, temp_count, temp_offset);

    NDSize data_offset(dimension_count, static_cast<size_t>(0));
    NDSize data_count(dimension_count, static_cast<size_t>(1));
    vector<string> units = tag.units();

    for (size_t i = 0; i < offset.num_elements(); ++i) {
        Dimension dimension = array.getDimension(i+1);
        string unit = "none";
        if (i <= units.size() && units.size() > 0) {
            unit = units[i];
        }
        data_offset[i] = positionToIndex(offset.get<double>(i), unit, dimension);
    }

    if (extents) {
        NDArray extent(extents.dataType(), temp_count);
        extents.getData(extent, temp_count, temp_offset);
        for (size_t i = 0; i < offset.num_elements(); ++i) {
            Dimension dimension = array.getDimension(i+1);
            string unit = "none";
            if (i <= units.size() && units.size() > 0) {
                unit = units[i];
            }
            if (i < extent.num_elements()) {
                data_count[i] = 1 + positionToIndex(offset.get<double>(i) + extent.get<double>(i), unit, dimension) - data_offset[i];
            }
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


NDArray retrieveData(const MultiTag &tag, size_t position_index, size_t reference_index) {
    DataArray positions = tag.positions();
    DataArray extents = tag.extents();
    vector<DataArray> refs = tag.references();

    if (refs.size() == 0) {
        throw nix::OutOfBounds("There are no references in this tag!", 0);
    }
    if (position_index >= positions.dataExtent()[0] ||
        (extents && position_index >= extents.dataExtent()[0])) {
        throw nix::OutOfBounds("Index out of bounds of positions or extents!", 0);
    }
    if (!(reference_index < tag.referenceCount())) {
        throw nix::OutOfBounds("Reference index out of bounds.", 0);
    }

    size_t dimension_count = refs[reference_index].dimensionCount();
    if (positions.dataExtent()[1] > dimension_count ||
        (extents &&extents.dataExtent()[1] > dimension_count)) {
        throw nix::IncompatibleDimensions("Number of dimensions in position or extent do not match dimensionality of data","util::retrieveData");
    }

    NDSize offset, count;
    getOffsetAndCount(tag, refs[reference_index], position_index, offset, count);
    if (!positionAndExtentInData(refs[reference_index], offset, count)) {
        throw nix::OutOfBounds("References data slice out of the extent of the DataArray!", 0);
    }
    NDArray data(refs[reference_index].dataType(), count);
    refs[reference_index].getData(data, count, offset);
    return data;
}


NDArray retrieveData(const Tag &tag, size_t reference_index) {
    vector<double> positions = tag.position();
    vector<double> extents = tag.extent();
    vector<DataArray> refs = tag.references();
    if (refs.size() == 0) {
        throw nix::OutOfBounds("There are no references in this tag!", 0);
    }
    if (!(reference_index < tag.referenceCount())) {
        throw nix::OutOfBounds("Reference index out of bounds.", 0);
    }
    size_t dimension_count = refs[reference_index].dimensionCount();
    if (positions.size() != dimension_count || (extents.size() > 0 && extents.size() != dimension_count)) {
        throw nix::IncompatibleDimensions("Number of dimensions in position or extent do not match dimensionality of data","util::retrieveData");
    }

    NDSize offset, count;
    getOffsetAndCount(tag, refs[reference_index], offset, count);
    if (!positionAndExtentInData(refs[reference_index], offset, count)) {
        throw nix::OutOfBounds("Referenced data slice out of the extent of the DataArray!", 0);
    }
    NDArray data(refs[reference_index].dataType(), count);
    refs[reference_index].getData(data, count, offset);
    return data;
}


NDArray retrieveFeatureData(const Tag &tag, size_t feature_index) {
    if (tag.featureCount() == 0) {
        throw nix::OutOfBounds("There are no features associated with this tag!", 0);
    }
    if (feature_index > tag.featureCount()) {
        throw nix::OutOfBounds("Feature index out of bounds.", 0);
    }
    Feature feat = tag.getFeature(feature_index);
    DataArray data = feat.data();
    if (data == nix::none) {
        return NDArray(nix::DataType::Float,{0});
    }
    if (feat.linkType() == nix::LinkType::Tagged) {
        NDSize offset, count;
        getOffsetAndCount(tag, data, offset, count);
        if (!positionAndExtentInData(data, offset, count)) {
            throw nix::OutOfBounds("Requested data slice out of the extent of the Feature!", 0);
        }
        NDArray feat_data(data.dataType(), count);
        data.getData(feat_data, count, offset);
        return feat_data;
    }
    // for untagged and indexed return the full data
    NDArray feat_data(data.dataType(), data.dataExtent());
    data.getData(feat_data);
    return feat_data;
}


NDArray retrieveFeatureData(const MultiTag &tag, size_t position_index, size_t feature_index) {
    if (tag.featureCount() == 0) {
       throw nix::OutOfBounds("There are no features associated with this tag!", 0);
    }
    if (feature_index > tag.featureCount()) {
        throw nix::OutOfBounds("Feature index out of bounds.", 0);
    }
    Feature feat = tag.getFeature(feature_index);
    DataArray data = feat.data();
    if (data == nix::none) {
        return NDArray(nix::DataType::Float,{0});
    }
    if (feat.linkType() == nix::LinkType::Tagged) {
        NDSize offset, count;
        getOffsetAndCount(tag, data, position_index, offset, count);
        if (!positionAndExtentInData(data, offset, count)) {
            throw nix::OutOfBounds("Requested data slice out of the extent of the Feature!", 0);
        }
        NDArray feat_data(data.dataType(), count);
        data.getData(feat_data, count, offset);
        return feat_data;
    } else if (feat.linkType() == nix::LinkType::Indexed) {
        //FIXME does the feature data to have a setdimension in the first dimension for the indexed case?
        //For now it will just be a slice across the first dim.
        if (position_index > data.dataExtent()[0]){
            throw nix::OutOfBounds("Position is larger than the data stored in the feature.", 0);
        }
        int start = 0;
        NDSize offset(data.dataExtent().size(), start);
        offset[0] = position_index;
        NDSize count(data.dataExtent());
        if (!positionAndExtentInData(data, offset, count)) {
            throw nix::OutOfBounds("Requested data slice out of the extent of the Feature!", 0);
        }
        NDArray feat_data(data.dataType(), count);
        data.getData(feat_data, count, offset);
        return feat_data;
    }
    // FIXME is this expected behavior? In the untagged case all data is returned
    NDArray feat_data(data.dataType(), data.dataExtent());
    data.getData(feat_data);
    return feat_data;
}


} // namespace util
} // namespace nix
