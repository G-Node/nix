// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Authors: Christian Kellner <kellner@bio.lmu.de>, Jan Grewe <jan.grewe@g-node.org>

//TODO convenience methods for accessing dimensionality and shape of data

#ifndef NIX_DATA_ARRAY_HDF5_H
#define NIX_DATA_ARRAY_HDF5_H

#include <memory>
#include <boost/multi_array.hpp>

#include <nix/base/IDataArray.hpp>
#include <nix/hdf5/EntityWithSourcesHDF5.hpp>

namespace nix {
namespace hdf5 {


class DataArrayHDF5 : virtual public base::IDataArray,  public EntityWithSourcesHDF5 {

private:

    static const NDSize MIN_CHUNK_SIZE;
    static const NDSize MAX_SIZE_1D;

    Group dimension_group;

public:

    /**
     * Copy constructor
     */
    DataArrayHDF5(const DataArrayHDF5 &tag);

    /**
     * Standard constructor
     */
    DataArrayHDF5(std::shared_ptr<base::IFile> file, const Block &block, const Group &group,
                  const std::string &id, const std::string &type, const std::string &name);

    /**
     * Standard constructor that preserves the creation time.
     */
    DataArrayHDF5(std::shared_ptr<base::IFile> file, const Block &block, const Group &group,
                  const std::string &id, const std::string &type, const std::string &name, time_t time);

    //--------------------------------------------------
    // Element getters and setters
    //--------------------------------------------------


    boost::optional<std::string> label() const;


    void label(const std::string &label);


    void label(const none_t t);


    boost::optional<std::string> unit() const;


    void unit(const std::string &unit);


    void unit(const none_t t);


    boost::optional<double> expansionOrigin() const;


    void expansionOrigin(double expansion_origin);


    void expansionOrigin(const none_t t);


    void polynomCoefficients(const std::vector<double> &polynom_coefficients);


    std::vector<double> polynomCoefficients() const;


    void polynomCoefficients(const none_t t);


    //--------------------------------------------------
    // Methods concerning dimensions
    //--------------------------------------------------

    size_t dimensionCount() const;


    std::shared_ptr<base::IDimension> getDimension(size_t id) const;


    std::shared_ptr<base::ISetDimension> createSetDimension(size_t id);


    std::shared_ptr<base::IRangeDimension> createRangeDimension(size_t id, std::vector<double> ticks);


    std::shared_ptr<base::ISampledDimension> createSampledDimension(size_t id, double sampling_interval);


    bool deleteDimension(size_t id);

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------


    void swap(DataArrayHDF5 &other);


    DataArrayHDF5& operator=(const DataArrayHDF5 &other);


    virtual ~DataArrayHDF5();


    //--------------------------------------------------
    // Methods concerning data access.
    //--------------------------------------------------

    virtual void createData(DataType dtype, const NDSize &size);


    bool hasData() const;


    void write(DataType dtype, const void *data, const NDSize &count, const NDSize &offset);


    void read(DataType dtype, void *buffer, const NDSize &count, const NDSize &offset) const;


    NDSize dataExtent(void) const;


    void   dataExtent(const NDSize &extent);


    DataType dataType(void) const;

private:

    // small helper for handling dimension groups
    Group createDimensionGroup(size_t index);
};


} // namespace hdf5
} // namespace nix

#endif // NIX_DATA_ARRAY_HDF5_H
