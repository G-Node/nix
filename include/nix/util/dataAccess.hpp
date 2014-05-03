// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @namespace nix::util
 * @brief Namespace for utility functions. This namespace is not part of the public API.
 */

#ifndef NIX_DATAACCESS_H
#define NIX_DATAACCESS_H

#include <nix/util/util.hpp>
#include <nix.hpp>

namespace nix {
namespace util {

/**
 * Converts a position given in a unit into an index according to the dimension descriptor.
 * This function can be used to get the index of e.g. a certain point in time in a Dimension that
 * represents time. The units of the position and that provided by the Dimension must match, i.e.
 * must be scalable versions of the same SI unit.
 *
 * @param position double, the position
 * @param unit string, the unit in which the position is given
 * @param dimension SetDimension, the dimension descriptor for the respective dimension.
 *
 * @return int the index
 * @throws nix::incompatibleDimension or nix::OutOfBounds exceptions.
 */
NIXAPI size_t positionToIndex(double position, const std::string &unit, const SetDimension &dimension);

/**
 * Returns the index of a position.
 * see positionToIndex(double position, const std::string &unit, const SetDimension &dimension)
 * for more information.
 */
NIXAPI size_t positionToIndex(double position, const std::string &unit, const SampledDimension &dimension);

/**
 * Returns the index of a position.
 * see positionToIndex(double position, const std::string &unit, const SetDimension &dimension)
 * for more information
 */
NIXAPI size_t positionToIndex(double position, const std::string &unit, const RangeDimension &dimension);

/**
 * Returns the offsets and element counts associated with position and extent of a SimpleTag and the referenced
 * DataArray.
 *
 * @param tag the SimpleTag
 * @param array the referenced DataArray
 * @param offsets NDSize output argument the offset into the data
 * @param counts NDSize output argument the number of elements to read from data
 */
NIXAPI void getOffsetAndCount(const SimpleTag &tag, const DataArray &array, NDSize &offsets, NDSize &counts);

}
}

#endif // NIX_DATAACCESS_H
