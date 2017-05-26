// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_DATA_ARRAY_H
#define NIX_I_DATA_ARRAY_H

#include <nix/base/IEntityWithSources.hpp>
#include <nix/base/IDimensions.hpp>
#include <nix/DataType.hpp>
#include <nix/NDSize.hpp>
#include <nix/ObjectType.hpp>

#include <string>
#include <vector>

namespace nix {
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

    virtual boost::optional<std::string> label() const = 0;


    virtual void label(const std::string &label) = 0;


    virtual void label(const none_t t) = 0;


    virtual boost::optional<std::string> unit() const = 0;


    virtual void unit(const std::string &unit) = 0;



    virtual void unit(const none_t t) = 0;



    virtual boost::optional<double> expansionOrigin()const = 0;


    virtual void expansionOrigin(double expansion_origin) = 0;


    virtual void expansionOrigin(const none_t t) = 0;


    virtual void polynomCoefficients(const std::vector<double> &polynom_coefficients) = 0;


    virtual std::vector<double> polynomCoefficients() const = 0;



    virtual void polynomCoefficients(const none_t t) = 0;

    //--------------------------------------------------
    // Methods concerning dimensions
    //--------------------------------------------------


    virtual ndsize_t dimensionCount() const = 0;


    virtual std::shared_ptr<base::IDimension> getDimension(ndsize_t id) const = 0;


    virtual std::shared_ptr<base::ISetDimension> createSetDimension(ndsize_t id) = 0;


    virtual std::shared_ptr<base::IRangeDimension> createRangeDimension(ndsize_t id, const std::vector<double> &ticks) = 0;


    virtual std::shared_ptr<base::IRangeDimension> createAliasRangeDimension() = 0;


    virtual std::shared_ptr<base::ISampledDimension> createSampledDimension(ndsize_t id, double sampling_interval) = 0;


    virtual bool deleteDimensions() = 0;

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


    virtual NDSize dataExtent(void) const = 0;


    virtual void dataExtent(const NDSize &extent) = 0;


    virtual DataType dataType(void) const = 0;

    /**
     * @brief Destructor
     */
    virtual ~IDataArray() {}

};


} // namespace base

template<>
struct objectToType<nix::base::IDataArray> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::DataArray;
};

} // namespace nix

#endif // NIX_I_DATA_ARRAY_H
