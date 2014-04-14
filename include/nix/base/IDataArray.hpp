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
enum class DimensionType : int;


namespace base {


class NIXAPI IDataArray : virtual public base::IEntityWithSources {

public:

    //--------------------------------------------------
    // Element getters and setters
    //--------------------------------------------------

    /**
     * Get the label for the values stored in the DataArray.
     *
     * @return boost::optional<std::string> the label
     */
    virtual boost::optional<std::string> label() const = 0;

    /**
     * Set the label for the data stored.
     *
     * @param string label
     */
    virtual void label(const std::string &label) = 0;

    /**
     * Deleter for the label attribute.
     *
     * @param boost::none_t.
     */    
    virtual void label(const none_t t) = 0;
    
    /**
     * Get the unit of the data stored in this dataArray.
     *
     * @return boost::optional<std::string> the unit.
     */
    virtual boost::optional<std::string> unit() const = 0;

    /**
     * Set the unit for the values stored in this DataArray.
     *
     * @param string the unit
     */
    virtual void unit(const std::string &unit) = 0;
    

    /**
     * Deleter for the unit attribute.
     *
     * @param boost::none_t.
     */    
    virtual void unit(const none_t t) = 0;
        

    /**
     * Returns the expansion origin of the calibration polynom.
     * This is set to 0.0 by default.
     *
     * @return double the expansion origin.
     */
    virtual boost::optional<double> expansionOrigin()const = 0;

    /**
     * Set the expansion origin for the calibration.
     *
     * @param double the expansion origin.
     */
    virtual void expansionOrigin(double expansion_origin) = 0;    

    /**
     * Deleter for the expansionOrigin attribute.
     *
     * @param boost::none_t.
     */    
    virtual void expansionOrigin(const none_t t) = 0;
    
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


    /**
     * Deleter for the polynomCoefficients attribute.
     *
     * @param boost::none_t.
     */  
    virtual void polynomCoefficients(const none_t t) = 0;

    //--------------------------------------------------
    // Methods concerning dimensions
    //--------------------------------------------------

    virtual size_t dimensionCount() const = 0;


    virtual Dimension getDimension(size_t id) const = 0;


    virtual Dimension createSetDimension(size_t id) = 0;


    virtual Dimension createRangeDimension(size_t id, std::vector<double> ticks) = 0;


    virtual Dimension createSampledDimension(size_t id, double samplingInterval) = 0;


    virtual bool deleteDimension(size_t id) = 0;


    //--------------------------------------------------
    // Methods concerning data access.
    //--------------------------------------------------

    virtual void createData(DataType dtype, const NDSize &size) = 0;
    
    virtual bool hasData() const = 0;

    virtual void write(DataType dtype, const void *data, const NDSize &count, const NDSize &offset) = 0;
    virtual void read(DataType dtype, void *buffer, const NDSize &count, const NDSize &offset) const = 0;

    virtual NDSize getExtent(void) const = 0;
    virtual void   setExtent(const NDSize &extent) = 0;

    virtual DataType getDataType(void) const = 0;

    // TODO add missing methods

    virtual ~IDataArray() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_DATA_ARRAY_H
