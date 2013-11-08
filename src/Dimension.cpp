// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <pandora/DataSet.hpp>
#include <pandora/Dimension.hpp>

using namespace std;

namespace pandora {


DimensionType dimensionTypeFromStr(const string &str) {
    if (str == "set") {
        return DimensionType::SET_DIMENSION;
    } else if (str == "range") {
        return DimensionType::RANGE_DIMENSION;
    } else if (str == "sample") {
        return DimensionType::SAMPLED_DIMENSION;
    } else {
        throw runtime_error("Not a valid dimension name");
    }
}


std::string dimensionTypeToStr(DimensionType dim) {
    switch (dim) {
        case DimensionType::SET_DIMENSION:
            return "set";
        case DimensionType::RANGE_DIMENSION:
            return "range";
        case DimensionType::SAMPLED_DIMENSION:
            return "sample";
        default:
            throw runtime_error("Not a valid dimension type");
    }
}

// Implementation of Dimension

Dimension::Dimension(Group group, size_t id)
    : group(group), dim_id(id)
{
}


Dimension::Dimension(const Dimension &other)
    : group(other.group), dim_id(other.dim_id)
{
}


void Dimension::setType() {
    group.setAttr("dimension_type", dimensionTypeToStr(dimensionType()));
}


void Dimension::swap(Dimension &other) {
    using std::swap;
    swap(group, other.group);
    swap(dim_id, other.dim_id);
}


bool Dimension::operator==(const Dimension &other) const {
    return group == other.group;
}


bool Dimension::operator!=(const Dimension &other) const {
    return !(*this == other);
}


Dimension::~Dimension() {}

// Implementation of SampledDimension

SampledDimension::SampledDimension(Group group, size_t id)
    : Dimension(group, id)
{
    setType();
}


SampledDimension::SampledDimension(const SampledDimension &other)
    : Dimension(other.group, other.dim_id)
{
    setType();
}


DimensionType SampledDimension::dimensionType() const {
    return DimensionType::SAMPLED_DIMENSION;
}


string SampledDimension::label() const {
    string label;
    group.getAttr("label", label);
    return label;
}


void SampledDimension::label(const string &label) {
    group.setAttr("label", label);
}


string SampledDimension::unit() const {
    string unit;
    group.getAttr("unit", unit);
    return unit;
}


void SampledDimension::unit(const string &unit) {
    group.setAttr("unit", unit);
}


double SampledDimension::samplingInterval() const {
    double sampling_interval = 1;
    group.getAttr("sampling_interval", sampling_interval);
    return sampling_interval;
}


void SampledDimension::samplingInterval(double sampling_interval) {
    group.setAttr("sampling_interval", sampling_interval);
}


double SampledDimension::offset() const {
    double offset = 0;
    group.getAttr("offset", offset);
    return offset;
}


void SampledDimension::offset(double offset) {
    group.setAttr("offset", offset);
}


SampledDimension& SampledDimension::operator=(const SampledDimension &other) {
    SampledDimension tmp(other);
    swap(tmp);
    return *this;
}


SampledDimension::~SampledDimension() {}

// Implementation of SetDimension

SetDimension::SetDimension(Group group, size_t id)
    : Dimension(group, id)
{
    setType();
}


SetDimension::SetDimension(const SetDimension &other)
    : Dimension(other.group, other.dim_id)
{
    setType();
}


DimensionType SetDimension::dimensionType() const {
    return DimensionType::SET_DIMENSION;
}


vector<string> SetDimension::labels() const {
    vector<string> labels;

    if (group.hasData("labels")) {
        DataSet ds = group.openData("labels");
        ds.read(labels, true);
    }

    return labels;
}


void SetDimension::labels(const vector<string> &labels) {
    if (group.hasData("labels")) {
        DataSet ds = group.openData("labels");
        ds.extend({labels.size()});
        ds.write(labels);
    } else {
        DataSet ds = DataSet::create(group.h5Group(), "labels", labels,
                                     nullptr, nullptr);
        ds.write(labels);
    }
}


SetDimension& SetDimension::operator=(const SetDimension &other) {
    SetDimension tmp(other);
    swap(tmp);
    return *this;
}


SetDimension::~SetDimension() {}

// Implementation of RangeDimension

RangeDimension::RangeDimension(Group group, size_t id)
    : Dimension(group, id)
{
    setType();
}


RangeDimension::RangeDimension(const RangeDimension &other)
    : Dimension(other.group, other.dim_id)
{
    setType();
}


DimensionType RangeDimension::dimensionType() const {
    return DimensionType::RANGE_DIMENSION;
}


string RangeDimension::label() const {
    string label;
    group.getAttr("label", label);
    return label;
}


void RangeDimension::label(const string &label) {
    group.setAttr("label", label);
}


string RangeDimension::unit() const {
    string unit;
    group.getAttr("unit", unit);
    return unit;
}


void RangeDimension::unit(const string &unit) {
    group.setAttr("unit", unit);
}


RangeDimension& RangeDimension::operator=(const RangeDimension &other) {
    RangeDimension tmp(other);
    swap(tmp);
    return *this;
}


vector<double> RangeDimension::tics() const {
    vector<double> tics;

    if (group.hasData("tics")) {
        DataSet ds = group.openData("tics");
        ds.read(tics, true);
    }

    return tics;
}


void RangeDimension::tics(const vector<double> &tics) {
    if (group.hasData("tics")) {
        DataSet ds = group.openData("tics");
        ds.extend({tics.size()});
        ds.write(tics);
    } else {
        DataSet ds = DataSet::create(group.h5Group(), "tics", tics,
                                     nullptr, nullptr);
        ds.write(tics);
    }
}

RangeDimension::~RangeDimension() {}

} /* namespace pandora */

