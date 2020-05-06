// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DIMENSIONS_H
#define NIX_DIMENSIONS_H

#include <nix/base/ImplContainer.hpp>
#include <nix/base/IDimensions.hpp>

#include <nix/DataFrame.hpp>

namespace nix {
class DataArray;
class Dimension;

/**
 * @brief Enumeration providing constants for position matching.
 *
 * These constants are used to control the behaviour of the index finding.
 */
enum class PositionMatch {
                          Equal,
                          Less,
                          Greater,
                          GreaterOrEqual,
                          LessOrEqual
};

/**
 * @brief Enumeration providing constants for position matching.
 *
 * These constants are used as Return values when checking if a position is in
 * the data range.
 */
enum class PositionInRange{
                          InRange,
                          Greater,
                          Less
};

/**
 * @brief Dimension descriptor for regularly sampled dimensions.
 *
 * Instances of the SampledDimension Class are used to describe a dimension of data in
 * a DataArray that has been sampled in regular intervals. For example this can be a time axis.
 *
 * Sampled dimensions are characterized by the label for the dimension, the unit in which the
 * sampling interval is given. If not specified otherwise the dimension starts with zero offset.
 *
 * ### Create a sampled dimension
 *
 * The following example will create a sampled dimension on a data array. The dimension
 * has a sampling rate of 10kHz and represents the time axis of a recording.
 *
 * ~~~
 * DataArray da = ...;
 * SampledDimension sd = da.appendSampledDimension(0.1);
 *
 * sd.unit("ms");
 * sd.label("time")
 * sd.offset(10000)
 * ~~~
 */
class NIXAPI SampledDimension : public base::ImplContainer<base::ISampledDimension> {

public:

    /**
     * @brief Constructor that creates an uninitialized SampledDimension.
     *
     * Calling any method on an uninitialized dimension will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a dimension is initialized:
     *
     * ~~~
     * SampledDimension e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    SampledDimension();

    /**
     * @brief Constructor that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    SampledDimension(const std::shared_ptr<base::ISampledDimension> &p_impl);

    /**
     * @brief Constructor with move semantics that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    SampledDimension(std::shared_ptr<base::ISampledDimension> &&ptr);

    /**
     * @brief Copy constructor
     *
     * Copying of all NIX front facing objects like SampledDimension is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The dimension to copy.
     */
    SampledDimension(const SampledDimension &other);

    /**
     * @brief The actual dimension that is described by the dimension descriptor.
     *
     * The index of the dimension entity representing the dimension of the actual
     * data that is defined by this descriptor.
     *
     * @return The dimension index of the dimension.
     */
    ndsize_t index() const {
        return backend()->index();
    }

    /**
     * @brief The type of the dimension.
     *
     * This field indicates whether the dimension is a SampledDimension, SetDimension or
     * RangeDimension.
     *
     * @return The dimension type.
     */
    DimensionType dimensionType() const {
        return backend()->dimensionType();
    }

    /**
     * @brief Getter for the label of the dimension.
     *
     * The label of a SampledDimension corresponds to the axis label
     * in a plot of the respective dimension.
     *
     * @return The label of the dimension.
     */
    boost::optional<std::string> label() const {
        return backend()->label();
    }

    /**
     * @brief Sets the label of the dimension.
     *
     * @param label     The label of the dimension.
     */
    void label(const std::string &label);

    /**
     * @brief Removes the label from a dimension.
     *
     * @param t         None
     */
    void label(const none_t t) {
        backend()->label(t);
    }

    /**
     * @brief Gets the unit of a dimension.
     *
     * The unit describes which SI unit applies to this dimension
     * and to its sampling interval.
     *
     * @return The unit of the dimension.
     */
    boost::optional<std::string> unit() const {
        return backend()->unit();
    }

    /**
     * @brief Sets the unit of a dimension.
     *
     * @param unit      The unit to set.
     */
    void unit(const std::string &unit);

    /**
     * @brief Removes the unit from a dimension.
     *
     * @param t         None
     */
    void unit(const none_t t) {
        backend()->unit(t);
    }

    /**
     * @brief Gets the sampling interval of the dimension.
     *
     * The unit of measure in which the sampling interval must be provided
     * is defined by the {@link unit} of the SampledDimension.
     *
     * @return The sampling interval.
     */
    double samplingInterval() const {
       return backend()->samplingInterval();
    }

    /**
     * @brief Sets the sampling interval of the dimension.
     *
     * @param interval  The sampling interval to set.
     */
    void samplingInterval(double interval);

    /**
     * @brief Gets the offset of the dimension.
     *
     * The offset defines at which position the sampling was started. The offset is
     * interpreted in the same unit as the sampling interval.
     *
     * By default the offset is 0.
     *
     * @returns The offset of the SampledDimension.
     */
    boost::optional<double> offset() const {
        return backend()->offset();
    }

    /**
     * @brief Sets the offset of the dimension.
     *
     * @param offset    The offset of the dimension.
     */
    void offset(double offset) {
        return backend()->offset(offset);
    }

    /**
     * @brief Removes the offset from the dimension.
     *
     * This resets the offset to the default which is 0.
     *
     * @param t         None
     */
    void offset(const boost::none_t t) {
        backend()->offset(t);
    }

    /**
     * @brief Returns the index of the given position.
     *
     * This method returns the index of the given position. Use this method for
     * example to find out which data point (index) relates to a given
     * time. Note: This method does not check if the position is within the
     * extent of the data!
     *
     * @param position  The position, e.g. a time
     *
     * @returns The respective index.
     */
    ndsize_t indexOf(const double position) const;


    /**
     * @brief Returns a pair of indices of the start and end positions within this dimension.
     *
     * This method returns the indices of the given start and end positions. Use this method for
     * example to find out which data indices relate to a given start and end positions given in
     * time. Note: This method does not check if the positions are within the
     * extent of the data!
     *
     * @param start  The start position of, e.g., a time segment
     * @param end    The end position
     *
     * @returns The pair of respective indices.
     */
    std::pair<ndsize_t, ndsize_t> indexOf(const double start, const double end) const;


    /**
     * @brief Returns a vector of start and end indices of given start and end positions.
     *
     * Method will return the index equal or larger than the respective positions
     *
     * @param start_positions    Vector of start positions
     * @param end_positions      Vector of end positions
     *
     * @return  Vector of pairs of start and end indices.
     */
    std::vector<std::pair<ndsize_t, ndsize_t>> indexOf(const std::vector<double> &start_positions,
                                                       const std::vector<double> &end_positions) const;

    /**
     * @brief Returns the position of this dimension at a given index.
     *
     * This method returns the position at a given index. Use this method for
     * example to find the position that relates to a certain index. Note: This
     * method does not check if the index is the extent of the data!
     *
     * @param index  The index.
     *
     * @returns The respective position, e.g. a time.
     */
    double positionAt(const ndsize_t index) const;

    /**
     * @brief Returns a vector containing the positions defined by this
     * dimension
     *
     * Returns an axis vector defined by this dimension.
     *
     * @param count        The number of indices
     * @param startIndex   The start index, default = 0
     *
     * @returns A vector of doubles containing the respective dimension.
     */
    std::vector<double> axis(const ndsize_t count, const ndsize_t startIndex = 0) const;

    /**
     * @brief Assignment operator.
     *
     * @param other     The dimension to assign.
     */
    SampledDimension& operator=(const SampledDimension &other);

    /**
     * @brief Assignment operator that converts a Dimension to a SampledDimension.
     *
     * @param other     The dimension to assign.
     */
    SampledDimension& operator=(const Dimension &other);

    /**
     * @brief Assignment operator for none.
     */
    SampledDimension &operator=(const none_t &t) {
        ImplContainer::operator=(t);
        return *this;
    }

    /**
     * @brief Returns the position at the given index
     *
     * @see positionAt for more information.
     *
     * @param index The index.
     *
     * @returns The position at the given index, e.g. the time.
     */
    double operator[](const ndsize_t index) {
        return positionAt(index);
    }
};


/**
 * @brief Dimension descriptor to be used in conjunction with Information stored in
 * a DataFrame.
 *
 * Suppose that a set of measurements have been made under certain
 * settings. These settings are stored in the table-like structure of a
 * DataFrame. The DataArray can use the information of the DataFrame to describe
 * the respective dimension of the data.
 *
 * A DataFrameDimension is defined by the DataFrame and the index of the column
 * that should be used to annotate the axis of the data.
 *
 * ### Creating a DataFrameDimension:
 *
 * The following example will create a DataArray that stores a set of regularly
 * sampled data recorded under conditions stored in a DataFrame.
 *
 * ~~~
 *  nix::NDSize data_shape = {1000, 10}
 *  double dt = 0.001;
 *  double pi = 3.1415;
 *
 *  // create dummy data
 *  std::vector<double> frequencies(10);
 *  for (size_t i = 0; i < 10; ++i)
 *      frequencies[i] = (i + 1) * 10;
 *
 *  typedef boost::multi_array<double, 2> array_type;
 *  typedef array_type::index index;
 *  array_type data(boost::extents[1000][10]);
 *  for(index i = 0; i < 1000; ++i) {
 *      for(index j = 0; j < 10; ++j) {
 *          data[i][j] = std::sin(i * st * 2 * pi * frequencies[j]);
 *      }
 *  }
 *
 *  // create the DataFrame
 *  std::vector<nix::Column> cols = {{"frequency", "Hz", nix::DataType::Double}};
 *  nix::DataFrame df = block.createDataFrame("frequencies", "nix.df", cols);
 *  df.rows(10)
 *  df.writeColumn(0, frequencies);
 *
 *  // store data in a DataArray
 *  nix::DataArray data_array = b.createDataArray("columnDimTest", "test",
 *                                                nix::DataType::Double, data_shape);
 *  data_array.setData(data, {0, 0});
 *  data_array.label("voltage");
 *  data_array.unit("mV")
 *  // define first data dimension, i.e. regularly sampled in time
 *  SampledDimension dim = data_array.appendSampledDimension(time_axis_samplingInterval)
 *  dim.label = "time";
 *  dim.unit = "s";
 *  // define second data dimension, information is stored in the
 *  // 'frequency' column of the DataFrame
 *  data_array.appendDataFrameDimension(df, 0);
 * ~~~
 */
class NIXAPI DataFrameDimension : public base::ImplContainer<base::IDataFrameDimension> {

 public:
    DataFrameDimension();

    DataFrameDimension(const std::shared_ptr<base::IDataFrameDimension> &p_impl);

    DataFrameDimension(std::shared_ptr<base::IDataFrameDimension> &&ptr);

    DataFrameDimension(const DataFrameDimension &other);

    /**
     * @brief The column index used from the referenced DataFrame.
     *
     * @returns the index the specified column index or -1 if none was set upon
     *          creation.
     */
    boost::optional<unsigned> columnIndex() const {
        return backend()->columnIndex();
    }

    /**
     * @brief Returns the DataFrame to which this Dimension links.
     *
     * @returns shared pointer of the type IDataFrame
     */
    std::shared_ptr<base::IDataFrame> data() const {
        return backend()->dataFrame();
    };

    /**
     * @brief Getter of the Column label.
     *
     * @param col_index the index of the DataFrame column. When called with no
     *        arguments, the default column specified during creation of the
     *        Dimension will be chosen. If no default column was specified upon
     *        creation of the DataFrameDimension and no argument is given the
     *        DataFrame's name will be returned.
     *
     * @returns the label, that is defined in the Column, or if not otherwise
     *          specified, the name of the DataFrame.
     */
    std::string label(boost::optional<unsigned> col_index = {}) const {
        return backend()->label(col_index);
    }

    /**
     * @brief Getter of the Column's unit.
     *
     * @param col_index the index of the DataFrame column. When called with no
     *        arguments, the default column specified during creation of the
     *        Dimension will be chosen. If no default column was specified upon
     *        creation of the DataFrameDimension and no argument is given an
     *        exception will be thrown.
     *
     * @returns the unit, if any, that is defined in the Column.
     */
    std::string unit(boost::optional<unsigned> col_index = {}) const {
        return backend()->unit(col_index);
    }

    /**
     * @brief Getter of the column's data type.
     *
     * @param col_index the index of the DataFrame column. When called with no
     *        arguments, the default column specified during creation of the
     *        Dimension will be chosen. If no default column was specified upon
     *        creation of the DataFrameDimension and no argument is given an
     *        exception will be thrown.
     *
     *  @returns the column's data type
     */
    nix::DataType columnDataType(boost::optional<unsigned> col_index = {}) const {
        return backend()->columnDataType(col_index);
    }

    /**
     * @brief The values (ticks) of the DataFrameDimension.
     *
     * @param col_index: the index of the DataFrame column. When called with no
     *                   arguments, the default column specified during creation
     *                   of the Dimension will be chosen. If no default column
     *                   was specified upon creation of the DataFrameDimension
     *                   and no argument is given an exception will be thrown.
     *
     * @param ticks std::vector<T> return argument to store the ticks.
     *
     * @param resize: boolean to indicate whether or not the ticks vector should
     *                be resized. If false (default), the size of the vector is
     *                taken as the number of ticks to read. If true, all ticks
     *                will be read and returned.
     *
     *  @param offset: the offset (default 0) relative to the start of the
     *                 column.
     *
     */
    template<typename T>
    void ticks(std::vector<T> &ticks, boost::optional<unsigned> col_index = {}, bool resize=false, ndsize_t offset=0) const {
        nix::DataFrame df = data();
        boost::optional<unsigned> column_index;
        if (!col_index) {
            column_index = columnIndex();
        } else {
            column_index = col_index;
        }
        if (!column_index) {
            throw nix::OutOfBounds("DataFrameDimension: Error accessing column, no column index was given and no default is specified.");
        }
        std::vector<Column> cols = df.columns();
        if (static_cast<size_t>(*column_index) >= cols.size()) {
            throw nix::OutOfBounds("DataFrameDimension: Error accessing column, column index exceeds number of columns!");
        }
        df.readColumn(*column_index, ticks, true, offset);
    }


    /**
     * @brief returns the number of entries in the dimension, aka the number of
     * rows in the DataFrame.
     *
     * @returns the size
     */
    ndsize_t size() const {
        nix::DataFrame df = data();
        return df.rows();
    }

    /**
     * @brief The type of the dimension.
     *
     * This field indicates whether the dimension is a SampledDimension, SetDimension or
     * RangeDimension.
     *
     * @return The dimension type.
     */
    DimensionType dimensionType() const {
        return backend()->dimensionType();
    }

    /**
     * @brief Assignment operator.
     *
     * @param other     The dimension to assign.
     */
    DataFrameDimension& operator=(const DataFrameDimension &other);

    /**
     * @brief Assignment operator that converts a Dimension to a DataFrameDimension.
     *
     * @param other     The dimension to assign.
     */
    DataFrameDimension& operator=(const Dimension &other);

    /**
     * @brief Assignment operator for none.
     */
    DataFrameDimension &operator=(const none_t &t) {
        ImplContainer::operator=(t);
        return *this;
    }
};

/**
 * @brief Dimension descriptor for a dimension that represents just a list or set of values.
 *
 * The SetDimension is used in cases where data is given as a set or list. This can be just a collection of values but
 * also a list of recorded signals or a stack of images. Optionally an array of labels, one for each index of this
 * dimension, can be specified.
 */
class NIXAPI SetDimension : public base::ImplContainer<base::ISetDimension> {

public:

    /**
     * @brief Constructor that creates an uninitialized SetDimension.
     *
     * Calling any method on an uninitialized dimension will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a dimension is initialized:
     *
     * ~~~
     * SetDimension e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    SetDimension();

    /**
     * @brief Constructor that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    SetDimension(const std::shared_ptr<base::ISetDimension> &p_impl);

    /**
     * @brief Constructor with move semantics that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    SetDimension(std::shared_ptr<base::ISetDimension> &&ptr);

    /**
     * @brief Copy constructor.
     *
     * Copying of all NIX front facing objects like SetDimension is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The dimension to copy.
     */
    SetDimension(const SetDimension &other);

    /**
     * @brief The actual dimension that is described by the dimension descriptor.
     *
     * The index of the dimension entity representing the dimension of the actual
     * data that is defined by this descriptor.
     *
     * @return The dimension index of the dimension.
     */
    ndsize_t index() const {
        return backend()->index();
    }

    /**
     * @brief The type of the dimension.
     *
     * This field indicates whether the dimension is a SampledDimension, SetDimension or
     * RangeDimension.
     *
     * @return The dimension type.
     */
    DimensionType dimensionType() const {
        return backend()->dimensionType();
    }

    /**
     * @brief Get the labels of the range dimension.
     *
     * The labels serve as names for each index of the data at the respective
     * dimension.
     *
     * @return The labels of the dimension as a vector of strings.
     */
    std::vector<std::string> labels() const {
        return backend()->labels();
    }

    /**
     * @brief Set the labels for the dimension.
     *
     * @param labels    A vector containing all new labels.
     */
    void labels(const std::vector<std::string> &labels) {
        backend()->labels(labels);
    }

    /**
     * @brief Remove the labels from the dimension.
     *
     * @param t         None
     */
    void labels(const boost::none_t t) {
        backend()->labels(t);
    }

    /**
     * @brief Assignment operator.
     *
     * @param other     The dimension to assign.
     */
    SetDimension& operator=(const SetDimension &other);

    /**
     * @brief Assignment operator that converts a Dimension to a SetDimension.
     *
     * @param other     The dimension to assign.
     */
    SetDimension& operator=(const Dimension &other);

    /**
     * @brief Assignment operator for none.
     */
    SetDimension &operator=(const none_t &t) {
        ImplContainer::operator=(t);
        return *this;
    }

};


/**
 * @brief Dimension descriptor for a dimension that is irregularly sampled.
 *
 * The RangeDimension covers cases when indexes of a dimension are mapped to other values
 * in a not regular fashion. A use-case for this would be for example irregularly sampled
 * time-series or certain kinds of histograms. To achieve the mapping of the indexes an
 * array of mapping values must be provided. Those values are stored in the dimensions {@link ticks}
 * property. In analogy to the sampled dimension a {@link unit} and a {@link label} can be defined.
 */
class NIXAPI RangeDimension : public base::ImplContainer<base::IRangeDimension> {

public:

    /**
     * @brief Constructor that creates an uninitialized RangeDimension.
     *
     * Calling any method on an uninitialized dimension will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a dimension is initialized:
     *
     * ~~~
     * RangeDimension e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    RangeDimension();

    /**
     * @brief Constructor that sets up the RangeDimension to use the values
     * stored in the DataArray as ticks.
     *
     * This is **only** valid for DataArray that are 1D and contain numeric values!
     *
     * @param array   The DataArray.
     */
    RangeDimension(const DataArray &array);

    /**
     * @brief Constructor that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    RangeDimension(const std::shared_ptr<base::IRangeDimension> &p_impl);

    /**
     * @brief Constructor with move semantics that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    RangeDimension(std::shared_ptr<base::IRangeDimension> &&ptr);

    /**
     * @brief Copy constructor
     *
     * Copying of all NIX front facing objects like RangeDimension is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The dimension to copy.
     */
    RangeDimension(const RangeDimension &other);

    /**
     *   @brief tells if the RangeDimension uses the contents of a linked DataArray for ticks,
     *  i.e. is an alias.
     *
     * @return bool true, if RangeDimension is an alias, false otherwise.
     */
    bool alias() const {
        return backend()->alias();
    }
    /**
     * @brief The actual dimension that is described by the dimension descriptor.
     *
     * The index of the dimension entity representing the dimension of the actual
     * data that is defined by this descriptor.
     *
     * @return The dimension index of the dimension.
     */
    ndsize_t index() const {
        return backend()->index();
    }

    /**
     * @brief The type of the dimension.
     *
     * This field indicates whether the dimension is a SampledDimension, SetDimension or
     * RangeDimension.
     *
     * @return The dimension type.
     */
    DimensionType dimensionType() const {
        return backend()->dimensionType();
    }

    /**
     * @brief Getter for the label of the dimension.
     *
     * The label of a RangeDimension corresponds to the axis label
     * in a plot of the respective dimension.
     *
     * @return The label of the dimension.
     */
    boost::optional<std::string> label() const {
        return backend()->label();
    }

    /**
     * @brief Sets the label of the dimension.
     *
     * @param label     The label of the dimension.
     */
    void label(const std::string &label);

    /**
     * @brief Remove the label of the dimension.
     *
     * @param t         None
     */
    void label(const none_t t) {
        backend()->label(t);
    }

    /**
     * @brief Gets the unit of a dimension.
     *
     * The unit describes which SI unit applies to this dimension
     * and to its sampling interval.
     *
     * @return The unit of the dimension.
     */
    boost::optional<std::string> unit() const {
        return backend()->unit();
    }

    /**
     * @brief Sets the unit of a dimension.
     *
     * @param unit      The unit to set.
     */
    void unit(const std::string &unit);

    /**
     * @brief Removes the unit from a dimension.
     *
     * @param t         None
     */
    void unit(const none_t t) {
        backend()->unit(t);
    }

    /**
     * @brief Get the ticks of the dimension.
     *
     * The ticks map the index of the data at the respective dimension to other
     * values. This can be used to store data that is sampled at irregular
     * intervals.
     *
     * @return A vector with all ticks for the dimension.
     */
    std::vector<double> ticks() const {
        return backend()->ticks();
    }

    /**
     * @brief Get a number of ticks from the dimension.
     *
     * The ticks map the index of the data at the respective dimension to other
     * values. This can be used to store data that is sampled at irregular
     * intervals.
     *
     * @param start ndsize_t the start index.
     * @param count size_t the number of ticks to read.
     *
     * @return A vector with the ticks for the dimension.
     */
    std::vector<double> ticks(ndsize_t start, size_t count) const {
        return backend()->ticks(start, count);
    }

    /**
     * @brief Set the ticks vector for the dimension.
     *
     * Ticks must be ordered in ascending order.
     *
     * @param ticks     The new ticks for the dimension provided as a vector.
     */
    void ticks(const std::vector<double> &ticks);

    /**
     * @brief Returns the entry of the range dimension at a given index.
     *
     * @param index     The index.
     *
     * @return The tick at the given index.
     *
     * Method will throw an nix::OutOfBounds Exception if the index is invalid
     */
    double tickAt(const ndsize_t index) const;

    /**
     * @brief Returns the index of the given position
     *
     * Method will return the index of the tick that matches or is close to
     * position. If "less_or_equal" is true, the index of the first tick that is
     * less or equal is given. If "less_or_equal" is false the index of the
     * first index that is greater than than position is returned.
     *
     * @param position       The position.
     * @param less_or_equal  If true, the first index that is less or
     *                       equal will be returned. Else, the first index
     *                       that is not less than position will be returned.
     *
     * @return The index.
     */
    ndsize_t indexOf(const double position, bool less_or_equal = true) const;


    /**
     * @brief Returns the start and end index of the given start and end positions.
     *
     * Method will return the index equal or larger than the respective positions
     *
     * @param start      The start position
     * @param end        The end position
     *
     * @return  Start and end index returned in a std::pair.
     */
    std::pair<ndsize_t, ndsize_t> indexOf(const double start, const double end) const;


     /**
     * @brief Returns a vector of start and end indices of given start and end positions.
     *
     * Method will return the index equal or larger than the respective positions
     *
     * @param start_positions    Vector of start positions
     * @param end_positions      Vector of end positions
     *
     * @return  Vector of pairs of start and end indices.
     */
    std::vector<std::pair<ndsize_t, ndsize_t>> indexOf(const std::vector<double> &start_positions,
                                                       const std::vector<double> &end_positions) const;


    /**
     * @brief Returns a vector containing a number of ticks
     *
     * The result vector contains a given number of ticks starting from a
     * starting index
     *
     * @param count       The number of ticks.
     * @param startIndex  The starting index. Default 0.
     *
     * @return vector<double> containing the ticks.
     *
     * Method will throw a nix::OutOfBounds exception if startIndex + count is beyond
     * the number of ticks.
     */
    std::vector<double> axis(const ndsize_t count, const ndsize_t startIndex = 0) const;

    /**
     * @brief Assignment operator.
     *
     * @param other     The dimension to assign.
     */
    RangeDimension& operator=(const RangeDimension &other);

    /**
     * @brief Assignment operator that converts a Dimension to a RangeDimension.
     *
     * @param other     The dimension to assign.
     */
    RangeDimension& operator=(const Dimension &other);

    /**
     * @brief Assignment operator for none.
     */
    RangeDimension &operator=(const none_t &t) {
        ImplContainer::operator=(t);
        return *this;
    }

    /**
     * @brief Returns the position at the given index
     *
     * @see positionAt for more information.
     *
     * @param index The index.
     *
     * @returns The position at the given index, e.g. the time.
     */
    double operator[](const ndsize_t index) {
        return tickAt(index);
    }

};


/**
 * @brief Instances of the Dimension subclasses are used to define the different dimensions of data in a DataArray.
 *
 * The real dimension descriptor are defined in three subclasses: RangeDimension, SampledDimension and  SetDimension
 */
class NIXAPI Dimension : public base::ImplContainer<base::IDimension> {

public:

    /**
     * @brief Constructor that creates an uninitialized Dimension.
     *
     * Calling any method on an uninitialized dimension will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a dimension is initialized:
     *
     * ~~~
     * Dimension e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    Dimension();

    /**
     * @brief Constructor that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Dimension(const std::shared_ptr<base::IDimension> &p_impl);

    /**
     * @brief Constructor with move semantics that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Dimension(std::shared_ptr<base::IDimension> &&ptr);


    /**
     * @brief Copy constructor
     *
     * Copying of all NIX front facing objects like Dimension is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The dimension to copy.
     */
    Dimension(const Dimension &other);

    /**
     * @brief Copy constructor that converts a SampledDimension to Dimension.
     *
     * Copying of all NIX front facing objects like Dimension is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The dimension to copy.
     */
    Dimension(const SampledDimension &other);

    /**
     * @brief Copy constructor that converts a RangeDimension to Dimension.
     *
     * Copying of all NIX front facing objects like Dimension is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The dimension to copy.
     */
    Dimension(const RangeDimension &other);

    /**
     * @brief Copy constructor that converts a SetDimension to Dimension.
     *
     * Copying of all NIX front facing objects like Dimension is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The dimension to copy.
     */
    Dimension(const SetDimension &other);

     /**
     * @brief Copy constructor that converts a DataFrameDimension to Dimension.
     *
     * Copying of all NIX front facing objects like Dimension is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The dimension to copy.
     */
    Dimension(const DataFrameDimension &other);

    /**
     * @brief The actual dimension that is described by the dimension descriptor.
     *
     * The index of the dimension entity representing the dimension of the actual
     * data that is defined by this descriptor.
     *
     * @return The dimension index of the dimension.
     */
    ndsize_t index() const {
        return backend()->index();
    }

    /**
     * @brief The type of the dimension.
     *
     * This field indicates whether the dimension is a SampledDimension, SetDimension,
     * RangeDimension, or DataFrameDimension..
     *
     * @return The dimension type.
     */
    DimensionType dimensionType() const {
        return backend()->dimensionType();
    }

    SetDimension asSetDimension() const;

    SampledDimension asSampledDimension() const;

    RangeDimension asRangeDimension() const;

    DataFrameDimension asDataFrameDimension() const;

    /**
     * @brief Assignment operator that converts a SampledDimension to Dimension.
     *
     * @param other     The dimension to assign.
     */
    Dimension& operator=(const SampledDimension &other);

    /**
     * @brief Assignment operator that converts a RangeDimension to Dimension.
     *
     * @param other     The dimension to assign.
     */
    Dimension& operator=(const RangeDimension &other);

    /**
     * @brief Assignment operator that converts a SetDimension to Dimension.
     *
     * @param other     The dimension to assign.
     */
    Dimension& operator=(const SetDimension &other);

    /**
     * @brief Assignment operator that converts a DataFrameDimension to Dimension.
     *
     * @param other     The dimension to assign.
     */
    Dimension &operator=(const DataFrameDimension &other);

    /**
     * @brief Assignment operator.
     */
    Dimension &operator=(const Dimension &other)  {
        ImplContainer::operator=(other);
        return *this;
    }

    /**
     * @brief Assignment operator for none.
     */
    Dimension &operator=(const none_t &t) {
        ImplContainer::operator=(t);
        return *this;
    }

};

template<>
struct objectToType<nix::SetDimension> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::SetDimension;
    typedef nix::base::ISetDimension backendType;
};


template<>
struct objectToType<nix::SampledDimension> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::SampledDimension;
    typedef nix::base::ISampledDimension backendType;
};



template<>
struct objectToType<nix::RangeDimension> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::RangeDimension;
    typedef nix::base::IRangeDimension backendType;
};

template<>
struct objectToType<nix::DataFrameDimension> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::DataFrameDimension;
    typedef nix::base::IDataFrameDimension backendType;
};

} // namespace nix

#endif // NIX_DIMENSIONS_H
