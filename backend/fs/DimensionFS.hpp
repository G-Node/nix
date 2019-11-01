// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DIMENSIONFS_HPP
#define NIX_DIMENSIONFS_HPP

#include <nix/base/IDimensions.hpp>
#include <nix/base/IBlock.hpp>
#include <nix/base/IFile.hpp>
#include <nix/DataFrame.hpp>

#include "DirectoryWithAttributes.hpp"
#include "DataArrayFS.hpp"

#include <string>
#include <iostream>
#include <ctime>
#include <memory>

namespace nix {
namespace file {


DimensionType dimensionTypeFromStr(const std::string &str);


std::string dimensionTypeToStr(DimensionType dim);


std::shared_ptr<base::IDimension> openDimensionFS(const std::string &loc, FileMode mode);


class DimensionFS : virtual public base::IDimension, public DirectoryWithAttributes {


public:
    DimensionFS(const std::string &loc, FileMode mode);
	DimensionFS(const bfs::path &loc, FileMode mode);


    DimensionFS(const std::string &loc, size_t index, FileMode mode);
	DimensionFS(const bfs::path &loc, size_t index, FileMode mode);


    ndsize_t index() const;


    bool operator==(const DimensionFS &other) const;


    bool operator!=(const DimensionFS &other) const;


    virtual ~DimensionFS();

protected:

    void setType();

    void index(size_t);

};


class SampledDimensionFS : virtual public base::ISampledDimension, public DimensionFS {

public:

    SampledDimensionFS(const std::string &loc, FileMode mode);


    SampledDimensionFS(const std::string &loc, size_t index, FileMode mode);


    SampledDimensionFS(const std::string &loc, size_t index, double sampling_interval, FileMode mode);


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


    virtual ~SampledDimensionFS();

};


class SetDimensionFS : virtual public base::ISetDimension, public DimensionFS {

public:
    SetDimensionFS(const std::string &loc, FileMode mode);


    SetDimensionFS(const std::string &loc, size_t index, FileMode mode);


    DimensionType dimensionType() const;


    std::vector<std::string> labels() const;


    void labels(const std::vector<std::string> &labels);


    void labels(const none_t t);


    virtual ~SetDimensionFS();

};


class ColumnDimensionFS : virtual public base::IColumnDimension, public DimensionFS {

public:
    ColumnDimensionFS(const std::string &loc, FileMode mode)
        :DimensionFS(loc, mode){};


    ColumnDimensionFS(const std::string &loc, size_t index, FileMode mode)
        :DimensionFS(loc, index, mode){};

    DimensionType dimensionType() const {
        return DimensionType::Column;
    }

    std::vector<unsigned> columnIndices() const {
        std::vector<unsigned> indices;
        return indices;
    }

    Column column(unsigned col_index) const {
        nix::Column c;
        return c;
    }

    std::vector<std::string> labels() const {
        std::vector<std::string> labels;
        return labels;
    }

    std::vector<std::string> units() const {
        std::vector<std::string> units;
        return units;
    };

    std::vector<nix::DataType> columnDataTypes() const {
        std::vector<nix::DataType> dtypes;
        return dtypes;
    }

    std::shared_ptr<base::IDataFrame> dataFrame() const {
        //std::shared_ptr<base::IDataFrameFS> df; //FIXME DataFrameFS does not exist
        return nullptr;
    }

    virtual ~ColumnDimensionFS(){};

};


class RangeDimensionFS : virtual public base::IRangeDimension, public DimensionFS {

public:
    RangeDimensionFS(const std::string &loc, FileMode mode);


    RangeDimensionFS(const std::string &loc, size_t index, FileMode mode);


    RangeDimensionFS(const std::string &loc, size_t index, std::vector<double> ticks, FileMode mode);


    RangeDimensionFS(const std::string &loc, size_t index, const DataArrayFS &dataArray, FileMode mode); //FIXME


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


    virtual ~RangeDimensionFS();

private:

    DirectoryWithAttributes redirectGroup() const;
};


} // namespace file
} // namespace nix

#endif //NIX_DIMENSIONFS_HPP
