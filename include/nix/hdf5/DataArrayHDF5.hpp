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

#include <nix.hpp>
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
    DataArrayHDF5(const File &file, const Block &block, const Group &group, const std::string &id);

    /**
     * Standard constructor that preserves the creation time.
     */
    DataArrayHDF5(const File &file, const Block &block, const Group &group, const std::string &id, time_t time);

    //--------------------------------------------------
    // Element getters and setters
    //--------------------------------------------------


    std::string label() const;


    void label(const std::string &label);



    std::string unit() const;


    void unit(const std::string &unit);


    double expansionOrigin()const;


    void expansionOrigin(double expansion_origin);


    void polynomCoefficients(std::vector<double> &polynom_coefficients);


    std::vector<double> polynomCoefficients() const;


    //--------------------------------------------------
    // Methods concerning dimensions
    //--------------------------------------------------

    std::vector<Dimension> dimensions() const;


    size_t dimensionCount() const;


    Dimension getDimension(size_t id) const;


    Dimension createDimension(size_t id, DimensionType type);


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


    void write(DataType dtype, const void *data, const NDSize &count, const NDSize &offset);
    void read(DataType dtype, void *buffer, const NDSize &count, const NDSize &offset) const;

    NDSize getExtent(void) const;
    void   setExtent(const NDSize &extent);
};


} // namespace hdf5
} // namespace nix

#endif // NIX_DATA_ARRAY_HDF5_H
