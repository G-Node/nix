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

#include <nix/base/IDimensions.hpp>
#include <nix/hdf5/Group.hpp>


namespace nix {
namespace hdf5 {


DimensionType dimensionTypeFromStr(const std::string &str);


std::string dimensionTypeToStr(DimensionType dim);


std::shared_ptr<base::IDimension> openDimensionHDF5(Group group, size_t index);


class DimensionHDF5 : virtual public base::IDimension {

protected:

    Group group;
    size_t dim_index;

public:

    DimensionHDF5(Group group, size_t index);


    size_t index() const { return dim_index; }


    void swap(DimensionHDF5 &other);


    bool operator==(const DimensionHDF5 &other) const;


    bool operator!=(const DimensionHDF5 &other) const;


    virtual ~DimensionHDF5();

protected:

    void setType();

};


class SampledDimensionHDF5 : virtual public base::ISampledDimension, public DimensionHDF5 {

public:

    SampledDimensionHDF5(Group group, size_t index);


    SampledDimensionHDF5(Group group, size_t index, double sampling_interval);


    SampledDimensionHDF5(const SampledDimensionHDF5 &other);


    DimensionType dimensionType() const;


    boost::optional<std::string> label() const;


    void label(const std::string &label);


    void label(none_t t);


    boost::optional<std::string> unit() const;


    void unit(const std::string &unit);


    void unit(none_t t);


    double samplingInterval() const;


    void samplingInterval(double sampling_interval);


    boost::optional<double> offset() const;


    void offset(double offset);


    void offset(const none_t t);


    SampledDimensionHDF5& operator=(const SampledDimensionHDF5 &other);


    virtual ~SampledDimensionHDF5();

};


class SetDimensionHDF5 : virtual public base::ISetDimension, public DimensionHDF5 {

public:

    SetDimensionHDF5(Group group, size_t index);


    SetDimensionHDF5(const SetDimensionHDF5 &other);


    DimensionType dimensionType() const;


    std::vector<std::string> labels() const;


    void labels(const std::vector<std::string> &labels);


    void labels(const none_t t);


    SetDimensionHDF5& operator=(const SetDimensionHDF5 &other);


    virtual ~SetDimensionHDF5();

};


class RangeDimensionHDF5 : virtual public base::IRangeDimension, public DimensionHDF5 {

public:

    RangeDimensionHDF5(Group group, size_t index);


    RangeDimensionHDF5(Group group, size_t index, std::vector<double> ticks);


    RangeDimensionHDF5(const RangeDimensionHDF5 &other);


    DimensionType dimensionType() const;


    boost::optional<std::string> label() const;


    void label(const std::string &label);


    void label(none_t t);


    boost::optional<std::string> unit() const;


    void unit(const std::string &unit);


    void unit(none_t t);


    std::vector<double> ticks() const;


    void ticks(const std::vector<double> &ticks);


    RangeDimensionHDF5& operator=(const RangeDimensionHDF5 &other);


    virtual ~RangeDimensionHDF5();

};


} // namespace hdf5
} // namespace nix

#endif // NIX_DIMENSIONS_HDF5_H
