// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/valid/checks.hpp>

#include <functional>
#include <vector>
#include <string>

#include <nix.hpp>

namespace nix {
namespace valid {

bool dimEquals::operator()(const DataArray &array) const {
    return (array.dataExtent().size() == value);
}


bool tagRefsHaveUnits::operator()(const std::vector<DataArray> &references) const {
    bool match = true;
    std::vector<std::string> dims_units;

    for (auto &ref : references) {
        dims_units = getDimensionsUnits(ref);
        if (dims_units.size() != units.size()){
            match = false;
            break;
        }
    }

    return match;
}


bool tagUnitsMatchRefsUnits::operator()(const std::vector<DataArray> &references) const {
    bool match = true;
    std::vector<std::string> dims_units;

    for (auto &ref : references) {
        dims_units = getDimensionsUnits(ref);
        if (!util::isScalable(units, dims_units)) {
            match = false;
            break;
        }
    }

    return match;
}


bool dimTicksMatchData::operator()(const std::vector<Dimension> &dims) const {
    bool mismatch = false;
    auto it = dims.begin();
    while (!mismatch && it != dims.end()) {
        if ((*it).dimensionType() == DimensionType::Range) {
            ndsize_t dimIndex = (*it).index() - 1;
            if (dimIndex >= data.dataExtent().size()) {
                break;
            }
            auto dim = (*it).asRangeDimension();
            size_t idx = check::fits_in_size_t(dimIndex, "Cannot check ticks: dimension bigger than size_t.");
            mismatch = !(dim.ticks().size() == data.dataExtent()[idx]);
        }
        ++it;
    }
    return !mismatch;
}


bool dimLabelsMatchData::operator()(const std::vector<Dimension> &dims) const {
    bool mismatch = false;
    auto it = dims.begin();
    while (!mismatch && it != dims.end()) {
        if ((*it).dimensionType() == DimensionType::Set) {
            ndsize_t dimIndex = (*it).index() - 1;
            if (dimIndex >= data.dataExtent().size()) {
                break;
            }
            auto dim = (*it).asSetDimension();
            size_t idx = check::fits_in_size_t(dimIndex, "Cannot check labels: dimension bigger than size_t.");
            mismatch = dim.labels().size() > 0 && !(dim.labels().size() == data.dataExtent()[idx]);
        }
        ++it;
    }
    return !mismatch;
}


bool dimDataFrameTicksMatchData::operator()(const std::vector<Dimension> &dims) const {
    bool mismatch = false;
    auto it = dims.begin();
    while (!mismatch && it != dims.end()) {
        if ((*it).dimensionType() == DimensionType::DataFrame) {
            ndsize_t dimIndex = (*it).index() - 1;
            if (dimIndex >= data.dataExtent().size()) {
                break;
            }
            auto dim = (*it).asDataFrameDimension();
            size_t idx = check::fits_in_size_t(dimIndex, "Cannot check ticks of DataFrameDimension: dimension bigger than size_t.");
            mismatch = !(dim.size() == data.dataExtent()[idx]);
        }
        ++it;
    }
    return !mismatch;
}

} // namespace valid
} // namespace nix
