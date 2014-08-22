// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>

#include <nix/hdf5/DimensionHDF5.hpp>
#include <nix/hdf5/DataSet.hpp>

using namespace std;
using namespace nix;
using namespace nix::hdf5;
using namespace nix::base;


DimensionType nix::hdf5::dimensionTypeFromStr(const string &str) {
    if (str == "set") {
        return DimensionType::Set;
    } else if (str == "range") {
        return DimensionType::Range;
    } else if (str == "sample") {
        return DimensionType::Sample;
    } else {
        throw runtime_error("Not a valid dimension name");
    }
}


std::string nix::hdf5::dimensionTypeToStr(DimensionType dim) {

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
        throw runtime_error("Not a valid dimension type");
    }

    return dimType;
}


shared_ptr<IDimension> nix::hdf5::openDimensionHDF5(const Group &group, size_t index) {
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
    }

    return dim;
}


// Implementation of Dimension

DimensionHDF5::DimensionHDF5(const Group &group, size_t index)
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

SampledDimensionHDF5::SampledDimensionHDF5(const Group &group, size_t index)
    : DimensionHDF5(group, index)
{
}

SampledDimensionHDF5::SampledDimensionHDF5(const Group &group, size_t index, double sampling_interval)
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
    if (label.empty()) {
        throw EmptyString("label");
    } else {
        group.setAttr("label", label);
        // NOTE: forceUpdatedAt() not possible since not reachable from here
    }
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
    if (unit.empty()) {
        throw EmptyString("unit");
    } else {
        group.setAttr("unit", unit);
        // NOTE: forceUpdatedAt() not possible since not reachable from here
    }
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

SetDimensionHDF5::SetDimensionHDF5(const Group &group, size_t index)
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
    if (group.hasAttr("offset")) {
        group.removeAttr("offset");
    }
}

SetDimensionHDF5::~SetDimensionHDF5() {}

//--------------------------------------------------------------
// Implementation of RangeDimensionHDF5
//--------------------------------------------------------------

RangeDimensionHDF5::RangeDimensionHDF5(const Group &group, size_t index)
    : DimensionHDF5(group, index)
{
}


RangeDimensionHDF5::RangeDimensionHDF5(const Group &group, size_t index, vector<double> ticks)
    : RangeDimensionHDF5(group, index)
{
    setType();
    this->ticks(ticks);
}


DimensionType RangeDimensionHDF5::dimensionType() const {
    return DimensionType::Range;
}


boost::optional<std::string> RangeDimensionHDF5::label() const {
    boost::optional<std::string> ret;
    string label;
    bool have_attr = group.getAttr("label", label);
    if (have_attr) {
        ret = label;
    }
    return ret;
}


void RangeDimensionHDF5::label(const string &label) {
    if (label.empty()) {
        throw EmptyString("label");
    } else {
        group.setAttr("label", label);
        // NOTE: forceUpdatedAt() not possible since not reachable from here
    }
}


void RangeDimensionHDF5::label(const none_t t) {
    if (group.hasAttr("label")) {
        group.removeAttr("label");
    }
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


boost::optional<std::string> RangeDimensionHDF5::unit() const {
    boost::optional<std::string> ret;
    string unit;
    bool have_attr = group.getAttr("unit", unit);
    if (have_attr) {
        ret = unit;
    }
    return ret;
}


void RangeDimensionHDF5::unit(const string &unit) {
    if (unit.empty()) {
        throw EmptyString("unit");
    } else {
        group.setAttr("unit", unit);
        // NOTE: forceUpdatedAt() not possible since not reachable from here
    }
}


void RangeDimensionHDF5::unit(const none_t t) {
    if (group.hasAttr("unit")) {
        group.removeAttr("unit");
    }
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


vector<double> RangeDimensionHDF5::ticks() const {
    vector<double> ticks;

    if (group.hasData("ticks")) {
        group.getData("ticks", ticks);
        return ticks;
    } else {
        throw MissingAttr("ticks");
    }
}


void RangeDimensionHDF5::ticks(const vector<double> &ticks) {
    group.setData("ticks", ticks);
}

RangeDimensionHDF5::~RangeDimensionHDF5() {}

