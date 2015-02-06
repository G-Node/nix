// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DIMENSIONS_HDF5_H
#define NIX_DIMENSIONS_HDF5_H

#include <nix/base/IDimensions.hpp>
#include <nix/hdf5/Group.hpp>

#include <string>
#include <iostream>
#include <ctime>

namespace nix {
namespace hdf5 {


DimensionType dimensionTypeFromStr(const std::string &str);


std::string dimensionTypeToStr(DimensionType dim);


std::shared_ptr<base::IDimension> openDimensionHDF5(const Group &group, size_t index);


class DimensionHDF5 : virtual public base::IDimension {

protected:

    Group group;
    size_t dim_index;

public:

    DimensionHDF5(const Group &group, size_t index);


    size_t index() const { return dim_index; }


    bool operator==(const DimensionHDF5 &other) const;


    bool operator!=(const DimensionHDF5 &other) const;


    virtual ~DimensionHDF5();

protected:

    void setType();

};


class SampledDimensionHDF5 : virtual public base::ISampledDimension, public DimensionHDF5 {

public:

    SampledDimensionHDF5(const Group &group, size_t index);


    SampledDimensionHDF5(const Group &group, size_t index, double sampling_interval);


    DimensionType dimensionType() const;


    boost::optional<std::string> label() const;


    void label(const std::string &label);


    void label(const none_t t);


    boost::optional<std::string> unit() const;


    void unit(const std::string &unit);


    void unit(const none_t t);


    double samplingInterval() const;


    void samplingInterval(double sampling_interval);


    boost::optional<double> offset() const;


    void offset(double offset);


    void offset(const none_t t);


    virtual ~SampledDimensionHDF5();

};


class SetDimensionHDF5 : virtual public base::ISetDimension, public DimensionHDF5 {

public:

    SetDimensionHDF5(const Group &group, size_t index);


    DimensionType dimensionType() const;


    std::vector<std::string> labels() const;


    void labels(const std::vector<std::string> &labels);


    void labels(const none_t t);


    virtual ~SetDimensionHDF5();

};


class RangeDimensionHDF5 : virtual public base::IRangeDimension, public DimensionHDF5 {

public:

    RangeDimensionHDF5(const Group &group, size_t index);


    RangeDimensionHDF5(const Group &group, size_t index, std::vector<double> ticks);


    DimensionType dimensionType() const;


    boost::optional<std::string> label() const;


    void label(const std::string &label);


    void label(const none_t t);


    boost::optional<std::string> unit() const;


    void unit(const std::string &unit);


    void unit(const none_t t);


    std::vector<double> ticks() const;


    void ticks(const std::vector<double> &ticks);


    virtual ~RangeDimensionHDF5();

};


} // namespace hdf5
} // namespace nix

#endif // NIX_DIMENSIONS_HDF5_H
