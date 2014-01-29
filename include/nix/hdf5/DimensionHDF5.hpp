// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DIMENSIONS_HDF5_H
#define NIX_DIMENSIONS_HDF5_H

#include <string>
#include <iostream>
#include <ctime>

#include <nix.hpp>
#include <nix/hdf5/Group.hpp>


namespace nix {
namespace hdf5 {


DimensionType dimensionTypeFromStr(const std::string &str);


std::string dimensionTypeToStr(DimensionType dim);


/**
 * TODO documentation
 */
class DimensionHDF5 : virtual public base::IDimension {

protected:

    Group group;
    size_t dim_id;

public:

    DimensionHDF5(Group group, size_t id);


    DimensionHDF5(const DimensionHDF5 &other);


    size_t id() const { return dim_id; }


    void swap(DimensionHDF5 &other);


    bool operator==(const DimensionHDF5 &other) const;


    bool operator!=(const DimensionHDF5 &other) const;


    virtual ~DimensionHDF5();

protected:

    void setType(); // TODO check if this is needed?

};


/**
 * TODO documentation
 */
class SampledDimensionHDF5 : virtual public base::ISampledDimension, public DimensionHDF5 {

public:

    SampledDimensionHDF5(Group group, size_t id);


    SampledDimensionHDF5(const SampledDimensionHDF5 &other);


    DimensionType dimensionType() const;


    std::string label() const;


    void label(const std::string &label);


    std::string unit() const;


    void unit(const std::string &unit);


    double samplingInterval() const;


    void samplingInterval(double sampling_interval);


    double offset() const;


    void offset(double offset);


    SampledDimensionHDF5& operator=(const SampledDimensionHDF5 &other);


    virtual ~SampledDimensionHDF5();

};


/**
 * TODO documentation
 */
class SetDimensionHDF5 : virtual public base::ISetDimension, public DimensionHDF5 {

public:

    SetDimensionHDF5(Group group, size_t id);


    SetDimensionHDF5(const SetDimensionHDF5 &other);


    DimensionType dimensionType() const;


    std::vector<std::string> labels() const;


    void labels(const std::vector<std::string> &labels);


    SetDimensionHDF5& operator=(const SetDimensionHDF5 &other);


    virtual ~SetDimensionHDF5();

};


/**
 * TODO documentation
 */
class RangeDimensionHDF5 : virtual public base::IRangeDimension, public DimensionHDF5 {

public:

    RangeDimensionHDF5(Group group, size_t id);


    RangeDimensionHDF5(const RangeDimensionHDF5 &other);


    DimensionType dimensionType() const;


    std::string label() const;


    void label(const std::string &label);


    std::string unit() const;


    void unit(const std::string &unit);


    std::vector<double> tics() const;


    void tics(const std::vector<double> &tics);


    RangeDimensionHDF5& operator=(const RangeDimensionHDF5 &other);


    virtual ~RangeDimensionHDF5();

};


} // namespace hdf5
} // namespace nix

#endif // NIX_DIMENSIONS_HDF5_H
