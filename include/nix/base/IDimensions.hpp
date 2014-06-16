// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_DIMENSIONS_H
#define NIX_I_DIMENSIONS_H

#include <nix/Platform.hpp>

#include <string>
#include <vector>
#include <boost/optional.hpp>

namespace nix {

/**
 * @brief Enumeration that provides constants for different dimension types.
 */
enum class DimensionType : int {
    Sample, Set, Range
};


namespace base {

/**
 * @brief Interface for implementations of the Dimension entity.
 *
 * See {@link nix::Dimension} for more detailed information.
 */
class NIXAPI IDimension {

protected:
    typedef boost::none_t none_t;

public:

    /**
     * @brief The actual dimension that is described by the dimension descriptor.
     *
     * The id of the dimension entity which at the same time is the order of the dimension.
     * That is the dimension of the actual data that is defined by this descriptor.
     *
     * @return The dimension index of the dimension.
     */
    virtual size_t id() const = 0;

    /**
     * @brief The type of the dimension.
     *
     * This field indicates whether the dimension is a SampledDimension, SetDimension or
     * RangeDimension.
     *
     * @return The dimension type.
     */
    virtual DimensionType dimensionType() const = 0;

    /**
     * @brief Destructor
     */
    virtual ~IDimension() {}

};


/**
 * @brief Interface for implementations of the SampledDimension entity.
 *
 * See {@link nix::SampledDimension} for more detailed information.
 */
class NIXAPI ISampledDimension : virtual public IDimension {

public:

    /**
     * @brief Getter for the label of the dimension.
     *
     * The label of a SampledDimension corresponds to the axis label
     * in a plot of the respective dimension.
     *
     * @return The label of the dimension.
     */
    virtual boost::optional<std::string> label() const = 0;

    /**
     * @brief Sets the label of the dimension.
     *
     * @param label     The label of the dimension.
     */
    virtual void label(const std::string &label) = 0;

    /**
     * @brief Removes the label from a dimension.
     *
     * @param t         None
     */
    virtual void label(const none_t t) = 0;

    /**
     * @brief Gets the unit of a dimension.
     *
     * The unit describes which SI unit applies to this dimension
     * and to its sampling interval.
     *
     * @return The unit of the dimension.
     */
    virtual boost::optional<std::string> unit() const = 0;

    /**
     * @brief Sets the unit of a dimension.
     *
     * @param unit      The unit to set.
     */
    virtual void unit(const std::string &unit) = 0;

    /**
     * @brief Removes the unit from a dimension.
     *
     * @param t         None
     */
    virtual void unit(const none_t t) = 0;

    /**
     * @brief Gets the sampling interval of the dimension.
     *
     * The unit of measure in which the sampling interval must be provided
     * is defined by the {@link unit} of the SampledDimension.
     *
     * @return The sampling interval.
     */
    virtual double samplingInterval() const = 0;

    /**
     * @brief Sets the sampling interval of the dimension.
     *
     * @param interval  The sampling interval to set.
     */
    virtual void samplingInterval(double interval) = 0;

    /**
     * @brief Gets the offset of the dimension.
     *
     * The offset defines at which position the sampling was started. The unit
     * of measure in which the offset must be provided id defined by the
     * {@link unit} of the SampledDimension.
     *
     * By default the offset is 0.
     *
     * @returns The offset of the SampledDimension.
     */
    virtual boost::optional<double> offset() const = 0;

    /**
     * @brief Sets the offset of the dimension.
     *
     * @param offset    The offset of the dimension.
     */
    virtual void offset(double offset) = 0;

    /**
     * @brief Removes the offset from the dimension.
     *
     * This resets the offset to the default which is 0.
     *
     * @param t         None
     */
    virtual void offset(const none_t t) = 0;

    /**
     * @brief Destructor
     */
    virtual ~ISampledDimension() {}

};


/**
 * @brief Interface for implementations of the SetDimension entity.
 *
 * See {@link nix::SetDimension} for more detailed information.
 */
class NIXAPI ISetDimension : virtual public IDimension {

public:

    /**
     * @brief Get the labels of the range dimension.
     *
     * The labels serve as names for each index of the data at the respective
     * dimension.
     *
     * @return The labels of the dimension as a vector of strings.
     */
    virtual std::vector<std::string> labels() const = 0;

    /**
     * @brief Set the labels for the dimension.
     *
     * @param labels    A vector containing all new labels.
     */
    virtual void labels(const std::vector<std::string> &labels) = 0;

    /**
     * @brief Remove the labels from the dimension.
     *
     * @param t         None
     */
    virtual void labels(const none_t t) = 0;

    /**
     * @brief Destructor
     */
    virtual ~ISetDimension() {}

};


/**
 * @brief Interface for implementations of the RangeDimension entity.
 *
 * See {@link nix::RangeDimension} for more detailed information.
 */
class NIXAPI IRangeDimension : virtual public IDimension {

public:

    /**
     * @brief Getter for the label of the dimension.
     *
     * The label of a RangeDimension corresponds to the axis label
     * in a plot of the respective dimension.
     *
     * @return The label of the dimension.
     */
    virtual boost::optional<std::string> label() const = 0;

    /**
     * @brief Sets the label of the dimension.
     *
     * @param label     The label of the dimension.
     */
    virtual void label(const std::string &label) = 0;

    /**
     * @brief Remove the label of the dimension.
     *
     * @param t         None
     */
    virtual void label(const none_t t) = 0;

    /**
     * @brief Gets the unit of a dimension.
     *
     * The unit describes which SI unit applies to this dimension
     * and to its sampling interval.
     *
     * @return The unit of the dimension.
     */
    virtual boost::optional<std::string> unit() const = 0;

    /**
     * @brief Sets the unit of a dimension.
     *
     * @param unit      The unit to set.
     */
    virtual void unit(const std::string &unit) = 0;

    /**
     * @brief Removes the unit from a dimension.
     *
     * @param t         None
     */
    virtual void unit(const none_t t) = 0;

    /**
     * @brief Get the ticks of the dimension.
     *
     * The ticks map the index of the data at the respective dimension to other
     * values. This can be used to store data that is sampled at irregular
     * intervals.
     *
     * Ticks must be ordered in ascending order.
     *
     * @return A vector with all ticks for the dimension.
     */
    virtual std::vector<double> ticks() const = 0;

    /**
     * @brief Set the ticks vector for the dimension.
     *
     * Ticks must be ordered in ascending order.
     *
     * @param ticks     The new ticks for the dimension provided as a vector.
     */
    virtual void ticks(const std::vector<double> &ticks) = 0;

    /**
     * @brief Destructor
     */
    virtual ~IRangeDimension() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_DIMENSIONS_H
