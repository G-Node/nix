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

namespace nix {
namespace hdf5 {


DimensionType dimensionTypeFromStr(const string &str) {
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
        throw runtime_error("Not a valid dimension type");
    }

    return dimType;

}

// Implementation of Dimension

DimensionHDF5::DimensionHDF5(Group group, size_t id)
    : group(group), dim_id(id)
{
}


DimensionHDF5::DimensionHDF5(const DimensionHDF5 &other)
    : group(other.group), dim_id(other.dim_id)
{
}


void DimensionHDF5::setType() {
    group.setAttr("dimension_type", dimensionTypeToStr(dimensionType()));
}


void DimensionHDF5::swap(DimensionHDF5 &other) {
    using std::swap;
    swap(group, other.group);
    swap(dim_id, other.dim_id);
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

SampledDimensionHDF5::SampledDimensionHDF5(Group group, size_t id, double _samplingInterval)
    : DimensionHDF5(group, id)
{
    setType();
    samplingInterval(_samplingInterval);
}


SampledDimensionHDF5::SampledDimensionHDF5(const SampledDimensionHDF5 &other)
    : DimensionHDF5(other.group, other.dim_id)
{
    setType();
    samplingInterval(other.samplingInterval());
}


DimensionType SampledDimensionHDF5::dimensionType() const {
    return DimensionType::Sample;
}


boost::optional<std::string> SampledDimensionHDF5::label() const {
    boost::optional<std::string> ret;
    string label;
    group.getAttr("label", label);
    ret = label;
    return ret;
}


void SampledDimensionHDF5::label(const string &label) {
    if(label.empty()) {
        throw EmptyString("label");
    }
    else {
        group.setAttr("label", label);
        // NOTE: forceUpdatedAt() not possible since not reachable from here
    }
}


void SampledDimensionHDF5::label(const none_t t) {
    if(group.hasAttr("label")) {
        group.removeAttr("label");
    }
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


boost::optional<std::string> SampledDimensionHDF5::unit() const {
    boost::optional<std::string> ret;
    string unit;
    group.getAttr("unit", unit);
    ret = unit;
    return ret;
}


void SampledDimensionHDF5::unit(const string &unit) {
    if(unit.empty()) {
        throw EmptyString("unit");
    }
    else {
        group.setAttr("unit", unit);
        // NOTE: forceUpdatedAt() not possible since not reachable from here
    }
}


void SampledDimensionHDF5::unit(const none_t t) {
    if(group.hasAttr("unit")) {
        group.removeAttr("unit");
    }
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


double SampledDimensionHDF5::samplingInterval() const {
    double sampling_interval;
    
    if(group.hasAttr("sampling_interval")) {
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
    if(group.getAttr("offset", offset)) {
        ret = offset;
    }
    return ret;
}


void SampledDimensionHDF5::offset(double offset) {
    group.setAttr("offset", offset);
}


void SampledDimensionHDF5::offset(const none_t t) {
    if(group.hasAttr("offset")) {
        group.removeAttr("offset");
    }
}


SampledDimensionHDF5& SampledDimensionHDF5::operator=(const SampledDimensionHDF5 &other) {
    SampledDimensionHDF5 tmp(other);
    swap(tmp);
    return *this;
}


SampledDimensionHDF5::~SampledDimensionHDF5() {}

//--------------------------------------------------------------
// Implementation of SetDimensionHDF5
//--------------------------------------------------------------

SetDimensionHDF5::SetDimensionHDF5(Group group, size_t id)
    : DimensionHDF5(group, id)
{
    setType();
}


SetDimensionHDF5::SetDimensionHDF5(const SetDimensionHDF5 &other)
    : DimensionHDF5(other.group, other.dim_id)
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
    if(group.hasAttr("offset")) {
        group.removeAttr("offset");
    }
}

SetDimensionHDF5& SetDimensionHDF5::operator=(const SetDimensionHDF5 &other) {
    SetDimensionHDF5 tmp(other);
    swap(tmp);
    return *this;
}


SetDimensionHDF5::~SetDimensionHDF5() {}

//--------------------------------------------------------------
// Implementation of RangeDimensionHDF5
//--------------------------------------------------------------

RangeDimensionHDF5::RangeDimensionHDF5(Group group, size_t id, vector<double> _ticks)
    : DimensionHDF5(group, id)
{
    setType();
    ticks(_ticks);
}


RangeDimensionHDF5::RangeDimensionHDF5(const RangeDimensionHDF5 &other)
    : DimensionHDF5(other.group, other.dim_id)
{
    setType();
    ticks(other.ticks());
}


DimensionType RangeDimensionHDF5::dimensionType() const {
    return DimensionType::Range;
}


boost::optional<std::string> RangeDimensionHDF5::label() const {
    boost::optional<std::string> ret;
    string label;
    group.getAttr("label", label);
    ret = label;
    return ret;
}


void RangeDimensionHDF5::label(const string &label) {
    if(label.empty()) {
        throw EmptyString("label");
    }
    else {
        group.setAttr("label", label);
        // NOTE: forceUpdatedAt() not possible since not reachable from here
    }
}


void RangeDimensionHDF5::label(const none_t t) {
    if(group.hasAttr("label")) {
        group.removeAttr("label");
    }
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


boost::optional<std::string> RangeDimensionHDF5::unit() const {
    boost::optional<std::string> ret;
    string unit;
    group.getAttr("unit", unit);
    ret = unit;
    return ret;
}


void RangeDimensionHDF5::unit(const string &unit) {
    if(unit.empty()) {
        throw EmptyString("unit");
    }
    else {
        group.setAttr("unit", unit);
        // NOTE: forceUpdatedAt() not possible since not reachable from here
    }
}


void RangeDimensionHDF5::unit(const none_t t) {
    if(group.hasAttr("unit")) {
        group.removeAttr("unit");
    }
    // NOTE: forceUpdatedAt() not possible since not reachable from here
}


RangeDimensionHDF5& RangeDimensionHDF5::operator=(const RangeDimensionHDF5 &other) {
    RangeDimensionHDF5 tmp(other);
    swap(tmp);
    return *this;
}


vector<double> RangeDimensionHDF5::ticks() const {
    vector<double> ticks;

    if(group.hasData("ticks")) {
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


} // namespace hdf5
} // namespace nix

