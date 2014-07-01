// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_DATA_ARRAY_H
#define NIX_I_DATA_ARRAY_H

#include <string>
#include <vector>
#include <memory>

#include <nix/base/EntityWithSources.hpp>
#include <nix/DataType.hpp>
#include <nix/NDSize.hpp>

namespace nix {

class Dimension;
class SetDimension;
class RangeDimension;
class SampledDimension;
enum class DimensionType : int;


namespace base {

/**
 * @brief Interface for implementations of the DataArray entity.
 *
 * See {@link nix::DataArray} for a more detailed description.
 */
class NIXAPI IDataArray : virtual public base::IEntityWithSources {

public:

    //--------------------------------------------------
    // Element getters and setters
    //--------------------------------------------------

    /**
     * @brief Get the label for the values stored in the DataArray.
     *
     * @return The label of the data array.
     */
    virtual boost::optional<std::string> label() const = 0;

    /**
     * @brief Set the label for the data stored.
     *
     * @param label     The label of the data array.
     */
    virtual void label(const std::string &label) = 0;

    /**
     * @brief Deleter for the label attribute.
     *
     * @param t         None
     */
    virtual void label(const none_t t) = 0;

    /**
     * @brief Get the unit of the data stored in this data array.
     *
     * @return The unit of the data array.
     */
    virtual boost::optional<std::string> unit() const = 0;

    /**
     * @brief Set the unit for the values stored in this DataArray.
     *
     * @param unit      The unit of the data array.
     */
    virtual void unit(const std::string &unit) = 0;


    /**
     * @brief Deleter for the unit attribute.
     *
     * @param t         None
     */
    virtual void unit(const none_t t) = 0;


    /**
     * @brief Returns the expansion origin of the calibration polynom.
     *
     * The expansion origin is 0.0 by default.
     *
     * @return The expansion origin.
     */
    virtual boost::optional<double> expansionOrigin()const = 0;

    /**
     * @brief Set the expansion origin for the calibration.
     *
     * @param expansion_origin  The expansion origin for the calibration.
     */
    virtual void expansionOrigin(double expansion_origin) = 0;

    /**
     * @brief Deleter for the expansion origin.
     *
     * This will reset the expansion origin for the calibration to its
     * default value which is given with 0.0.
     *
     * @param t         None
     */
    virtual void expansionOrigin(const none_t t) = 0;

    /**
     * @brief Set the polynom coefficients for the calibration.
     *
     * By default this is set to a two element vector of [0.0, 1.0] for a linear calibration
     * with zero offset.
     *
     * @param polynom_coefficients      The new polynom coefficients for the calibration.
     */
    virtual void polynomCoefficients(const std::vector<double> &polynom_coefficients) = 0;

    /**
     * @brief Returns the polynom coefficients.
     *
     * @return The polynom coefficients for the calibration.
     */
    virtual std::vector<double> polynomCoefficients() const = 0;


    /**
     * @brief Deleter for the `polynomCoefficients` attribute.
     *
     * @param t         None
     */
    virtual void polynomCoefficients(const none_t t) = 0;

    //--------------------------------------------------
    // Methods concerning dimensions
    //--------------------------------------------------

    /**
     * @brief Returns the number of dimensions stored in the DataArray.
     *
     * This should match (but is not ensured) the dimensionality of the data stored in this data array.
     *
     * @return The number of dimensions.
     */
    virtual size_t dimensionCount() const = 0;

    /**
     * @brief Returns the Dimension object for the specified dimension of the data.
     *
     * @param id        The index of the respective dimension.
     *
     * @return The dimension object.
     */
    virtual Dimension getDimension(size_t id) const = 0;

    /**
     * @brief Create a new SetDimension at a specified dimension index.
     *
     * This adds a new dimension descriptor of the type {@link nix::SetDimension} that describes the dimension
     * of the data at the specified index.
     *
     * @param id        The index of the dimension. Must be a value > 0 and <= `dimensionCount + 1`.
     *
     * @return The created dimension descriptor.
     */
    virtual SetDimension createSetDimension(size_t id) = 0;

    /**
     * @brief Create a new RangeDimension at a specified dimension index.
     *
     * This adds a new dimension descriptor of the type {@link nix::RangeDimension} that describes the dimension
     * of the data at the specified index.
     *
     * @param id        The index of the dimension. Must be a value > 0 and <= `dimensionCount + 1`.
     * @param ticks     The ticks of the range dimension.
     *
     * @return The created dimension descriptor.
     */
    virtual RangeDimension createRangeDimension(size_t id, std::vector<double> ticks) = 0;

    /**
     * @brief Create a new SampledDimension at a specified dimension index.
     *
     * This adds a new dimension descriptor of the type {@link nix::SampledDimension} that describes the dimension
     * of the data at the specified index.
     *
     * @param id                The index of the dimension. Must be a value > 0
     *                          and <= `dimensionCount + 1`.
     * @param samplingInterval  The sampling interval of the sampled dimension.
     *
     * @return The created dimension descriptor.
     */
    virtual SampledDimension createSampledDimension(size_t id, double sampling_interval) = 0;

    /**
     * @brief Remove a dimension descriptor at a specified index.
     *
     * @param id        The index of the dimension. Must be a value > 0 and < `dimensionCount + 1`.
     */
    virtual bool deleteDimension(size_t id) = 0;


    //--------------------------------------------------
    // Methods concerning data access.
    //--------------------------------------------------

    /**
     * @brief Create data for this data array.
     *
     * The following example allocates a file space for a matrix of integers with
     * 10 x 100 elements.
     *
     * ~~~
     * DataArray da = ...;
     * if (!da.hasData()) {
     *     da.createData(DataType::Int32, {10, 100});
     * }
     * ~~~
     *
     * @param dtype     The data type that should be stored in this data array.
     * @param size      The size of the data to store.
     */
    virtual void createData(DataType dtype, const NDSize &size) = 0;

    /**
     * @brief Check if the data array has some data.
     *
     * @return True if the array contains data, false otherwise.
     */
    virtual bool hasData() const = 0;

    /**
     * @brief Write data into the data array.
     *
     * @param dtype     The type of data to write (e.g. {@link nix::DataType::Int32}).
     * @param data      The data to write.
     * @param count     The size of the data to write.
     * @param offset    The position where the writing should start.
     */
    virtual void write(DataType dtype, const void *data, const NDSize &count, const NDSize &offset) = 0;

    /**
     * @brief Read data from the data array.
     *
     * @param dtype     The type of data to read (e.g. {@link nix::DataType::Int32}).
     * @param buffer    Buffer where the data is written.
     * @param count     The size of the data to read.
     * @param offset    The position where the reading should start.
     */
    virtual void read(DataType dtype, void *buffer, const NDSize &count, const NDSize &offset) const = 0;

    /**
     * @brief Get the extent of the data of the DataArray entity.
     *
     * @return The data extent.
     */
    virtual NDSize dataExtent(void) const = 0;

    /**
     * @brief Set the data extent of the DataArray entity.
     *
     * @param extent    The extent of the data.
     */
    virtual void dataExtent(const NDSize &extent) = 0;

    /**
     * @brief Get the data type of the data stored in the DataArray entity.
     *
     * @return The data type of the DataArray.
     */
    virtual DataType dataType(void) const = 0;

    /**
     * @brief Destructor
     */
    virtual ~IDataArray() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_DATA_ARRAY_H
