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


class DataArrayHDF5 : virtual public IDataArray,  public EntityWithSourcesHDF5 {

    static const PSize MIN_CHUNK_SIZE;
    static const PSize MAX_SIZE_1D;

    Group dimension_group;

public:

    /**
     * Copy constructor
     */
    DataArrayHDF5(const DataArrayHDF5 &tag);

    /**
     * Standard constructor
     */
    DataArrayHDF5(File file, Block block, Group group, const std::string &id);

    /**
     * Standard constructor that preserves the creation time.
     */
    DataArrayHDF5(File file, Block block, Group group, const std::string &id, time_t time);

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
    // Methods concerning data access.
    //--------------------------------------------------


    template<typename T, size_t dims>
    void getData(boost::multi_array<T, dims> &data) const{
        getRawData(data);
        double origin = expansionOrigin();
        std::vector<double> polynoms = polynomCoefficients();
        for(auto i = data.data(); i < (data.data() + data.num_elements()); ++i) {
            *i = applyPolynomial(polynoms, origin, (double)(*i));
        }
    }


    template<typename T, size_t dims>
    void getRawData(boost::multi_array<T, dims> &data) const{
        if(group.hasData("data")){
            DataSet ds = group.openData("data");
            ds.read(data, true);
        }
    }


    template<typename T, size_t dims>
    void setRawData(const boost::multi_array<T, dims> &data){
        if (!group.hasData("data")){
            PSize size = {dims};
            PSize maxsize = {H5S_UNLIMITED};
            PSize chunks = {1};
            DataSet ds(DataSet::create(group.h5Group(), "data", data, &maxsize, &chunks));
            ds.write(data);
        }
        else{
            DataSet ds = group.openData("data");
            ds.write(data);
        }
    }

    //--------------------------------------------------
    // Methods concerning dimensions
    //--------------------------------------------------

    std::vector<std::shared_ptr<Dimension>> dimensions() const;


    size_t dimensionCount() const;


    std::shared_ptr<Dimension> getDimension(size_t id) const;


    std::shared_ptr<Dimension> createDimension(size_t id, DimensionType type);


    bool removeDimension(size_t id);

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------


    double applyPolynomial(std::vector<double> &coefficients, double origin, double input) const;


    DataArrayHDF5& operator=(const DataArrayHDF5 &other);


    friend std::ostream& operator<<(std::ostream &out, const DataArrayHDF5 &ent);


    virtual ~DataArrayHDF5();
};


} // namespace hdf5
} // namespace nix

#endif // NIX_DATA_ARRAY_HDF5_H
