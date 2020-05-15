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


Dimension::Dimension(const DataFrameDimension &other)
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


DataFrameDimension Dimension::asDataFrameDimension() const {
    if (dimensionType() != DimensionType::DataFrame) {
        throw IncompatibleDimensions("Dimension is not of type DataFrame and thus cannot be cast to this type", "asDataFrameDimension");
    }
    return DataFrameDimension(std::dynamic_pointer_cast<base::IDataFrameDimension>(impl()));
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


Dimension& Dimension::operator=(const DataFrameDimension &other) {
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
    if (position < offset) {
        throw nix::OutOfBounds("SampledDimension::indexOf: Position is less than offset!", position);
    }
    ndssize_t index = static_cast<ndssize_t>(round(( position - offset) / sampling_interval));
    return index;
}


ndsize_t SampledDimension::indexOf(const double position) const {
    double offset = backend()->offset() ? *(backend()->offset()) : 0.0;
    double sampling_interval = backend()->samplingInterval();
    return getSampledIndex(position, offset, sampling_interval);
}


std::pair<ndsize_t, ndsize_t> SampledDimension::indexOf(const double start, const double end, RangeMatch match) const {
    double offset = backend()->offset() ? *(backend()->offset()) : 0.0;
    double sampling_interval = backend()->samplingInterval();
    return indexOf(start, end, sampling_interval, offset, match);
}


std::pair<ndsize_t, ndsize_t> SampledDimension::indexOf(double start, double end, const double sampling_interval, 
                                                        const double offset, RangeMatch match) const {
    if (start > end) {
        std::swap(start, end);
    }
    if (match == RangeMatch::Exclusive && end - start < sampling_interval/2) {
        throw nix::OutOfBounds("SampledDimension::indexOf: An invalid range occured, difference of start and end is less than the sampling interval.");
    }
    ndsize_t si = getSampledIndex(start, offset, sampling_interval);
    ndsize_t ei = getSampledIndex(end - (match == RangeMatch::Exclusive ? sampling_interval : 0.0), offset, sampling_interval);
    return std::pair<ndsize_t, ndsize_t>(si, ei);
}


std::vector<std::pair<ndsize_t, ndsize_t>> SampledDimension::indexOf(const std::vector<double> &start_positions,
                                                                     const std::vector<double> &end_positions, 
                                                                     RangeMatch match) const {
    if (start_positions.size() != end_positions.size()) {
        throw runtime_error("Dimension::IndexOf - Number of start and end positions must match!");
    }

    std::vector<std::pair<ndsize_t, ndsize_t>> indices;
    double offset = backend()->offset() ? *(backend()->offset()) : 0.0;
    double sampling_interval = backend()->samplingInterval();
    for (size_t i = 0; i < start_positions.size(); ++i) {
        indices.push_back(indexOf(start_positions[i], end_positions[i], sampling_interval, offset, match));
    }
    return indices;
}


double SampledDimension::positionAt(const ndsize_t index) const {

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
        double dbl = check::converts_to_double(i, "SampledDimension::axis conversion to double failed!");
        axis[i] = (dbl + startIndex) * sampling_interval + offset;
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
    vector<double> ticks = this->ticks(index, 1);
    return ticks[0];
}


PositionInRange RangeDimension::positionInRange(const double position) const {
    PositionInRange result;
    vector<double> ticks = this->ticks();
    if (ticks.size() == 0) {
        result = PositionInRange::NoRange;
    } else if (position < *ticks.begin()) {
        result = PositionInRange::Less;
    } else if (position > *prev(ticks.end())) {
        result = PositionInRange::Greater;
    } else {
        result = PositionInRange::InRange;
    }
    return result;
}


boost::optional<ndsize_t> getIndex(const double position, std::vector<double> &ticks, PositionMatch matching) {
    boost::optional<ndsize_t> idx;
    // check easy cases first ...
    if (ticks.size() == 0)
        return idx;
    if (position < *ticks.begin()) {
        if (matching == PositionMatch::Greater || matching == PositionMatch::GreaterOrEqual)
            idx = 0;
        return idx;
    } else if (position > *prev(ticks.end())) {
        if (matching == PositionMatch::Less || matching == PositionMatch::LessOrEqual)
            idx =  prev(ticks.end()) - ticks.begin();
        return idx;
    }
    // need to do some searching --> first element larger or equal to position
    std::vector<double>::iterator lower = std::lower_bound(ticks.begin(), ticks.end(), position);
    if (matching == PositionMatch::Greater || matching == PositionMatch::GreaterOrEqual) {
        idx = lower - ticks.begin();
        if (matching == PositionMatch::Greater && *lower == position) {
            if ((lower + 1) < ticks.end()) {
                idx = lower + 1 - ticks.begin();
            } else {
                idx = boost::none;
            }
        }
    } else if (matching == PositionMatch::LessOrEqual && *lower > position) {
        if (lower - 1 >= ticks.begin()) {
            idx = lower - 1 - ticks.begin();
        } else {
            idx = boost::none;
        }
    } else if (matching == PositionMatch::Less && *lower >= position) {
        if ((lower - 1) >= ticks.begin()) {
            idx = lower - 1 - ticks.begin();
        } else {
            idx = boost::none;
        }
    } else { // exact match
        if (lower != ticks.end() && *lower == position) {
            idx = lower - ticks.begin();
        }
    }
    return idx;
}


boost::optional<ndsize_t> RangeDimension::indexOf(const double position, PositionMatch matching) const {
    vector<double> ticks = this->ticks();
    boost::optional<ndsize_t> index = getIndex(position, ticks, matching);
    return index;
}


boost::optional<std::pair<ndsize_t, ndsize_t>> RangeDimension::indexOf(double start, double end,
                                                                       std::vector<double> &&ticks,
                                                                       RangeMatch match) const {
    if (ticks.size() == 0) {
        ticks = this->ticks();
    }
    boost::optional<std::pair<ndsize_t, ndsize_t>> range;
    if (start > end){
        std::swap(start, end);
    }

    boost::optional<ndsize_t> si = getIndex(start, ticks, PositionMatch::GreaterOrEqual);
    if (!si) {
        return range;
    }
    PositionMatch endMatching = (match == RangeMatch::Inclusive) ? PositionMatch::LessOrEqual : PositionMatch::Less;
    boost::optional<ndsize_t> ei = getIndex(end, ticks, endMatching);
    if (ei && *si <= *ei) {
        range = std::pair<ndsize_t, ndsize_t>(*si, *ei);
    }
    return range;
}


ndsize_t RangeDimension::indexOf(const double position, bool less_or_equal) const {
    vector<double> ticks = this->ticks();
    PositionMatch matching = less_or_equal ? PositionMatch::LessOrEqual : PositionMatch::GreaterOrEqual;
    boost::optional<ndsize_t> index = getIndex(position, ticks, matching);
    if (index)
        return *index;
    else
        throw nix::OutOfBounds("RangeDimension::indexOf: Position is out of bounds.");
}


pair<ndsize_t, ndsize_t> RangeDimension::indexOf(const double start, const double end) const {
    vector<double> ticks = this->ticks();
    boost::optional<ndsize_t> si = getIndex(start, ticks, PositionMatch::GreaterOrEqual);
    boost::optional<ndsize_t> ei = getIndex(end, ticks, PositionMatch::LessOrEqual);
    if (!ei || !si) {
        throw nix::OutOfBounds("RangeDimension::indexOf: start or end of range are out of Bounds!");
    }
    return std::pair<ndsize_t, ndsize_t>(*si, *ei);
}


std::vector<std::pair<ndsize_t, ndsize_t>> RangeDimension::indexOf(const std::vector<double> &start_positions,
                                                                   const std::vector<double> &end_positions,
                                                                   bool strict, RangeMatch match) const {
    std::vector<boost::optional<std::pair<ndsize_t, ndsize_t>>> optionalIndices;
    optionalIndices = indexOf(start_positions, end_positions, match);
    std::vector<std::pair<ndsize_t, ndsize_t>> indices;
    for(auto o: optionalIndices) {
        if (o) {
            indices.push_back(*o);
        } else if(strict) {
            throw nix::OutOfBounds("RangeDimension::indexOf: an invalid range was encountered.");
        }
    }
    return indices;
}


std::vector<boost::optional<std::pair<ndsize_t, ndsize_t>>> RangeDimension::indexOf(const std::vector<double> &start_positions,
                                                                                    const std::vector<double> &end_positions,
                                                                                    RangeMatch match) const {
    if (start_positions.size() != end_positions.size()) {
        throw runtime_error("Dimension::IndexOf - Number of start and end positions must match!");
    }

    std::vector<boost::optional<std::pair<ndsize_t, ndsize_t>>> indices;
    vector<double> ticks = this->ticks();
    for (size_t i = 0; i < start_positions.size(); ++i) {
        boost::optional<std::pair<ndsize_t, ndsize_t>> range;
        range = this->indexOf(start_positions[i], end_positions[i], std::move(ticks), match);
        indices.push_back(range);
    }
    return indices;
}


vector<double> RangeDimension::axis(const ndsize_t count, const ndsize_t startIndex) const {
    size_t cnt = check::fits_in_size_t(count, "Axis count exceeds memory (size larger than current system supports)");
    vector<double> ticks = this->ticks(startIndex, cnt);
    return ticks;
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


//-------------------------------------------------------
// Implementation of DataFrameDimension
//-------------------------------------------------------

DataFrameDimension::DataFrameDimension()
    : ImplContainer()
{
}


DataFrameDimension::DataFrameDimension(const std::shared_ptr<IDataFrameDimension> &p_impl)
    : ImplContainer(p_impl)
{
}


DataFrameDimension::DataFrameDimension(std::shared_ptr<IDataFrameDimension> &&ptr)
    : ImplContainer(std::move(ptr))
{
}


DataFrameDimension::DataFrameDimension(const DataFrameDimension &other)
    : ImplContainer(other)
{
}


DataFrameDimension& DataFrameDimension::operator=(const DataFrameDimension &other) {
    shared_ptr<IDataFrameDimension> tmp(other.impl());

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


DataFrameDimension& DataFrameDimension::operator=(const Dimension &other) {
    shared_ptr<IDataFrameDimension> tmp(dynamic_pointer_cast<IDataFrameDimension>(other.impl()));

    if (other.dimensionType() != DimensionType::DataFrame) {
        throw nix::IncompatibleDimensions("Cannot assign dimension of type " + nix::util::dimTypeToStr(other.dimensionType())
                                          + " to a DataFrameDimension", "DataFrameDimension::operator=");
    }
    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }
    return *this;
}
