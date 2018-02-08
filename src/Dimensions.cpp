// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/Dimensions.hpp>

#include <cmath>
#include <nix/DataArray.hpp>
#include <nix/util/util.hpp>
#include <nix/Exception.hpp>

using namespace std;
using namespace nix;
using namespace nix::base;


//-------------------------------------------------------
// Implementation of Dimension
//-------------------------------------------------------

Dimension::Dimension()
    : ImplContainer()
{
}


Dimension::Dimension(const shared_ptr<IDimension> &p_impl)
    : ImplContainer(p_impl)
{
}


Dimension::Dimension(shared_ptr<IDimension> &&ptr)
    : ImplContainer(std::move(ptr))
{
}


Dimension::Dimension(const Dimension &other)
    : ImplContainer(other)
{
}


Dimension::Dimension(const SampledDimension &other)
    : ImplContainer(dynamic_pointer_cast<IDimension>(other.impl()))
{
}


Dimension::Dimension(const RangeDimension &other)
    : ImplContainer(dynamic_pointer_cast<IDimension>(other.impl()))
{
}


Dimension::Dimension(const SetDimension &other)
    : ImplContainer(dynamic_pointer_cast<IDimension>(other.impl()))
{
}


SetDimension Dimension::asSetDimension() const {
    if (dimensionType() != DimensionType::Set) {
        throw IncompatibleDimensions("Dimension is not of type Set and thus cannot be cast to this type", "asSetDimension");
    }
    return SetDimension(std::dynamic_pointer_cast<base::ISetDimension>(impl()));
}


SampledDimension Dimension::asSampledDimension() const {
    if (dimensionType() != DimensionType::Sample) {
        throw IncompatibleDimensions("Dimension is not of type Sample and thus cannot be cast to this type", "asSampledDimension");
    }
    return SampledDimension(std::dynamic_pointer_cast<base::ISampledDimension>(impl()));
}


RangeDimension Dimension::asRangeDimension() const {
    if (dimensionType() != DimensionType::Range) {
        throw IncompatibleDimensions("Dimension is not of type Range and thus cannot be cast to this type", "asRangeDimension");
    }
    return RangeDimension(std::dynamic_pointer_cast<base::IRangeDimension>(impl()));
}


Dimension& Dimension::operator=(const SampledDimension &other) {
    shared_ptr<IDimension> tmp(dynamic_pointer_cast<IDimension>(other.impl()));

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


Dimension& Dimension::operator=(const RangeDimension &other) {
    shared_ptr<IDimension> tmp(dynamic_pointer_cast<IDimension>(other.impl()));

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


Dimension& Dimension::operator=(const SetDimension &other) {
    shared_ptr<IDimension> tmp(dynamic_pointer_cast<IDimension>(other.impl()));

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}

//-------------------------------------------------------
// Implementation of SampledDimension
//-------------------------------------------------------

SampledDimension::SampledDimension()
    : ImplContainer()
{
}


SampledDimension::SampledDimension(const std::shared_ptr<ISampledDimension> &p_impl)
    : ImplContainer(p_impl)
{
}


SampledDimension::SampledDimension(std::shared_ptr<ISampledDimension> &&ptr)
    : ImplContainer(std::move(ptr))
{
}



SampledDimension::SampledDimension(const SampledDimension &other)
    : ImplContainer(other)
{
}


void SampledDimension::label(const std::string &label) {
    util::checkEmptyString(label, "label");
    backend()->label(label);
}


void SampledDimension::unit(const std::string &unit) {
    util::checkEmptyString(unit, "unit");
    if (!(util::isSIUnit(unit))) {
        throw InvalidUnit("Unit is not a SI unit. Note: so far, only atomic SI units are supported.",
                          "SampledDimension::unit(const string &unit)");
    }
    backend()->unit(unit);
}


void SampledDimension::samplingInterval(double interval) {
    if (interval <= 0.0) {
        throw std::runtime_error("SampledDimenion::samplingInterval: Sampling intervals must be larger than 0.0!");
    }
    backend()->samplingInterval(interval);
}


ndsize_t getSampledIndex(const double position, const double offset, const double sampling_interval) {
    ndssize_t index = static_cast<ndssize_t>(round(( position - offset) / sampling_interval));
    if (index < 0) {
        throw nix::OutOfBounds("Position is out of bounds of this dimension!", 0);
    }
    return static_cast<ndsize_t>(index);
}


ndsize_t SampledDimension::indexOf(const double position) const {
    double offset = backend()->offset() ? *(backend()->offset()) : 0.0;
    double sampling_interval = backend()->samplingInterval();
    return getSampledIndex(position, offset, sampling_interval);
}


std::pair<ndsize_t, ndsize_t> SampledDimension::indexOf(const double start, const double end) const {
    double offset = backend()->offset() ? *(backend()->offset()) : 0.0;
    double sampling_interval = backend()->samplingInterval();

    ndsize_t si = getSampledIndex(start, offset, sampling_interval);
    ndsize_t ei = getSampledIndex(end, offset, sampling_interval);
    return std::pair<ndsize_t, ndsize_t>(si, ei);
}


std::vector<std::pair<ndsize_t, ndsize_t>> SampledDimension::indexOf(const std::vector<double> &start_positions,
                                                                     const std::vector<double> &end_positions) const {
    std::vector<std::pair<ndsize_t, ndsize_t>> indices;
    double offset = backend()->offset() ? *(backend()->offset()) : 0.0;
    double sampling_interval = backend()->samplingInterval();

    for (size_t i = 0; i < (std::min(start_positions.size(), end_positions.size())); i++) {
        if (start_positions[i] > end_positions[i] ) {
            continue;
        }
        indices.emplace_back(getSampledIndex(start_positions[i], offset, sampling_interval),
                             getSampledIndex(end_positions[i], offset, sampling_interval));
    }
    return indices;
}

double SampledDimension::positionAt(const ndsize_t index) const {

    double offset = backend()->offset() ? *(backend()->offset()) : 0.0;
    double sampling_interval = backend()->samplingInterval();
    return index * sampling_interval + offset;
}


vector<double> SampledDimension::axis(const ndsize_t count, const ndsize_t startIndex) const {

    size_t cnt = check::fits_in_size_t(count, "Axis count exceeds memory (size larger than current system supports)");

    vector<double> axis(cnt);
    double offset =  backend()->offset() ? *(backend()->offset()) : 0.0;
    double sampling_interval = backend()->samplingInterval();
    for (size_t i = 0; i < axis.size(); ++i) {
        axis[i] = (static_cast<double>(i) + startIndex) * sampling_interval + offset;
    }
    return axis;
}


SampledDimension& SampledDimension::operator=(const SampledDimension &other) {
    shared_ptr<ISampledDimension> tmp(other.impl());

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


SampledDimension& SampledDimension::operator=(const Dimension &other) {
    shared_ptr<ISampledDimension> tmp(dynamic_pointer_cast<ISampledDimension>(other.impl()));

    if (other.dimensionType() != DimensionType::Sample) {
        throw nix::IncompatibleDimensions("Cannot assign dimension of type " + nix::util::dimTypeToStr(other.dimensionType())
                                          + " to a SampledDimension", "SampledDimension::operator=");
    }
    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }
    return *this;
}

//-------------------------------------------------------
// Implementation of SetDimension
//-------------------------------------------------------


SetDimension::SetDimension()
    : ImplContainer()
{
}


SetDimension::SetDimension(const std::shared_ptr<ISetDimension> &p_impl)
    : ImplContainer(p_impl)
{
}

SetDimension::SetDimension(std::shared_ptr<ISetDimension> &&ptr)
    : ImplContainer(std::move(ptr))
{
}


SetDimension::SetDimension(const SetDimension &other)
    : ImplContainer(other)
{
}


SetDimension& SetDimension::operator=(const SetDimension &other) {
    shared_ptr<ISetDimension> tmp(other.impl());

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


SetDimension& SetDimension::operator=(const Dimension &other) {
    shared_ptr<ISetDimension> tmp(dynamic_pointer_cast<ISetDimension>(other.impl()));

    if (other.dimensionType() != DimensionType::Set) {
        throw nix::IncompatibleDimensions("Cannot assign dimension of type " + nix::util::dimTypeToStr(other.dimensionType())
                                          + " to a SetDimension", "SetDimension::operator=");
    }
    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}

//-------------------------------------------------------
// Implementation of RangeDimension
//-------------------------------------------------------


RangeDimension::RangeDimension()
    : ImplContainer()
{
}


RangeDimension::RangeDimension(const DataArray &array)
    : ImplContainer()
{
    if (array.dataExtent().size() > 1) {
        throw InvalidRank("Error creating RangeDimension with DataArray: array must be 1-D!");
    }
}


RangeDimension::RangeDimension(const std::shared_ptr<IRangeDimension> &p_impl)
    : ImplContainer(p_impl)
{
}


RangeDimension::RangeDimension(std::shared_ptr<IRangeDimension> &&ptr)
    : ImplContainer(std::move(ptr))
{
}


RangeDimension::RangeDimension(const RangeDimension &other)
    : ImplContainer(other)
{
}


void RangeDimension::label(const std::string &label) {
    util::checkEmptyString(label, "label");
    backend()->label(label);
}

void RangeDimension::unit(const std::string &unit) {
    util::checkEmptyString(unit, "unit");
    if (!(util::isSIUnit(unit))) {
        throw InvalidUnit("Unit is not an atomic SI. Note: So far composite units are not supported", "RangeDimension::unit(const string &unit)");
    }
    backend()->unit(unit);
}


void RangeDimension::ticks(const std::vector<double> &ticks) {
    if (!std::is_sorted(ticks.begin(), ticks.end())) {
        std::string caller = "Range::ticks()";
        throw UnsortedTicks(caller);
    }
    backend()->ticks(ticks);
}


double RangeDimension::tickAt(const ndsize_t index) const {

    size_t idx = check::fits_in_size_t(index, "Tick index exceeds memory (size larger than current system supports)");

    vector<double> ticks = this->ticks();
    if (idx >= ticks.size()) {
        throw nix::OutOfBounds("RangeDimension::tickAt: Given index is out of bounds!", idx);
    }
    return ticks[idx];
}

ndsize_t getIndex(const double position, std::vector<double> &ticks, bool lower_bound) {
    if (position < *ticks.begin()) {
        return 0;
    } else if (position > *prev(ticks.end())) {
        return prev(ticks.end()) - ticks.begin();
    }
    ndsize_t index;
    if (lower_bound) {
        std::vector<double>::iterator lower = std::lower_bound(ticks.begin(), ticks.end(), position);
        index = lower - ticks.begin();
    } else{
        std::vector<double>::iterator upper = std::upper_bound(ticks.begin(), ticks.end(), position) - 1;
        index = upper - ticks.begin();
    }
    return index;
}


ndsize_t RangeDimension::indexOf(const double position, bool less_or_equal) const {
    vector<double> ticks = this->ticks();
    return getIndex(position, ticks, !less_or_equal);
}


pair<ndsize_t, ndsize_t> RangeDimension::indexOf(const double start, const double end) const {
    vector<double> ticks = this->ticks();
    ndsize_t si = getIndex(start, ticks, true);
    ndsize_t ei = getIndex(end, ticks, false);
    return std::pair<ndsize_t, ndsize_t>(si, ei);
}


std::vector<std::pair<ndsize_t, ndsize_t>> RangeDimension::indexOf(const std::vector<double> &start_positions,
                                                                   const std::vector<double> &end_positions) const {
    std::vector<std::pair<ndsize_t, ndsize_t>> indices;
    vector<double> ticks = this->ticks();

    for (size_t i = 0; i < (std::min(start_positions.size(), end_positions.size())); i++) {
        if (start_positions[i] > end_positions[i] ) {
            continue;
        }
        indices.emplace_back(getIndex(start_positions[i], ticks, true),
                             getIndex(end_positions[i], ticks, false));
    }
    return indices;
}


vector<double> RangeDimension::axis(const ndsize_t count, const ndsize_t startIndex) const {
    size_t cnt = check::fits_in_size_t(count, "Axis count exceeds memory (size larger than current system supports)");
    size_t idx = check::fits_in_size_t(startIndex, "Axis start index exceeds memory (size larger than current system supports)");

    vector<double> ticks = this->ticks();

    size_t end;
    if (nix_safe_add(cnt, idx, &end)) {
        throw nix::OutOfBounds("RangeDimension::axis: Count + startIndex > ndsize_t");
    }

    if (end > ticks.size()) {
        throw nix::OutOfBounds("RangeDimension::axis: Count + startIndex is invalid, reaches beyond the ticks stored in this dimension.");
    }

    vector<double>::const_iterator first = ticks.begin() + idx;
    vector<double> axis(first, first + cnt);
    return axis;
}


RangeDimension& RangeDimension::operator=(const RangeDimension &other) {
    shared_ptr<IRangeDimension> tmp(other.impl());

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


RangeDimension& RangeDimension::operator=(const Dimension &other) {
    shared_ptr<IRangeDimension> tmp(dynamic_pointer_cast<IRangeDimension>(other.impl()));

    if (other.dimensionType() != DimensionType::Range) {
        throw nix::IncompatibleDimensions("Cannot assign dimension of type " + nix::util::dimTypeToStr(other.dimensionType())
                                          + " to a RangeDimension", "RangeDimension::operator=");
    }
    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}
