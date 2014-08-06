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
        if (!util::isScalable(units, dims_units)) {
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

bool extentsMatchPositions::operator()(const DataArray &positions) const {
    // check that positions.dataExtent()[0] == extents.dataExtent()[0]
    // and that   positions.dataExtent()[1] == extents.dataExtent()[1]
    // and that   positions.dataExtent().size() == extents.dataExtent().size()
    return positions.dataExtent() == boost::any_cast<DataArray>(extents).dataExtent();
}
bool extentsMatchPositions::operator()(const std::vector<double> &positions) const {
    return positions.size() == boost::any_cast<std::vector<double>>(extents).size();
}

bool extentsMatchRefs::operator()(const DataArray &extents) const {
    bool mismatch = false;
    auto extExtent = extents.dataExtent();
    auto it = refs.begin();
    while(!mismatch && (it != refs.end())) {
        auto arrayExtent = (*it).dataExtent();
        mismatch = extExtent[1] != arrayExtent.size();
        ++it;
    }
    
    return !mismatch;
}
bool extentsMatchRefs::operator()(const std::vector<double> &extents) const {
    bool mismatch = false;
    auto extSize = extents.size();
    auto it = refs.begin();
    while(!mismatch && (it != refs.end())) {
        auto arrayExtent = (*it).dataExtent();
        mismatch = extSize != arrayExtent.size();
        ++it;
    }
    
    return !mismatch;
}

bool positionsMatchRefs::operator()(const DataArray &positions) const {
    extentsMatchRefs alias = extentsMatchRefs(refs);
    
    return alias(positions);
}
bool positionsMatchRefs::operator()(const std::vector<double> &positions) const {
    extentsMatchRefs alias = extentsMatchRefs(refs);
    
    return alias(positions);
}

bool dimTicksMatchData::operator()(const std::vector<Dimension> &dims) const {
    bool mismatch = false;
    auto it = dims.begin();
    while(!mismatch && it != dims.end()) {
        if ((*it).dimensionType() == DimensionType::Range) {
            size_t dimIndex = (*it).index() - 1;
            auto dim = (*it).asRangeDimension();
            mismatch = dim.ticks().size() == data.dataExtent()[dimIndex];
        }
        ++it;
    }
    
    return mismatch;
}

bool dimLabelsMatchData::operator()(const std::vector<Dimension> &dims) const {
    bool mismatch = false;
    auto it = dims.begin();
    while(!mismatch && it != dims.end()) {
        if ((*it).dimensionType() == DimensionType::Set) {
            size_t dimIndex = (*it).index() - 1;
            auto dim = (*it).asSetDimension();
            mismatch = dim.labels().size() == data.dataExtent()[dimIndex];
        }
        ++it;
    }
    
    return mismatch;
}

} // namespace valid
} // namespace nix
