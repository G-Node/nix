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

namespace nix {

class Dimension;
enum class DimensionType : int;

// TODO inherit from IEntityWithSources
class IDataArray : virtual public base::IEntityWithSources {

public:

    //--------------------------------------------------
    // Element getters and setters
    //--------------------------------------------------

    /**
     * Get the label for the values stored in the DataArray.
     *
     * @return string the label
     */
    virtual std::string label() const = 0;

    /**
     * Set the label for the data stored.
     *
     * @param string label
     */
    virtual void label(const std::string &label) = 0;


    /**
     * Get the unit of the data stored in this dataArray.
     *
     * @return string the unit.
     */
    virtual std::string unit() const = 0;

    /**
     * Set the unit for the values stored in this DataArray.
     *
     * @param string the unit
     */
    virtual void unit(const std::string &unit) = 0;


    /**
     * Returns the expansion origin of the calibration polynom.
     * This is set to 0.0 by default.
     *
     * @return double the expansion origin.
     */
    virtual double expansionOrigin()const = 0;

    /**
     * Set the expansion origin for the calibration.
     *
     * @param double the expansion origin.
     */
    virtual void expansionOrigin(double expansion_origin) = 0;

    /**
     * Set the polynom coefficients for the calibration. By default this is set
     * to a two element vector of [0.0, 1.0] for a linear calibration with zero offset.
     *
     * @param vector<double> the coefficients
     */
    virtual void polynomCoefficients(std::vector<double> &polynom_coefficients) = 0;

    /**
     * Returns the polynom coefficients.
     *
     * @return vector<double> the coefficients.
     */
    virtual std::vector<double> polynomCoefficients() const = 0;

    //--------------------------------------------------
    // Methods concerning data access.
    //--------------------------------------------------

    // TODO what about the templates?

    //--------------------------------------------------
    // Methods concerning dimensions
    //--------------------------------------------------

    virtual std::vector<Dimension> dimensions() const = 0;


    virtual size_t dimensionCount() const = 0;


    virtual Dimension getDimension(size_t id) const = 0;


    virtual Dimension createDimension(size_t id, DimensionType type) = 0;


    virtual bool removeDimension(size_t id) = 0;

    // TODO add missing methods

};


} // namespace nix

#endif // NIX_I_DATA_ARRAY_H
