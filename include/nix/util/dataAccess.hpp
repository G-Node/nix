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

#include <nix/NDArray.hpp>
#include <nix/DataView.hpp>
#include <nix/Dimensions.hpp>
#include <nix/DataArray.hpp>
#include <nix/MultiTag.hpp>
#include <nix/Tag.hpp>

#include <ctime>

namespace nix {
namespace util {

/**
 * @brief Converts a position to an index according to the dimension descriptor.
 *
 * This function can be used to get the index of e.g. a certain point in time in a Dimension that
 * represents time.
 *
 * @param position      The position
 * @param match         Member of the PositionMatch enum to control the matching.
 * @param dimension     The dimension descriptor for the respective dimension, i.e. a SetDimension.
 *
 * @return The calculated index.
 *
 * @throws nix::IncompatibleDimension The the dimensions are incompatible.
 * @throws nix::OutOfBounds If the position either too large or too small for the dimension.
 */
NIXAPI boost::optional<ndsize_t> positionToIndex(double position, const PositionMatch match, const SetDimension &dimension);

NIXAPI DEPRECATED ndsize_t positionToIndex(double position, const std::string &unit, const SetDimension &dimension);

/**
 * @brief Converts a position given in a unit into an index according to the dimension descriptor.
 *
 * This function can be used to get the index of e.g. a certain point in time in a Dimension that
 * represents time. The units of the position and that provided by the Dimension must match, i.e.
 * must be scalable versions of the same SI unit.
 *
 * @param position      The position
 * @param unit          The unit in which the position is given, may be "none"
 * @param dimension     The dimension descriptor for the respective dimension.
 *
 * @return The calculated index.
 *
 * @throws nix::IncompatibleDimension The the dimensions are incompatible.
 * @throws nix::OutOfBounds If the position either too large or too small for the dimension.
 */
NIXAPI  boost::optional<ndsize_t> positionToIndex(double position, const std::string &unit, const PositionMatch match, const SampledDimension &dimension);

NIXAPI  DEPRECATED ndsize_t positionToIndex(double position, const std::string &unit, const SampledDimension &dimension);

/**
 * @brief Converts a position given in a unit into an index according to the dimension descriptor.
 *
 * This function can be used to get the index of e.g. a certain point in time in a Dimension that
 * represents time. The units of the position and that provided by the Dimension must match, i.e.
 * must be scalable versions of the same SI unit.
 *
 * @param position      The position
 * @param unit          The unit in which the position is given, may be "none"
 * @param dimension     The dimension descriptor for the respective dimension.
 *
 * @return An optional containng the index.
 *
 * @throws nix::IncompatibleDimension The the dimensions are incompatible.
 * @throws nix::OutOfBounds If the position either too large or too small for the dimension.
 */
NIXAPI boost::optional<ndsize_t> positionToIndex(double position, const std::string &unit, const PositionMatch position_match, const RangeDimension &dimension);

NIXAPI DEPRECATED ndsize_t positionToIndex(double position, const std::string &unit, const RangeDimension &dimension);

 /**
 * @brief Converts the passed vector of start and end positions in a unit into a vector of respective indices
 * according to the dimension descriptor.
 *
 * This function can be used to get the index of e.g. a certain point in time in a Dimension that
 * represents time. The units of the position and that provided by the Dimension must match, i.e.
 * must be scalable versions of the same SI unit.
 *
 * @param positions     std::vector of positions
 * @param units         std::vector of units in which the respective position is given, must have the same size or may
 *                      be empty
 * @param rangeMatching The desired range matching behavior see {@link Dimension::RangeMatch}.
 * @param dimension     The dimension descriptor for the respective dimension.
 *
 * @return The calculated indices.
 *
 * @throws nix::IncompatibleDimension The the dimensions are incompatible.
 * @throws nix::OutOfBounds If the position either too large or too small for the dimension.
 */       
NIXAPI std::vector<boost::optional<std::pair<ndsize_t, ndsize_t>>> positionToIndex(const std::vector<double> &start_positions,
                                                                                   const std::vector<double> &end_positions,
                                                                                   const std::vector<std::string> &units,
                                                                                   const RangeMatch rangeMatching,
                                                                                   const SampledDimension &dimension);

/**
 * @deprecated this function has been deprecated see {@link nix::util::positionToIndex(const std::vector<double>&, const std::vector<double>&, const std::vector<std::string> &, const RangeMatch, const SampledDimension&)}
 */
NIXAPI DEPRECATED std::vector<std::pair<ndsize_t, ndsize_t>> positionToIndex(const std::vector<double> &start_positions,
                                                                             const std::vector<double> &end_positions,
                                                                             const std::vector<std::string> &units,
                                                                             const SampledDimension &dimension);


NIXAPI std::vector<boost::optional<std::pair<ndsize_t, ndsize_t>>> positionToIndex(const std::vector<double> &start_positions,
                                                                                   const std::vector<double> &end_positions,
                                                                                   const RangeMatch rangeMatching,
                                                                                   const SetDimension &dimension);

/**
 * @deprecated this function has been deprecated see {@link nix::util::positionToIndex(const std::vector<double>&, const std::vector<double>&, const std::vector<std::string> &, const RangeMatch, const SetDimension&)}
 */
NIXAPI DEPRECATED std::vector<std::pair<ndsize_t, ndsize_t>> positionToIndex(const std::vector<double> &start_positions,
                                                                             const std::vector<double> &end_positions,
                                                                             const std::vector<std::string> &units,
                                                                             const SetDimension &dimension);


NIXAPI std::vector<boost::optional<std::pair<ndsize_t, ndsize_t>>> positionToIndex(const std::vector<double> &start_positions,
                                                                                   const std::vector<double> &end_positions,
                                                                                   const std::vector<std::string> &units,
                                                                                   const RangeMatch rangeMatching,
                                                                                   const RangeDimension &dimension);

/**
 * @deprecated this function has been deprecated see {@link nix::util::positionToIndex(const std::vector<double>&, const std::vector<double>&, const std::vector<std::string> &, const RangeMatch, const RangeDimension&)}
 */
NIXAPI DEPRECATED std::vector<std::pair<ndsize_t, ndsize_t>> positionToIndex(const std::vector<double> &start_positions,
                                                                             const std::vector<double> &end_positions,
                                                                             const std::vector<std::string> &units,
                                                                             const RangeDimension &dimension);


/**
 * @brief Returns the offsets and element counts associated with position and extent of a Tag and
 *        the referenced DataArray.
 *
 * @param tag           The tag.
 * @param array         A referenced data array.
 * @param[out] offsets  The resulting offset.
 * @param[out] counts   The number of elements to read from data
 * @param match         Member of the RangeMatch enum, defining wheter the range includes the end position
 *                      or not. By default this is RangeMatch::Inclusive (Note: this behavior will change 
 *                      with version 1.5)
 */
NIXAPI void getOffsetAndCount(const Tag &tag, const DataArray &array, NDSize &offsets, NDSize &counts, RangeMatch match = RangeMatch::Inclusive);


NIXAPI void getOffsetAndCount(const MultiTag &tag, const DataArray &array, ndsize_t index, NDSize &offsets, NDSize &counts, RangeMatch match = RangeMatch::Inclusive);

NIXAPI void getOffestAndCount(const MultiTag &tag, const DataArray &array, const std::vector<ndsize_t> indices,
                              std::vector<NDSize> &offsets, std::vector<NDSize> & counts, RangeMatch match = RangeMatch::Inclusive);


/**
 * @brief Returns a slice of the data stored in the passed {@link
 * nix::DataArray} that is defined by start and end points given in dimension
 * units. For example, if the data array is 1-D defined with a time axis, the
 * requested data slice is given in times.
 *
 * @param array          The {@link nix::DataArray}.
 * @param start          A std::vector<double> that define the starting points of the data slice.
 * @param end            A std::vector<double> that defines the end point of the data slice.
 * @param units          A std::vector<std::string> containing the  units which have to be used for each dimension.
 *                       Start and end are assumed to be given in the same unit. If no unit is required for a dimension
 *                       provide an empty string or "none". SI units will be automatically scaled. That is, a time
 *                       start may be given in s even though the dimension is defined in ms.
 *
 * @returns {@link nix::DataView} the data slice.
 */
NIXAPI DataView dataSlice(const DataArray &array, const std::vector<double> &start, const std::vector<double> &end,
                          const std::vector<std::string> &units={}, RangeMatch match = RangeMatch::Inclusive);

/**
 * @brief Retrieve the data referenced by the given position and extent of the MultiTag.
 *
 * @param tag                   The multi tag.
 * @param position_index        The index of the position.
 * @param array                 The referenced DataArray.
 *
 * @return The data referenced by position and extent.
 * @deprecated This function has been deprecated! Use retrieveData(MultiTag, vector<ndsize_t>, DataArray) instead.
 */
NIXAPI DEPRECATED DataView retrieveData(const MultiTag &tag, ndsize_t position_index, const DataArray &array, RangeMatch match = RangeMatch::Inclusive);

/**
 * @brief Retrieve the data referenced by the given position and extent of the MultiTag.
 *
 * @param tag                   The multi tag.
 * @param position_index        The index of the position.
 * @param reference_index       The index of the reference from which data should be returned.
 *
 * @return The data referenced by position and extent.
 * @deprecated This function has been deprecated! Use retrieveData(MultiTag, vector<ndsize_t>, DataArray) instead.
 */
NIXAPI DEPRECATED DataView retrieveData(const MultiTag &tag, ndsize_t position_index, ndsize_t reference_index, RangeMatch match = RangeMatch::Inclusive);


/**
 * @brief Retrieve several data segments referenced by the given position and extent of the MultiTag.
 *
 * @param tag                   The multi tag.
 * @param position_index        The indices of the positions.
 * @param array                 The referenced DataArray.
 *
 * @return The data referenced by the specified indices, respectively their positions and extents.
 */
NIXAPI std::vector<DataView> retrieveData(const MultiTag &tag, std::vector<ndsize_t> &position_indices, const DataArray &array, RangeMatch match = RangeMatch::Inclusive);

/**
 * @brief Retrieve several segments of  data referenced by the given position and extent of the MultiTag.
 *
 * @param tag                   The multi tag.
 * @param position_index        The indices of the position.
 * @param array                 The referenced DataArray.
 *
 * @return The data referenced by the specified indices, respectively their positions and extents.
 */
NIXAPI std::vector<DataView> retrieveData(const MultiTag &tag, std::vector<ndsize_t> &position_indices, ndsize_t reference_index, RangeMatch match = RangeMatch::Inclusive);


/**
 * @brief Retrieve the data referenced by the given position and extent of the Tag.
 *
 * @param tag                   The multi tag.
 * @param reference_index       The index of the reference from which data should be returned.
 *
 * @return The data referenced by the position.
 */
NIXAPI DataView retrieveData(const Tag &tag, ndsize_t reference_index, RangeMatch match = RangeMatch::Inclusive);

/**
 * @brief Retrieve the data referenced by the given position and extent of the Tag.
 *
 * @param tag                   The multi tag.
 * @param array                 The referenced DataArray.
 *
 * @return The data referenced by the position.
 */
NIXAPI DataView retrieveData(const Tag &tag, const DataArray &array, RangeMatch match = RangeMatch::Inclusive);


/**
 * @brief Checks whether a given position is in the extent of the given DataArray.
 *
 * @param data          The data array.
 * @param position      The position.
 *
 * @return True if the position is in the extent of the data array, false otherwise.
 */
NIXAPI bool positionInData(const DataArray &data, const NDSize &position);

/**
 * @brief Checks whether a given position plus count is in the extent of the given DataArray.
 *
 * @param data          The DataArray.
 * @param position      The position
 * @param count         The number of elements per dimension.
 *
 * @return True if position and count are in the extent of the data array, false otherwise.
 */
NIXAPI bool positionAndExtentInData(const DataArray &data, const NDSize &position, const NDSize &count);

/**
 * @brief Retruns the feature data associated with a Tag.
 *
 * @param tag           The Tag whos feature data is requested
 * @param feature_index The index of the desired feature. Default is 0.
 * @param range_match   Member of RangeMatch enum to control whether a 
 *                      data range should include the last position or not
 *                      (only required for tagged features, otherwise ignored) 
 *
 * @return The associated data.
 */
NIXAPI DataView retrieveFeatureData(const Tag &tag, ndsize_t feature_index=0, RangeMatch range_match = RangeMatch::Inclusive);

/**
 * @brief Retruns the feature data associated with a Tag.
 *
 * @param tag           The Tag whos feature data is requested.
 * @param feature       The Feature of which the tagged data is requested.
 * @param range_match   Member of RangeMatch enum to control whether a 
 *                      data range should include the last position or not
 *                      (only required for tagged features, otherwise ignored)
 * @return The associated data.
 */
NIXAPI DataView retrieveFeatureData(const Tag &tag, const Feature &feature, RangeMatch range_match = RangeMatch::Inclusive);

/**
 * @brief Returns the feature data associated with the given MuliTag's position.
 *
 * @param tag            The MultiTag whos feature data is requested.
 * @param position_index The index of the selected position, respectively the selected tag of the MultiTag.
 * @param feature_index  The index of the desired feature. Default is 0.
 * @param range_match   Member of RangeMatch enum to control whether a 
 *                      data range should include the last position or not
 *                      (only required for tagged features, otherwise ignored)
 * 
 * @return The associated data.
 */
NIXAPI DataView retrieveFeatureData(const MultiTag &tag, ndsize_t position_index, ndsize_t feature_index=0, RangeMatch range_match = RangeMatch::Inclusive);

/**
 * @brief Returns the feature data associated with the given MuliTag's position.
 *
 * @param tag            The MultiTag whos feature data is requested.
 * @param position_index The index of the selected position, respectively the selected tag of the MultiTag.
 * @param feature        The feature of which the tagged data is requested.
 * @param range_match   Member of RangeMatch enum to control whether a 
 *                      data range should include the last position or not
 *                      (only required for tagged features, otherwise ignored)
 *
 * @return The associated data.
 */
NIXAPI DataView retrieveFeatureData(const MultiTag &tag, ndsize_t position_index, const Feature &feature, RangeMatch range_match = RangeMatch::Inclusive);


/**
 * @brief Retruns the feature data associated with a MultiTag.
 *
 * @param tag              The MultiTag whos feature data is requested
 * @param position_indices A vector of position indices.
 * @param feature_index    The index of the desired feature. Default is 0.
 * @param range_match   Member of RangeMatch enum to control whether a 
 *                      data range should include the last position or not
 *                      (only required for tagged features, otherwise ignored)
 * 
 * @return A vector of the associated data, may be empty.
 */
NIXAPI std::vector<DataView> retrieveFeatureData(const MultiTag &tag, std::vector<ndsize_t> position_indices,
                                                 ndsize_t feature_index = 0, RangeMatch range_match = RangeMatch::Inclusive);

/**
 * @brief Returns the feature data associated with the given MuliTag's positions.
 *
 * @param tag              The MultiTag whos feature data is requested.
 * @param position_indices A vector of position indices.
 * @param feature          The feature of which the tagged data is requested.
 * @param range_match   Member of RangeMatch enum to control whether a 
 *                      data range should include the last position or not
 *                      (only required for tagged features, otherwise ignored)
 * 
 * @return A vector of the associated data, may be empty.
 */
NIXAPI std::vector<DataView> retrieveFeatureData(const MultiTag &tag, std::vector<ndsize_t> position_indices,
                                                 const Feature &feature, RangeMatch range_match = RangeMatch::Inclusive);

} //namespace util
} //namespace nix
#endif // NIX_DATAACCESS_H
