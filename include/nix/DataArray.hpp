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
#include <nix/IDataArray.hpp>
#include <nix/Dimensions.hpp>

namespace nix {


class DataArray : virtual public IDataArray, public base::EntityWithSources<IDataArray> {

public:

    DataArray()
        : EntityWithSources()
    {}

    DataArray(const DataArray &other)
        : EntityWithSources(other.impl_ptr)
    {
    }

    DataArray(const std::shared_ptr<IDataArray> &p_impl)
        : EntityWithSources(p_impl)
    {
    }

    //--------------------------------------------------
    // Element getters and setters
    //--------------------------------------------------

    /**
     * Get the label for the values stored in the DataArray.
     *
     * @return string the label
     */
    std::string label() const {
        return impl_ptr->label();
    }

    /**
     * Set the label for the data stored.
     *
     * @param string label
     */
    void label(const std::string &label) {
        impl_ptr->label(label);
    }


    /**
     * Get the unit of the data stored in this dataArray.
     *
     * @return string the unit.
     */
    std::string unit() const {
        return impl_ptr->unit();
    }

    /**
     * Set the unit for the values stored in this DataArray.
     *
     * @param string the unit
     */
    void unit(const std::string &unit) {
        impl_ptr->unit(unit);
    }


    /**
     * Returns the expansion origin of the calibration polynom.
     * This is set to 0.0 by default.
     *
     * @return double the expansion origin.
     */
    double expansionOrigin() const {
        return impl_ptr->expansionOrigin();
    }

    /**
     * Set the expansion origin for the calibration.
     *
     * @param double the expansion origin.
     */
    void expansionOrigin(double expansion_origin) {
        impl_ptr->expansionOrigin(expansion_origin);
    }

    /**
     * Set the polynom coefficients for the calibration. By default this is set
     * to a two element vector of [0.0, 1.0] for a linear calibration with zero offset.
     *
     * @param vector<double> the coefficients
     */
    void polynomCoefficients(std::vector<double> &polynom_coefficients) {
        impl_ptr->polynomCoefficients(polynom_coefficients);
    }

    /**
     * Returns the polynom coefficients.
     *
     * @return vector<double> the coefficients.
     */
    std::vector<double> polynomCoefficients() const {
        return impl_ptr->polynomCoefficients();
    }

    //--------------------------------------------------
    // Methods concerning data access.
    //--------------------------------------------------

    /**
     * Returns the data stored in the DataArray and applies the calibration.
     * Output is returned in a boost multi_array.
     *
     * @param output argument boost::multi_array<T, dims>
     */
    template<typename U, size_t dims>
    void getData(boost::multi_array<U, dims> &data) const {

        impl_ptr->getRawData<U, dims>(data);
        double origin = impl_ptr->expansionOrigin();
        std::vector<double> polynom = impl_ptr->polynomCoefficients();

        for(auto i = data.data(); i < (data.data() + data.num_elements()); ++i) {
            *i = applyPolynomial(polynom, origin, (double)(*i));
        }
    }

    /**
     * Returns the data as it is stored. Does not apply the calibration.
     *
     * @param output argument boost::multi_array<T, dims>
     */
    template<typename U, size_t dims>
    void getRawData(boost::multi_array<U, dims> &data) const {
        // TODO this does not compile and I do not know why?
        // impl_ptr->getRawData<U, dims>(data);
    }

    /**
     * Set the rawData that is to be stored in the DataArray.
     *
     * @param boost::multi_array<T, dims>
     */
    template<typename U, size_t dims>
    void setRawData(const boost::multi_array<U, dims> &data) {
        impl_ptr->setRawData<U, dims>(data);
    }

    //--------------------------------------------------
    // Methods concerning dimensions
    //--------------------------------------------------

    std::vector<Dimension> dimensions() const {
        return impl_ptr->dimensions();
    }


    size_t dimensionCount() const {
        return impl_ptr->dimensionCount();
    }


    Dimension getDimension(size_t id) const {
        return impl_ptr->getDimension(id);
    }

    Dimension createDimension(size_t id, DimensionType type) {
        return impl_ptr->createDimension(id, type);
    }

    bool removeDimension(size_t id) {
        return impl_ptr->removeDimension(id);
    }

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    double applyPolynomial(std::vector<double> &coefficients, double origin, double input) const;

 };


} // namespace nix

#endif // NIX_DATA_ARRAY_H
