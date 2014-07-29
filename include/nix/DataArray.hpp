// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DATA_ARRAY_H
#define NIX_DATA_ARRAY_H

#include <boost/multi_array.hpp>

#include <nix/base/EntityWithSources.hpp>
#include <nix/base/IDataArray.hpp>
#include <nix/Dimensions.hpp>
#include <nix/Hydra.hpp>

#include <nix/Platform.hpp>

namespace nix {

// TODO add documentation for undocumented methods.

/**
 * @brief A class that can store arbitrary n-dimensional data along with further
 * information.
 *
 * The {@link DataArray} is the core entity of the NIX data model, its purpose is to
 * store arbitrary n-dimensional data. In addition to the common fields id, name, type, and definition
 * the DataArray stores sufficient information to understand the physical nature of the stored data.
 *
 * A guiding principle of the data model is provides enough information to create a
 * plot of the stored data. In order to do so, the DataArray defines a property
 * {@link dataType} which provides the physical type of the stored data (for example
 * 16 bit integer or double precision IEEE floatingpoint number).
 * The property {@link unit} specifies the SI unit of the values stored in the
 * DataArray{} whereas the {@link label} defines what is given in this units.
 * Together, both specify what corresponds to the the y-axis of a plot.
 *
 * In some cases it is much more efficient or convenient to store data not as
 * floating point numbers but rather as (16 bit) integer values as, for example
 * read from a data acquisition board.
 * In order to convert such data to the correct values, we follow the approach
 * taken by the comedi data-acquisition library (http://www.comedi.org)
 * and provide {@link polynomCoefficients} and an {@link expansionOrigin}.
 *
 * ### Create a new data array with a 10 x 10 float matrix as data
 *
 * A DataArray can only be created at an existing block. Do not use the
 * DataArrays constructors for this purpose.
 *
 * ~~~
 * Block b = ...;
 * DataArray da = b.crateDataArray("matrix_10_10", "data");
 * da.createData(DataType::Float, {10, 10});
 * ~~~
 *
 * ### Remove a data array from a file
 *
 * ~~~
 * Block b = ...;
 * bool deleted = da.deleteDataArray(some_id);
 * ~~~
 */
class NIXAPI DataArray : public base::EntityWithSources<base::IDataArray> {

public:

    /**
     * @brief Constructor that creates an uninitialized DataArray.
     *
     * Calling any method on an uninitialized data array will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a data array is initialized:
     *
     * ~~~
     * DataArray e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    DataArray()
        : EntityWithSources()
    {}

    /**
     * @brief Copy constructor.
     *
     * Copying of all NIX front facing objects like DataArray is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The data array to copy.
     */
    DataArray(const DataArray &other)
        : EntityWithSources(other.impl())
    {
    }

    /**
     * @brief Constructor that creates a new data array from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    DataArray(const std::shared_ptr<base::IDataArray> &p_impl)
        : EntityWithSources(p_impl)
    {
    }

    /**
     * @brief Constructor with move semantics that creates a new data array from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    DataArray(std::shared_ptr<base::IDataArray> &&ptr)
        : EntityWithSources(std::move(ptr))
    {
    }

    //--------------------------------------------------
    // Element getters and setters
    //--------------------------------------------------

    /**
     * @brief Get the label for the values stored in the DataArray.
     *
     * @return The label of the data array.
     */
    boost::optional<std::string> label() const {
        return backend()->label();
    }

    /**
     * @brief Set the label for the data stored.
     *
     * @param label     The label of the data array.
     */
    void label(const std::string &label) {
        backend()->label(label);
    }

    /**
     * @brief Deleter for the label attribute.
     *
     * @param t         None
     */
    void label(const none_t t)
    {
        backend()->label(t);
    }

    /**
     * @brief Get the unit of the data stored in this data array.
     *
     * @return The unit of the data array.
     */
    boost::optional<std::string> unit() const {
        return backend()->unit();
    }

    /**
     * @brief Deleter for the unit attribute.
     *
     * @param t         None
     */
    void unit(const none_t t)
    {
        backend()->unit(t);
    }

    /**
     * @brief Set the unit for the values stored in this DataArray.
     *
     * @param unit      The unit of the data array.
     */
    void unit(const std::string &unit);

    /**
     * @brief Returns the expansion origin of the calibration polynom.
     *
     * The expansion origin is 0.0 by default.
     *
     * @return The expansion origin.
     */
    boost::optional<double> expansionOrigin() const {
        return backend()->expansionOrigin();
    }

    /**
     * @brief Set the expansion origin for the calibration.
     *
     * @param expansion_origin  The expansion origin for the calibration.
     */
    void expansionOrigin(double expansion_origin) {
        backend()->expansionOrigin(expansion_origin);
    }

    /**
     * @brief Deleter for the expansion origin.
     *
     * This will reset the expansion origin for the calibration to its
     * default value which is given with 0.0.
     *
     * @param t         None
     */
    void expansionOrigin(const none_t t)
    {
        backend()->expansionOrigin(t);
    }

    /**
     * @brief Set the polynom coefficients for the calibration.
     *
     * By default this is set to a two element vector of [0.0, 1.0] for a linear calibration
     * with zero offset.
     *
     * @param polynom_coefficients      The new polynom coefficients for the calibration.
     */
    void polynomCoefficients(const std::vector<double> &polynom_coefficients) {
        backend()->polynomCoefficients(polynom_coefficients);
    }

    /**
     * @brief Returns the polynom coefficients.
     *
     * @return The polynom coefficients for the calibration.
     */
    std::vector<double> polynomCoefficients() const {
        return backend()->polynomCoefficients();
    }

    /**
     * @brief Deleter for the `polynomCoefficients` attribute.
     *
     * @param t         None
     */
    void polynomCoefficients(const none_t t)
    {
        backend()->polynomCoefficients(t);
    }

    //--------------------------------------------------
    // Methods concerning dimensions
    //--------------------------------------------------

    /**
     * @brief Get all dimensions associated with this data array.
     *
     * The parameter filter can be used to filter sources by various
     * criteria.
     *
     * @param filter    A filter function ({@link nix::util::Filter::type})
     *
     * @return The filtered dimensions as a vector
     */
    std::vector<Dimension> dimensions(util::Filter<Dimension>::type filter) const;

    /**
     * @brief Get all dimensions associated with this data array.
     *
     * Always uses filter that accepts all sources.
     *
     * @return The filtered dimensions as a vector
     */
    std::vector<Dimension> dimensions() const
    {
        return dimensions(util::AcceptAll<Dimension>());
    }

    /**
     * @brief Returns the number of dimensions stored in the DataArray.
     *
     * This matches the dimensionality of the data stored in this property.
     *
     * @return The number of dimensions.
     */
    size_t dimensionCount() const {
        return backend()->dimensionCount();
    }

    /**
     * @brief Returns the Dimension object for the specified dimension of the data.
     *
     * @param id        The index of the respective dimension.
     *
     * @return The dimension object.
     */
    Dimension getDimension(size_t id) const {
        return backend()->getDimension(id);
    }

    /**
     * @brief Append a new SetDimension to the list of existing dimension descriptors.
     *
     * @return The newly created SetDimension.
     */
    SetDimension appendSetDimension() {
        return backend()->createSetDimension(backend()->dimensionCount() + 1);
    }

    /**
     * @brief Append a new RangeDimension to the list of existing dimension descriptors.
     *
     * @param ticks     The ticks of the RangeDimension to create.
     *
     * @return The newly created RangeDimension
     */
    RangeDimension appendRangeDimension(std::vector<double> ticks) {
        return backend()->createRangeDimension(backend()->dimensionCount() + 1, ticks);
    }

    /**
     * @brief Append a new SampledDimension to the list of existing dimension descriptors.
     *
     * @param sampling_interval         The sampling interval of the SetDimension to create.
     *
     * @return The newly created SampledDimension.
     */
    SampledDimension appendSampledDimension(double sampling_interval) {
        return backend()->createSampledDimension(backend()->dimensionCount() + 1, sampling_interval);
    }

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
    SetDimension createSetDimension(size_t id) {
        return backend()->createSetDimension(id);
    }

    /**
     * @brief Create a new RangeDimension at a specified dimension index.
     *
     * This adds a new dimension descriptor of the type {@link nix::RangeDimension} that describes the dimension
     * of the data at the specified index.
     *
     * @param id        The index of the dimension. Must be a value > 0 and <= `dimensionCount + 1`.
     * @param ticks     Vector with {@link nix::RangeDimension::ticks}.
     *
     * @return The created dimension descriptor.
     */
    RangeDimension createRangeDimension(size_t id, std::vector<double> ticks) {
        return backend()->createRangeDimension(id, ticks);
    }

    /**
     * @brief Create a new SampledDimension at a specified dimension index.
     *
     * This adds a new dimension descriptor of the type {@link nix::SampledDimension} that describes the dimension
     * of the data at the specified index.
     *
     * @param id                The index of the dimension. Must be a value > 0 and <= `dimensionCount + 1`.
     * @param sampling_interval  The sampling interval of the dimension.
     *
     * @return The created dimension descriptor.
     */
    SampledDimension createSampledDimension(size_t id, double sampling_interval) {
        return backend()->createSampledDimension(id, sampling_interval);
    }

    /**
     * @brief Remove a dimension descriptor at a specified index.
     *
     * @param id        The index of the dimension. Must be a value > 0 and < `dimensionCount + 1`.
     */
    bool deleteDimension(size_t id) {
        return backend()->deleteDimension(id);
    }

    //--------------------------------------------------
    // Methods concerning data access.
    //--------------------------------------------------

    template<typename T> void getData(T &value) const;

    template<typename T> void setData(const T &value);

    template<typename T> void getData(T &value, const NDSize &count, const NDSize &offset) const;

    template<typename T> void getData(T &value, const NDSize &offset) const;

    template<typename T> void setData(const T &value, const NDSize &offset);

    void getData(DataType dtype,
                 void *data,
                 const NDSize &count,
                 const NDSize &offset) const {
        backend()->read(dtype, data, count, offset);
    }

    void setData(DataType dtype,
                 const void *data,
                 const NDSize &count,
                 const NDSize &offset)
    {
        backend()->write(dtype, data, count, offset);
    }

    /**
     * @brief Get the extent of the data of the DataArray entity.
     *
     * @return The data extent.
     */
    NDSize dataExtent() const {
        return backend()->dataExtent();
    }

    /**
     * @brief Set the data extent of the DataArray entity.
     *
     * @param extent    The extent of the data.
     */
    void dataExtent(const NDSize &extent) {
        backend()->dataExtent(extent);
    }

    /**
     * @brief Get the data type of the data stored in the DataArray entity.
     *
     * @return The data type of the DataArray.
     */
    DataType dataType(void) const {
        return backend()->dataType();
    }

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    /**
    * @brief Output operator
    */
    friend std::ostream & operator<<(std::ostream &out, const DataArray &ent);

    double applyPolynomial(std::vector<double> &coefficients, double origin, double input) const;

};

template<typename T>
void DataArray::getData(T &value) const
{
    Hydra<T> hydra(value);

    NDSize extent = backend()->dataExtent();
    hydra.resize(extent);

    DataType dtype = hydra.element_data_type();
    NDSize shape = hydra.shape();

    backend()->read(dtype, hydra.data(), shape, {});
}

template<typename T>
void DataArray::setData(const T &value)
{
    const Hydra<const T> hydra(value);

    DataType dtype = hydra.element_data_type();
    NDSize shape = hydra.shape();

    if(!backend()->hasData()) {
        backend()->createData(dtype, shape);
    }
    backend()->dataExtent(shape);
    backend()->write(dtype, hydra.data(), shape, {});
}

template<typename T>
void DataArray::getData(T &value, const NDSize &count, const NDSize &offset) const
{
    Hydra<T> hydra(value);
    DataType dtype = hydra.element_data_type();

    hydra.resize(count);
    backend()->read(dtype, hydra.data(), count, offset);
}

template<typename T>
void DataArray::getData(T &value, const NDSize &offset) const
{
    Hydra<T> hydra(value);
    DataType dtype = hydra.element_data_type();

    NDSize count = hydra.shape();
    backend()->read(dtype, hydra.data(), count, offset);
}


template<typename T>
void DataArray::setData(const T &value, const NDSize &offset)
{
    const Hydra<const T> hydra(value);

    DataType dtype = hydra.element_data_type();
    NDSize shape = hydra.shape();

    backend()->write(dtype, hydra.data(), shape, offset);
}

} // namespace nix

#endif // NIX_DATA_ARRAY_H
