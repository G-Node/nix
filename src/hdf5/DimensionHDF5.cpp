// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/DimensionHDF5.hpp>

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

SampledDimensionHDF5::SampledDimensionHDF5(Group group, size_t id)
    : DimensionHDF5(group, id)
{
    setType();
}


SampledDimensionHDF5::SampledDimensionHDF5(const SampledDimensionHDF5 &other)
    : DimensionHDF5(other.group, other.dim_id)
{
    setType();
}


DimensionType SampledDimensionHDF5::dimensionType() const {
    return DimensionType::Sample;
}


string SampledDimensionHDF5::label() const {
    string label;
    group.getAttr("label", label);
    return label;
}


void SampledDimensionHDF5::label(const string &label) {
    group.setAttr("label", label);
}


string SampledDimensionHDF5::unit() const {
    string unit;
    group.getAttr("unit", unit);
    return unit;
}


void SampledDimensionHDF5::unit(const string &unit) {
    group.setAttr("unit", unit);
}


double SampledDimensionHDF5::samplingInterval() const {
    double sampling_interval = 1;
    group.getAttr("sampling_interval", sampling_interval);
    return sampling_interval;
}


void SampledDimensionHDF5::samplingInterval(double sampling_interval) {
    group.setAttr("sampling_interval", sampling_interval);
}


boost::optional<double> SampledDimensionHDF5::offset() const {
    boost::optional<double> ret;
    double offset = 0;
    if(group.getAttr("offset", offset)){
        ret = offset;
    }
    return offset;
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

RangeDimensionHDF5::RangeDimensionHDF5(Group group, size_t id)
    : DimensionHDF5(group, id)
{
    setType();
}


RangeDimensionHDF5::RangeDimensionHDF5(const RangeDimensionHDF5 &other)
    : DimensionHDF5(other.group, other.dim_id)
{
    setType();
}


DimensionType RangeDimensionHDF5::dimensionType() const {
    return DimensionType::Range;
}


string RangeDimensionHDF5::label() const {
    string label;
    group.getAttr("label", label);
    return label;
}


void RangeDimensionHDF5::label(const string &label) {
    group.setAttr("label", label);
}


string RangeDimensionHDF5::unit() const {
    string unit;
    group.getAttr("unit", unit);
    return unit;
}


void RangeDimensionHDF5::unit(const string &unit) {
    group.setAttr("unit", unit);
}


RangeDimensionHDF5& RangeDimensionHDF5::operator=(const RangeDimensionHDF5 &other) {
    RangeDimensionHDF5 tmp(other);
    swap(tmp);
    return *this;
}


vector<double> RangeDimensionHDF5::tics() const {
    vector<double> tics;
    group.getData("tics", tics);
    return tics;
}


void RangeDimensionHDF5::tics(const vector<double> &tics) {
    group.setData("tics", tics);
}

RangeDimensionHDF5::~RangeDimensionHDF5() {}


} // namespace hdf5
} // namespace nix

