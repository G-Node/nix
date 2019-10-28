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
    } else if (str == "column") {
        return DimensionType::Column;
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
        case DimensionType::Column:
            dimType = "column";
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
        case DimensionType::Column:
            dim = make_shared<ColumnDimensionHDF5>(group, index);
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
    if (!group.hasAttr("dimension_type"))
        group.setAttr("dimension_type", dimensionTypeToStr(dimensionType()));
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
// Implementation of ColumnDimensionHDF5
//--------------------------------------------------------------
ColumnDimensionHDF5::ColumnDimensionHDF5(const H5Group &group, ndsize_t index)
    : DimensionHDF5(group, index)
{
    setType();
    // Note: maybe having this is a bad idea, because a ColumnDimension created
    // this way will not have the block and file
}

ColumnDimensionHDF5::ColumnDimensionHDF5(const H5Group &group, ndsize_t index,  const std::shared_ptr<IFile> &file,
                                         const std::shared_ptr<IBlock> &block)
    : DimensionHDF5(group, index), entity_block(block), entity_file(file)
{
    setType();
}

ColumnDimensionHDF5::ColumnDimensionHDF5(const H5Group &group, ndsize_t index, const std::shared_ptr<IFile> &file,
                                         const std::shared_ptr<IBlock> &block, const DataFrame &frame,
                                         unsigned col_index)
    :ColumnDimensionHDF5(group, index, file, block)
{
    setType();
    this->group.setAttr("column_index", col_index);
    std::shared_ptr<IDataFrame> idf = block->getEntity<IDataFrame>(frame.id());
    if (!idf)
        throw std::runtime_error("ColumnDimensionHDF5 DataFrame not found in block!");
    if (this->group.hasGroup("data_frame"))
        this->group.removeGroup("data_frame");

    auto target = std::dynamic_pointer_cast<DataFrameHDF5>(idf);

    this->group.createLink(target->group(), "data_frame");
}

unsigned ColumnDimensionHDF5::columnIndex() const {
    unsigned col_idx;
    if (group.hasAttr("column_index")) {
        group.getAttr("column_index", col_idx);
        return col_idx;
    } else {
        throw MissingAttr("column_index");
    }
}

DimensionType ColumnDimensionHDF5::dimensionType() const {
    return DimensionType::Column;
}

boost::optional<std::string> ColumnDimensionHDF5::unit() const {
    boost::optional<std::string> ret;
    std::string u = this->dataFrame()->columns()[this->columnIndex()].unit;
    if (u != "")
        ret = u;
    return ret;
}

boost::optional<std::string> ColumnDimensionHDF5::label() const {
    boost::optional<std::string> ret;
    std::string n = this->dataFrame()->colName(this->columnIndex());
    if (n != "")
        ret = n;
    return ret;
}

std::vector<Variant> ColumnDimensionHDF5::ticks() const {
    std::vector<Variant> tcks;
    return tcks;
}

Column ColumnDimensionHDF5::column() const {
    std::vector<nix::Column> cols = this->dataFrame()->columns();
    return cols[this->columnIndex()];
}

std::shared_ptr<base::IDataFrame> ColumnDimensionHDF5::dataFrame() const {
    std::shared_ptr<DataFrameHDF5> df;
    bool error = false;

    if (this->group.hasGroup("data_frame")) {
        H5Group other_group = this->group.openGroup("data_frame", false);
        df = std::make_shared<DataFrameHDF5>(entity_file, entity_block, other_group);
    } else error = true;
    if (error)
        throw std::runtime_error("ColumnDimensionHDF5::dataFrame: DataFrame not found!");

    return df;
}


ColumnDimensionHDF5::~ColumnDimensionHDF5() {}


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
        throw nix::OutOfBounds("Access to RangeDimension::ticks: start is out of Bounds!");
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
