// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "DimensionHDF5.hpp"
#include <nix/util/util.hpp>

using namespace std;
using namespace nix::base;

namespace nix {
namespace hdf5 {

DimensionType dimensionTypeFromStr(const string &str) {
    if (str == "set") {
        return DimensionType::Set;
    } else if (str == "range") {
        return DimensionType::Range;
    } else if (str == "sample") {
        return DimensionType::Sample;
    } else if (str == "data_frame") {
        return DimensionType::DataFrame;
    } else {
        throw runtime_error("Not a valid dimension name");
    }
}


std::string dimensionTypeToStr(DimensionType dim) {

    //The way this switch + string.empty() checking is
    // done here might seem a bit convoluted, but the
    // idea behind it is:
    // a) have no default case in the switch to get a
    //    compile warning in case a new element is
    //    added to the enum
    // b) still be safe and throw an exception in case
    //    not all enum cases are handled properly

    std::string dimType;

    switch (dim) {
        case DimensionType::Set:
            dimType = "set";
            break;
        case DimensionType::Range:
            dimType = "range";
            break;
        case DimensionType::Sample:
            dimType = "sample";
            break;
        case DimensionType::DataFrame:
            dimType = "data_frame";
            break;
    }

    if (dimType.empty()) {
        throw runtime_error("Not a valid dimension type");
    }

    return dimType;
}


shared_ptr<IDimension> openDimensionHDF5(const H5Group &group, ndsize_t index) {
    string type_name;
    group.getAttr("dimension_type", type_name);

    DimensionType type = dimensionTypeFromStr(type_name);
    shared_ptr<IDimension> dim;

    switch (type) {
        case DimensionType::Set:
            dim = make_shared<SetDimensionHDF5>(group, index);
            break;
        case DimensionType::Range:
            dim = make_shared<RangeDimensionHDF5>(group, index);
            break;
        case DimensionType::Sample:
            dim = make_shared<SampledDimensionHDF5>(group, index);
            break;
        case DimensionType::DataFrame:
            dim = make_shared<DataFrameDimensionHDF5>(group, index);
            break;
    }

    return dim;
}


// Implementation of Dimension

DimensionHDF5::DimensionHDF5(const H5Group &group, ndsize_t index)
    : group(group), dim_index(index)
{
}


void DimensionHDF5::setType() {
    if (!group.hasAttr("dimension_type")) {
        group.setAttr("dimension_type", dimensionTypeToStr(dimensionType()));
    }
}


bool DimensionHDF5::operator==(const DimensionHDF5 &other) const {
    return group == other.group;
}


bool DimensionHDF5::operator!=(const DimensionHDF5 &other) const {
    return !(*this == other);
}


DimensionHDF5::~DimensionHDF5() {}

//--------------------------------------------------------------
// Implementation of SampledDimension
//--------------------------------------------------------------

SampledDimensionHDF5::SampledDimensionHDF5(const H5Group &group, ndsize_t index)
    : DimensionHDF5(group, index)
{
    setType();
}

SampledDimensionHDF5::SampledDimensionHDF5(const H5Group &group, ndsize_t index, double sampling_interval)
    : SampledDimensionHDF5(group, index)
{
    setType();
    this->samplingInterval(sampling_interval);
}


DimensionType SampledDimensionHDF5::dimensionType() const {
    return DimensionType::Sample;
}


boost::optional<std::string> SampledDimensionHDF5::label() const {
    boost::optional<std::string> ret;
    string label;
    bool have_attr = group.getAttr("label", label);
    if (have_attr) {
        ret = label;
    }
    return ret;
}


void SampledDimensionHDF5::label(const string &label) {
    group.setAttr("label", label);
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


void SampledDimensionHDF5::label(const none_t t) {
    if (group.hasAttr("label")) {
        group.removeAttr("label");
    }
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


boost::optional<std::string> SampledDimensionHDF5::unit() const {
    boost::optional<std::string> ret;
    string unit;
    bool have_attr = group.getAttr("unit", unit);
    if (have_attr) {
        ret = unit;
    }
    return ret;
}


void SampledDimensionHDF5::unit(const string &unit) {
    group.setAttr("unit", unit);
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


void SampledDimensionHDF5::unit(const none_t t) {
    if (group.hasAttr("unit")) {
        group.removeAttr("unit");
    }
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


double SampledDimensionHDF5::samplingInterval() const {
    double sampling_interval;

    if (group.hasAttr("sampling_interval")) {
        group.getAttr("sampling_interval", sampling_interval);
        return sampling_interval;
    } else {
        throw MissingAttr("sampling_interval");
    }
}


void SampledDimensionHDF5::samplingInterval(double sampling_interval) {
    group.setAttr("sampling_interval", sampling_interval);
}


boost::optional<double> SampledDimensionHDF5::offset() const {
    boost::optional<double> ret;
    double offset = 0;
    if (group.getAttr("offset", offset)) {
        ret = offset;
    }
    return ret;
}


void SampledDimensionHDF5::offset(double offset) {
    group.setAttr("offset", offset);
}


void SampledDimensionHDF5::offset(const none_t t) {
    if (group.hasAttr("offset")) {
        group.removeAttr("offset");
    }
}


SampledDimensionHDF5::~SampledDimensionHDF5() {}

//--------------------------------------------------------------
// Implementation of SetDimensionHDF5
//--------------------------------------------------------------

SetDimensionHDF5::SetDimensionHDF5(const H5Group &group, ndsize_t index)
    : DimensionHDF5(group, index)
{
    setType();
}


DimensionType SetDimensionHDF5::dimensionType() const {
    return DimensionType::Set;
}


vector<string> SetDimensionHDF5::labels() const {
    vector<string> labels;

    group.getData("labels", labels);
    return labels;
}


void SetDimensionHDF5::labels(const vector<string> &labels) {
   group.setData("labels", labels);
}

void SetDimensionHDF5::labels(const none_t t) {
    if (group.hasData("labels")) {
        group.removeData("labels");
    }
}

SetDimensionHDF5::~SetDimensionHDF5() {}

//--------------------------------------------------------------
// Implementation of DataFrameDimensionHDF5
//--------------------------------------------------------------
DataFrameDimensionHDF5::DataFrameDimensionHDF5(const H5Group &group, ndsize_t index)
    : DimensionHDF5(group, index)
{
    setType();
}

DataFrameDimensionHDF5::DataFrameDimensionHDF5(const H5Group &group, ndsize_t index,
                                               const std::shared_ptr<IFile> &file,
                                               const std::shared_ptr<IBlock> &block)
    : DimensionHDF5(group, index), entity_block(block), entity_file(file)
{
    setType();
}

DataFrameDimensionHDF5::DataFrameDimensionHDF5(const H5Group &group, ndsize_t index,
                                               const std::shared_ptr<IFile> &file,
                                               const std::shared_ptr<IBlock> &block, const DataFrame &frame)
    :DataFrameDimensionHDF5(group, index, file, block)
{
    std::shared_ptr<IDataFrame> idf = block->getEntity<IDataFrame>(frame.id());
    if (!idf)
        throw std::runtime_error("DataFrameDimensionHDF5 DataFrame not found in block!");
    if (this->group.hasGroup("data_frame"))
        this->group.removeGroup("data_frame");
    auto target = std::dynamic_pointer_cast<DataFrameHDF5>(idf);

    setType();
    this->group.createLink(target->group(), "data_frame");
}

DataFrameDimensionHDF5::DataFrameDimensionHDF5(const H5Group &group, ndsize_t index,
                                               const std::shared_ptr<IFile> &file,
                                               const std::shared_ptr<IBlock> &block, const DataFrame &frame,
                                               unsigned col_index)
    :DataFrameDimensionHDF5(group, index, file, block, frame)
{
    this->group.setAttr("column_index", col_index);
}

int DataFrameDimensionHDF5::columnIndex() const {
    int col_index = -1;
    if (group.hasAttr("column_index")) {
        group.getAttr("column_index", col_index);
    }
    return col_index;
}

int DataFrameDimensionHDF5::checkColumnIndex(int col_index) const {
    int column_index = col_index;
    if (col_index == -1) {
        column_index = columnIndex();
    }

    if (column_index == -1) {
        throw nix::OutOfBounds("DataFrameDimensionHDF5: Error accessing column, no column index was given and no default is specified.");
    }

    nix::DataFrame df = dataFrame();
    std::vector<Column> cols = df.columns();
    if (static_cast<size_t>(column_index) >= cols.size()) {
        throw nix::OutOfBounds("DataFrameDimensionHDF5: Error accessing column, column index exceeds number of columns!");
    }

    return column_index;
}


DimensionType DataFrameDimensionHDF5::dimensionType() const {
    return DimensionType::DataFrame;
}


std::string DataFrameDimensionHDF5::unit(int col_index) const {
    int column_index = checkColumnIndex(col_index);

    nix::DataFrame df = dataFrame();
    std::vector<Column> cols = df.columns();
    return cols[column_index].unit;
}


std::string DataFrameDimensionHDF5::label(int col_index) const {
    int column_index;
    nix::DataFrame df = dataFrame();
    try {
        column_index = checkColumnIndex(col_index);
    } catch (const nix::OutOfBounds& e) {
        if (col_index == -1)
            return df.name();
        else
            throw e;
    }
    std::vector<unsigned> indices = {static_cast<unsigned>(column_index)};
    std::vector<std::string> names = df.colName(indices);
    return names[0];
}


Column DataFrameDimensionHDF5::column(int col_index) const {
    int column_index = checkColumnIndex(col_index);

    nix::DataFrame df = dataFrame();
    std::vector<Column> cols = df.columns();
    return cols[column_index];
}


nix::DataType DataFrameDimensionHDF5::columnDataType(int col_index) const {
    int column_index = checkColumnIndex(col_index);

    nix::DataFrame df = dataFrame();
    std::vector<Column> all_cols = df.columns();
    return all_cols[column_index].dtype;
}


std::shared_ptr<base::IDataFrame> DataFrameDimensionHDF5::dataFrame() const {
    std::shared_ptr<DataFrameHDF5> df;
    bool error = false;

    if (this->group.hasGroup("data_frame")) {
        H5Group other_group = this->group.openGroup("data_frame", false);
        df = std::make_shared<DataFrameHDF5>(entity_file, entity_block, other_group);
    } else error = true;
    if (error)
        throw std::runtime_error("DataFrameDimensionHDF5::dataFrame: DataFrame not found!");

    return df;
}


DataFrameDimensionHDF5::~DataFrameDimensionHDF5() {}


//--------------------------------------------------------------
// Implementation of RangeDimensionHDF5
//--------------------------------------------------------------

RangeDimensionHDF5::RangeDimensionHDF5(const H5Group &group, ndsize_t index)
    : DimensionHDF5(group, index)
{
    setType();
}


RangeDimensionHDF5::RangeDimensionHDF5(const H5Group &group, ndsize_t index, vector<double> ticks)
    : RangeDimensionHDF5(group, index)
{
    setType();
    this->ticks(ticks);
}


RangeDimensionHDF5::RangeDimensionHDF5(const H5Group &group, ndsize_t index, const DataArrayHDF5 &array)
    :RangeDimensionHDF5(group, index)
{
    setType();
    this->group.createLink(array.group(), array.id());
}


DimensionType RangeDimensionHDF5::dimensionType() const {
    return DimensionType::Range;
}


H5Group RangeDimensionHDF5::redirectGroup() const {
    H5Group g;
    if (alias()) {
        string group_name = group.objectName(0);
        g = group.openGroup(group_name, false);
    } else {
        g = group;
    }
    return g;
}


boost::optional<std::string> RangeDimensionHDF5::label() const {
    boost::optional<std::string> ret;
    string label;
    H5Group g = redirectGroup();
    bool have_attr = g.getAttr("label", label);
    if (have_attr) {
        ret = label;
    }
    return ret;
}


void RangeDimensionHDF5::label(const string &label) {
    H5Group g = redirectGroup();
    g.setAttr("label", label);
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


void RangeDimensionHDF5::label(const none_t t) {
    H5Group g = redirectGroup();
    if (g.hasAttr("label")) {
        g.removeAttr("label");
    }
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


boost::optional<std::string> RangeDimensionHDF5::unit() const {
    boost::optional<std::string> ret;
    string unit;
    H5Group g = redirectGroup();
    bool have_attr = g.getAttr("unit", unit);
    if (have_attr) {
        ret = unit;
    }
    return ret;
}


void RangeDimensionHDF5::unit(const string &unit) {
    H5Group g = redirectGroup();
    g.setAttr("unit", unit);
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


void RangeDimensionHDF5::unit(const none_t t) {
    H5Group g = redirectGroup();
    if (g.hasAttr("unit")) {
        g.removeAttr("unit");
    }
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


bool RangeDimensionHDF5::alias() const {
    return group.objectCount() > 0 && !group.hasData("ticks");
}


vector<double> RangeDimensionHDF5::ticks() const {
    vector<double> ticks;
    H5Group g = redirectGroup();
    if (g.hasData("ticks")) {
        g.getData("ticks", ticks);
        return ticks;
    } else if (g.hasData("data")) {
        g.getData("data", ticks);
        return ticks;
    } else {
        throw MissingAttr("ticks");
    }
}


vector<double> RangeDimensionHDF5::ticks(ndsize_t start, size_t count) const {
    vector<double> ticks;
    if (count > ticks.max_size()) {
        throw nix::OutOfBounds("count exceeds the maximum size of std::vector!");
    }
    ticks.resize(count);

    H5Group g = redirectGroup();
    std::string dset_name = g.hasData("ticks") ? "ticks" : "data";
    DataSet ds = g.openData(dset_name);
    NDSize s = ds.size();
    if (start > s[0] || count > s[0] || (start + count) > s[0]) {
        throw nix::OutOfBounds("Access to RangeDimensionHDF5::ticks: start is out of Bounds!");
    }
    h5x::DataType memType = data_type_to_h5_memtype(nix::DataType::Double);
    DataSpace fileSpace, memSpace;
    nix::NDSize offst(1, start);
    nix::NDSize cnt(1, count);
    std::tie(memSpace, fileSpace) = ds.offsetCount2DataSpaces(cnt, offst);
    ds.read(ticks.data(),  memType, memSpace, fileSpace);
    return ticks;
}


void RangeDimensionHDF5::ticks(const vector<double> &ticks) {
    H5Group g = redirectGroup();
    if (!alias()) {
        g.setData("ticks", ticks);
    } else if (g.hasData("data")) {
        NDSize extent(1, ticks.size());
        DataSet ds = g.openData("data");
        ds.setExtent(extent);
        ds.write(ticks);
    } else {
        throw MissingAttr("ticks");
    }
}

RangeDimensionHDF5::~RangeDimensionHDF5() {}

} // ns nix::hdf5
} // ns nix
