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
#include <nix/base/IBlock.hpp>
#include <nix/base/IFile.hpp>
#include <nix/DataFrame.hpp>

#include "h5x/H5Group.hpp"
#include "DataArrayHDF5.hpp"
#include "DataFrameHDF5.hpp"

#include <string>
#include <iostream>
#include <ctime>
#include <memory>

namespace nix {
namespace hdf5 {


DimensionType dimensionTypeFromStr(const std::string &str);


std::string dimensionTypeToStr(DimensionType dim);


std::shared_ptr<base::IDimension> openDimensionHDF5(const H5Group &group, ndsize_t index);


class DimensionHDF5 : virtual public base::IDimension {

protected:

    H5Group group;
    ndsize_t dim_index;

public:

    DimensionHDF5(const H5Group &group, ndsize_t index);


    ndsize_t index() const { return dim_index; }


    bool operator==(const DimensionHDF5 &other) const;


    bool operator!=(const DimensionHDF5 &other) const;


    virtual ~DimensionHDF5();

protected:

    void setType();

};


class SampledDimensionHDF5 : virtual public base::ISampledDimension, public DimensionHDF5 {

public:

    SampledDimensionHDF5(const H5Group &group, ndsize_t index);


    SampledDimensionHDF5(const H5Group &group, ndsize_t index, double sampling_interval);


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

    SetDimensionHDF5(const H5Group &group, ndsize_t index);


    DimensionType dimensionType() const;


    std::vector<std::string> labels() const;


    void labels(const std::vector<std::string> &labels);


    void labels(const none_t t);


    virtual ~SetDimensionHDF5();

};

class ColumnDimensionHDF5 : virtual public base::IColumnDimension, public DimensionHDF5 {

private:
    std::shared_ptr<base::IBlock> entity_block;
    std::shared_ptr<base::IFile>  entity_file;

public:
    ColumnDimensionHDF5(const H5Group &group, ndsize_t index);

    ColumnDimensionHDF5(const H5Group &group, ndsize_t index, const std::shared_ptr<nix::base::IFile> &file,
                        const std::shared_ptr<nix::base::IBlock> &block);

    ColumnDimensionHDF5(const H5Group &group, ndsize_t index, const std::shared_ptr<nix::base::IFile> &file,
                        const std::shared_ptr<nix::base::IBlock> &block, const DataFrame &frame,
                        std::vector<unsigned> column_indices);

    DimensionType dimensionType() const;

    std::vector<unsigned> columnIndices() const;

    Column column(unsigned col_index) const;

    std::vector<std::string> labels() const;

    std::vector<std::string> units() const;

    std::vector<nix::DataType> columnDataTypes() const;

    std::shared_ptr<base::IDataFrame> dataFrame() const;

    virtual ~ColumnDimensionHDF5();

};

class RangeDimensionHDF5 : virtual public base::IRangeDimension, public DimensionHDF5 {

public:

    RangeDimensionHDF5(const H5Group &group, ndsize_t index);


    RangeDimensionHDF5(const H5Group &group, ndsize_t index, std::vector<double> ticks);


    RangeDimensionHDF5(const H5Group &group, ndsize_t index, const DataArrayHDF5 &dataArray);


    DimensionType dimensionType() const;


    bool alias() const;


    boost::optional<std::string> label() const;


    void label(const std::string &label);


    void label(const none_t t);


    boost::optional<std::string> unit() const;


    void unit(const std::string &unit);


    void unit(const none_t t);


    std::vector<double> ticks() const;


    std::vector<double> ticks(ndsize_t start, size_t count) const;


    void ticks(const std::vector<double> &ticks);


    virtual ~RangeDimensionHDF5();

private:

    H5Group redirectGroup() const;
};


} // namespace hdf5
} // namespace nix

#endif // NIX_DIMENSIONS_HDF5_H
