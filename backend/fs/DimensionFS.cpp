// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "DimensionFS.hpp"

using namespace nix::base;

namespace nix {
namespace file {

DimensionType dimensionTypeFromStr(const std::string &str) {
    if (str == "set") {
        return DimensionType::Set;
    } else if (str == "range") {
        return DimensionType::Range;
    } else if (str == "sample") {
        return DimensionType::Sample;
    } else {
        throw std::runtime_error("Not a valid dimension name");
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
    }

    if (dimType.empty()) {
        throw std::runtime_error("Not a valid dimension type");
    }

    return dimType;
}


std::shared_ptr<IDimension> openDimensionFS(const std::string &loc, FileMode mode) {
    AttributesFS attr(loc, mode);
    std::string type_name;
    attr.get("dimension_type", type_name);

    DimensionType type = dimensionTypeFromStr(type_name);
    std::shared_ptr<IDimension> dim;

    switch (type) {
        case DimensionType::Set:
            dim = std::make_shared<SetDimensionFS>(loc, mode);
            break;
        case DimensionType::Range:
            dim = std::make_shared<RangeDimensionFS>(loc, mode);
            break;
        case DimensionType::Sample:
            dim = std::make_shared<SampledDimensionFS>(loc, mode);
            break;
    }
    return dim;
}


// Implementation of Dimension
DimensionFS::DimensionFS(const std::string &loc, FileMode mode)
    : DimensionFS(bfs::path(loc), mode)
{
}

DimensionFS::DimensionFS(const bfs::path &loc, FileMode mode)
	: DirectoryWithAttributes(loc, mode)
{
}

DimensionFS::DimensionFS(const bfs::path &loc, size_t index, FileMode mode)
    : DirectoryWithAttributes(bfs::path(loc) / bfs::path(util::numToStr(index)), mode)
{
    this->index(index);
}

DimensionFS::DimensionFS(const std::string &loc, size_t index, FileMode mode) 
	: DimensionFS(bfs::path(loc), index, mode) {
}

void DimensionFS::index(size_t index) {
    setAttr("index", index);
}

ndsize_t DimensionFS::index() const {
    size_t idx;
    if (!hasAttr("index"))
        throw MissingAttr("MissingAttr index in Dimension");
    getAttr("index", idx);
    return idx;
}

void DimensionFS::setType() {
    setAttr("dimension_type", dimensionTypeToStr(dimensionType()));
}


bool DimensionFS::operator==(const DimensionFS &other) const {
    return location() == other.location();
}


bool DimensionFS::operator!=(const DimensionFS &other) const {
    return !(*this == other);
}


DimensionFS::~DimensionFS() {}

//--------------------------------------------------------------
// Implementation of SampledDimension
//--------------------------------------------------------------

SampledDimensionFS::SampledDimensionFS(const std::string &loc, FileMode mode)
    : DimensionFS(loc, mode)
{
}

SampledDimensionFS::SampledDimensionFS(const std::string &loc, size_t index, FileMode mode)
    : DimensionFS(loc, index, mode) {
}

SampledDimensionFS::SampledDimensionFS(const std::string &loc, size_t index, double sampling_interval, FileMode mode)
    : SampledDimensionFS(loc, index, mode)
{
    setType();
    this->samplingInterval(sampling_interval);
}


DimensionType SampledDimensionFS::dimensionType() const {
    return DimensionType::Sample;
}


boost::optional<std::string> SampledDimensionFS::label() const {
    boost::optional<std::string> ret;
    std::string label;
    if (hasAttr("label")) {
        getAttr("label", label);
        ret = label;
    }
    return ret;
}


void SampledDimensionFS::label(const std::string &label) {
    if (label.empty()) {
        throw EmptyString("label");
    } else {
        setAttr("label", label);
        // NOTE: forceUpdatedAt() not possible since not reachable from here
    }
}


void SampledDimensionFS::label(const none_t t) {
    if (hasAttr("label")) {
        removeAttr("label");
    }
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


boost::optional<std::string> SampledDimensionFS::unit() const {
    boost::optional<std::string> ret;
    std::string unit;
    if (hasAttr("unit")) {
        getAttr("unit", unit);
        ret = unit;
    }
    return ret;
}


void SampledDimensionFS::unit(const std::string &unit) {
    if (unit.empty()) {
        throw EmptyString("unit");
    } else {
        setAttr("unit", unit);
        // NOTE: forceUpdatedAt() not possible since not reachable from here
    }
}


void SampledDimensionFS::unit(const none_t t) {
    if (hasAttr("unit")) {
        removeAttr("unit");
    }
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


double SampledDimensionFS::samplingInterval() const {
    double sampling_interval;
    if (hasAttr("sampling_interval")) {
        getAttr("sampling_interval", sampling_interval);
        return sampling_interval;
    } else {
        throw MissingAttr("sampling_interval");
    }
}


void SampledDimensionFS::samplingInterval(double sampling_interval) {
    setAttr("sampling_interval", sampling_interval);
}


boost::optional<double> SampledDimensionFS::offset() const {
    boost::optional<double> ret;
    double offset = 0;
    if (hasAttr("offset")) {
        getAttr("offset", offset);
        ret = offset;
    }
    return ret;
}


void SampledDimensionFS::offset(double offset) {
    setAttr("offset", offset);
}


void SampledDimensionFS::offset(const none_t t) {
    if (hasAttr("offset")) {
        removeAttr("offset");
    }
}


SampledDimensionFS::~SampledDimensionFS() {}

//--------------------------------------------------------------
// Implementation of SetDimensionHDF5
//--------------------------------------------------------------
SetDimensionFS::SetDimensionFS(const std::string &loc, FileMode mode)
    : DimensionFS(loc, mode)
{
}


SetDimensionFS::SetDimensionFS(const std::string &loc, size_t index, FileMode mode)
    : DimensionFS(loc, index, mode)
{
    setType();
}


DimensionType SetDimensionFS::dimensionType() const {
    return DimensionType::Set;
}


std::vector<std::string> SetDimensionFS::labels() const {
    std::vector<std::string> labels;
    getAttr("labels", labels);
    return labels;
}


void SetDimensionFS::labels(const std::vector<std::string> &labels) {
    setAttr("labels", labels);
}

void SetDimensionFS::labels(const none_t t) {
    if (hasAttr("labels")) {
        removeAttr("labels");
    }
}

SetDimensionFS::~SetDimensionFS() {}

//--------------------------------------------------------------
// Implementation of RangeDimensionHDF5
//--------------------------------------------------------------
RangeDimensionFS::RangeDimensionFS(const std::string &loc, FileMode mode)
    : DimensionFS(loc, mode)
{
}


RangeDimensionFS::RangeDimensionFS(const std::string &loc, size_t index, FileMode mode)
    : DimensionFS(loc, index, mode)
{
}


RangeDimensionFS::RangeDimensionFS(const std::string &loc, size_t index, std::vector<double> ticks, FileMode mode)
    : RangeDimensionFS(loc, index, mode)
{
    setType();
    this->ticks(ticks);
}


RangeDimensionFS::RangeDimensionFS(const std::string &loc, size_t index, const DataArrayFS &array, FileMode mode)
    :RangeDimensionFS(loc, index, mode)
{
    setType();
    createDirectoryLink(array.location(), "data");
}


DimensionType RangeDimensionFS::dimensionType() const {
    return DimensionType::Range;
}


DirectoryWithAttributes RangeDimensionFS::redirectGroup() const {
    boost::filesystem::path l(location());
    if (alias()) {
        return DirectoryWithAttributes(l / boost::filesystem::path("data"), fileMode());
    } else {
        return DirectoryWithAttributes(l, fileMode());
    }
}


boost::optional<std::string> RangeDimensionFS::label() const {
    boost::optional<std::string> ret;
    std::string label;
    DirectoryWithAttributes d = redirectGroup();
    if (d.hasAttr("label")) {
        d.getAttr("label", label);
        ret = label;
    }
    return ret;
}


void RangeDimensionFS::label(const std::string &label) {
    if (label.empty()) {
        throw EmptyString("label");
    }
    DirectoryWithAttributes d = redirectGroup();
    d.setAttr("label", label);
}


void RangeDimensionFS::label(const none_t t) {
    DirectoryWithAttributes d = redirectGroup();
    if (d.hasAttr("label")) {
        d.removeAttr("label");
    }
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


boost::optional<std::string> RangeDimensionFS::unit() const {
    boost::optional<std::string> ret;
    std::string unit;
    DirectoryWithAttributes d = redirectGroup();
    if (d.hasAttr("unit")) {
        d.getAttr("unit", unit);
        ret = unit;
    }
    return ret;
}


void RangeDimensionFS::unit(const std::string &unit) {
    if (unit.empty()) {
        throw EmptyString("unit");
    }
    DirectoryWithAttributes d = redirectGroup();
    d.setAttr("unit", unit);
}


void RangeDimensionFS::unit(const none_t t) {
    DirectoryWithAttributes d = redirectGroup();
    if (d.hasAttr("unit")) {
        d.removeAttr("unit");
    }
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


bool RangeDimensionFS::alias() const {
    return subdirCount() > 0 && !hasObject("ticks");
}


std::vector<double> RangeDimensionFS::ticks() const {
    std::vector<double> ticks;
    /*
    Group g = redirectGroup();
    if (g.hasData("ticks")) {
        g.getData("ticks", ticks);
        return ticks;
    } else if (g.hasData("data")) {
        g.getData("data", ticks);
        return ticks;
    } else {
        throw MissingAttr("ticks");
    }
    */ // FIXME
    return ticks;
}


void RangeDimensionFS::ticks(const std::vector<double> &ticks) {
    /*
    Group g = redirectGroup();
    if (!alias()) {
        g.setData("ticks", ticks);
    } else if (g.hasData("data")) {
        NDSize extent(1, ticks.size());
        DataSet ds = g.openData("data");
        ds.setExtent(extent);
        ds.write(nix::DataType::Double, extent, ticks.data());
    } else {
        throw MissingAttr("ticks");
    }
    */ //FIXME
}

RangeDimensionFS::~RangeDimensionFS() {}

} // ns nix::file
} // ns nix

