// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in Section and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <limits>
#include <sstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>

#include "BaseTestDimension.hpp"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <boost/math/constants/constants.hpp>

using namespace nix;
using namespace valid;


void BaseTestDimension::testValidate() {
    Dimension d = data_array.appendSetDimension();
    valid::Result result = validate(d);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void BaseTestDimension::testSetValidate() {
    SetDimension d = data_array.appendSetDimension();
    valid::Result result = validate(d);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void BaseTestDimension::testRangeValidate() {
    std::vector<double> ticks;
    for (size_t i = 0; i < 5; i++) {
        ticks.push_back(i * boost::math::constants::pi<double>());
    }
    RangeDimension d = data_array.appendRangeDimension(ticks);
    valid::Result result = validate(d);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void BaseTestDimension::testSampleValidate() {
    double samplingInterval = boost::math::constants::pi<double>();
    SampledDimension d = data_array.appendSampledDimension(samplingInterval);
    valid::Result result = validate(d);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void BaseTestDimension::testIndex() {
    Dimension sd = data_array.appendSetDimension();
    CPPUNIT_ASSERT(data_array.dimensionCount() == 1 && sd.index() == 1);
    data_array.deleteDimensions();
    CPPUNIT_ASSERT(data_array.dimensionCount() == 0);
}


void BaseTestDimension::testSampledDimLabel() {
    std::string label = "aLabel";
    std::string other_label = "anotherLabel";
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);

    SampledDimension sd;
    sd = d;
    sd.label(label);
    CPPUNIT_ASSERT(*(sd.label()) == label);
    sd.label(other_label);
    CPPUNIT_ASSERT(*(sd.label()) == other_label);
    CPPUNIT_ASSERT_NO_THROW(sd.label(none));
    CPPUNIT_ASSERT(sd.label() == none);

    data_array.deleteDimensions();
}


void BaseTestDimension::testSampledDimUnit() {
    std::string invalidUnit = "invalidunit";
    std::string validUnit = "mV^2";
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);

    SampledDimension sd;
    sd = d;
    CPPUNIT_ASSERT_THROW(sd.unit(invalidUnit), InvalidUnit);
    CPPUNIT_ASSERT_NO_THROW(sd.unit(validUnit));
    CPPUNIT_ASSERT(*(sd.unit()) == validUnit);
    CPPUNIT_ASSERT_NO_THROW(sd.unit(boost::none));
    CPPUNIT_ASSERT(sd.unit() == boost::none);
    data_array.deleteDimensions();
}


void BaseTestDimension::testSampledDimSamplingInterval() {
    double impossible_sampling_interval = -1.0;
    double invalid_sampling_interval = 0.0;
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);

    SampledDimension sd;
    sd = d;
    CPPUNIT_ASSERT(sd.samplingInterval() == boost::math::constants::pi<double>());
    CPPUNIT_ASSERT_THROW(sd.samplingInterval(impossible_sampling_interval), std::runtime_error);
    CPPUNIT_ASSERT_THROW(sd.samplingInterval(invalid_sampling_interval), std::runtime_error);
    CPPUNIT_ASSERT_NO_THROW(sd.samplingInterval(samplingInterval));
    CPPUNIT_ASSERT(sd.samplingInterval() == samplingInterval);

    data_array.deleteDimensions();
}


void BaseTestDimension::testSampledDimOffset() {
    double offset = 1.0;
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);

    SampledDimension sd;
    sd = d;
    CPPUNIT_ASSERT_NO_THROW(sd.offset(offset));
    CPPUNIT_ASSERT(*(sd.offset()) == offset);
    CPPUNIT_ASSERT_NO_THROW(sd.offset(boost::none));
    CPPUNIT_ASSERT(sd.offset() == boost::none);

    data_array.deleteDimensions();
}


void BaseTestDimension::testSampledDimIndexOfOld() {
    double offset = 1.0;
    double samplingInterval = 1.0;

    Dimension d = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);
    SampledDimension sd;
    sd = d;
    // no offset, sampling interval = 1
    CPPUNIT_ASSERT_NO_THROW(sd.indexOf(-0.5));
    CPPUNIT_ASSERT_NO_THROW(sd.indexOf(0.0));
    CPPUNIT_ASSERT_NO_THROW(sd.indexOf(0.5));
    CPPUNIT_ASSERT_NO_THROW(sd.indexOf(10000000));

    CPPUNIT_ASSERT(sd.indexOf(-0.5) == 0);
    CPPUNIT_ASSERT(sd.indexOf(1.0) == 1);
    CPPUNIT_ASSERT(sd.indexOf(2.4) == 3);  // GreaterOrEqual
    CPPUNIT_ASSERT(sd.indexOf(4.28) == 5);
    CPPUNIT_ASSERT(sd.indexOf(6.99) == 7);
    CPPUNIT_ASSERT(sd.indexOf(7.0) == 7);

    // offset = 1.0 , sampling interval = 1.0
    sd.offset(offset);
    CPPUNIT_ASSERT(*(sd.offset()) == offset);
    
    CPPUNIT_ASSERT_NO_THROW(sd.indexOf(-3.14));
    CPPUNIT_ASSERT_NO_THROW(sd.indexOf(0.5));
    CPPUNIT_ASSERT_NO_THROW(sd.indexOf(1.0));
    
    CPPUNIT_ASSERT(sd.indexOf(-1.0) == 0);
    CPPUNIT_ASSERT(sd.indexOf(0.0) == 0);
    CPPUNIT_ASSERT(sd.indexOf(2.0) == 1);
    CPPUNIT_ASSERT(sd.indexOf(6.28) == 6);
    CPPUNIT_ASSERT(sd.indexOf(3.75) == 3);

    // test ranges, offset = 1.0, sampling interval = 1.0
    CPPUNIT_ASSERT_NO_THROW(sd.indexOf(-0.25, 1.01));
    CPPUNIT_ASSERT_NO_THROW(sd.indexOf(0.0, 1.01));
    CPPUNIT_ASSERT_NO_THROW(sd.indexOf(1.0, 1.01));
    CPPUNIT_ASSERT_THROW(sd.indexOf(1.01, 0.0), nix::OutOfBounds);

    CPPUNIT_ASSERT_THROW(sd.indexOf(-1.0, -.05), nix::OutOfBounds);  // because end pos is invalid
    std::pair<ndsize_t, ndsize_t> range = sd.indexOf(-1.0, 5.0);
    CPPUNIT_ASSERT(range.first == 0 && range.second == 4);
    range = sd.indexOf(1.0, 5.0);
    CPPUNIT_ASSERT(range.first == 0 && range.second == 4);
    CPPUNIT_ASSERT_THROW(sd.indexOf(5.0, 1.0), nix::OutOfBounds);
    range = sd.indexOf(1.5, 3.2);
    CPPUNIT_ASSERT(range.first == 1 && range.second == 2); // less or equal for end of range
    range = sd.indexOf(1.5, 3.7);
    CPPUNIT_ASSERT(range.first == 1 && range.second == 2); // less or equal for end of range
    range = sd.indexOf(1.5, 4.0);
    CPPUNIT_ASSERT(range.first == 1 && range.second == 3); // less or equal for end of range

    // test vectors of ranges, offset = 1, sampling interval = 1
    CPPUNIT_ASSERT_THROW(sd.indexOf({0.0, 20.0, 40.0}, {10.9}), std::runtime_error);        
    CPPUNIT_ASSERT_THROW(sd.indexOf({0.0, 20.0, 40.0}, {10.9, 12., 1.}), nix::OutOfBounds);
    CPPUNIT_ASSERT_NO_THROW(sd.indexOf({1.0, 12.0, 1.0}, {10.9, 20., 40.}));
    CPPUNIT_ASSERT(sd.indexOf({1.0, 12.0, 1.0}, {10.9, 20., 40.}).size() == 3);

    std::vector<std::pair<ndsize_t, ndsize_t>> ranges = sd.indexOf({1.0, 12.0, 1.0}, {10.9, 20., 40.});
    CPPUNIT_ASSERT(ranges.size() == 3);
    CPPUNIT_ASSERT(ranges[0].first == 0 && ranges[0].second == 9);
    CPPUNIT_ASSERT(sd.positionAt(ranges[0].first) == 1.0 && sd.positionAt(ranges[0].second) == 10.); 
    CPPUNIT_ASSERT(ranges[1].first == 11 && ranges[1].second == 19);
    CPPUNIT_ASSERT(sd.positionAt(ranges[1].first) == 12 && sd.positionAt(ranges[1].second) == 20); 
    CPPUNIT_ASSERT(ranges[2].first == 0 && ranges[2].second == 39);
    CPPUNIT_ASSERT(sd.positionAt(ranges[2].first) == 1. && sd.positionAt(ranges[2].second) == 40); 

    // offset = -1.0 , sampling interval = 1.0
    sd.offset(-1.0);
    CPPUNIT_ASSERT(*(sd.offset()) == -1.0);
    
    CPPUNIT_ASSERT_NO_THROW(sd.indexOf(-3.14));
    CPPUNIT_ASSERT_NO_THROW(sd.indexOf(-1.5));
    CPPUNIT_ASSERT_NO_THROW(sd.indexOf(-0.5));
    
    CPPUNIT_ASSERT(sd.indexOf(-3.14) == 0);
    CPPUNIT_ASSERT(sd.indexOf(-0.5) == 1);
    CPPUNIT_ASSERT(sd.indexOf(-1.0) == 0);
    CPPUNIT_ASSERT(sd.indexOf(0.5) == 2);
    data_array.deleteDimensions();
}


void BaseTestDimension::testSampledDimIndexOf() {
    double samplingInterval = 1.;

    Dimension d = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);

    SampledDimension sd;
    sd = d;
    // offset = 0, sampling interval = 1
    CPPUNIT_ASSERT(!sd.indexOf(-3.14, PositionMatch::Less));
    CPPUNIT_ASSERT(!sd.indexOf(-3.14, PositionMatch::LessOrEqual));
    CPPUNIT_ASSERT(!sd.indexOf(-3.14, PositionMatch::Equal));
    CPPUNIT_ASSERT(sd.indexOf(-3.14, PositionMatch::GreaterOrEqual));
    CPPUNIT_ASSERT(sd.indexOf(-3.14, PositionMatch::Greater));
     
    CPPUNIT_ASSERT(!sd.indexOf(0.0, PositionMatch::Less));
    CPPUNIT_ASSERT(sd.indexOf(0.0, PositionMatch::LessOrEqual));
    CPPUNIT_ASSERT(sd.indexOf(0.0, PositionMatch::Equal));
    CPPUNIT_ASSERT(sd.indexOf(0.0, PositionMatch::GreaterOrEqual));
    CPPUNIT_ASSERT(sd.indexOf(0.0, PositionMatch::Greater));

    CPPUNIT_ASSERT(sd.indexOf(1.0, PositionMatch::Less));
    CPPUNIT_ASSERT(sd.indexOf(1.0, PositionMatch::LessOrEqual));
    CPPUNIT_ASSERT(sd.indexOf(1.0, PositionMatch::Equal));
    CPPUNIT_ASSERT(sd.indexOf(1.0, PositionMatch::GreaterOrEqual));
    CPPUNIT_ASSERT(sd.indexOf(1.0, PositionMatch::Greater));
   
    // offset = 1.0, sampling interval = 1
    sd.offset(1.0);
    CPPUNIT_ASSERT(!sd.indexOf(-3.14, PositionMatch::Less));
    CPPUNIT_ASSERT(!sd.indexOf(-3.14, PositionMatch::LessOrEqual));
    CPPUNIT_ASSERT(!sd.indexOf(-3.14, PositionMatch::Equal));
    CPPUNIT_ASSERT(sd.indexOf(-3.14, PositionMatch::GreaterOrEqual));
    CPPUNIT_ASSERT(sd.indexOf(-3.14, PositionMatch::Greater));
     
    CPPUNIT_ASSERT(!sd.indexOf(0.0, PositionMatch::Less));
    CPPUNIT_ASSERT(!sd.indexOf(0.0, PositionMatch::LessOrEqual));
    CPPUNIT_ASSERT(!sd.indexOf(0.0, PositionMatch::Equal));
    CPPUNIT_ASSERT(sd.indexOf(0.0, PositionMatch::GreaterOrEqual));
    CPPUNIT_ASSERT(sd.indexOf(0.0, PositionMatch::Greater));

    CPPUNIT_ASSERT(!sd.indexOf(1.0, PositionMatch::Less));
    CPPUNIT_ASSERT(sd.indexOf(1.0, PositionMatch::LessOrEqual));
    CPPUNIT_ASSERT(sd.indexOf(1.0, PositionMatch::Equal));
    CPPUNIT_ASSERT(sd.indexOf(1.0, PositionMatch::GreaterOrEqual));
    CPPUNIT_ASSERT(sd.indexOf(1.0, PositionMatch::Greater));

    sd.offset(-1.0);
    // offset = -1.0, sampling interval = 1
    CPPUNIT_ASSERT(!sd.indexOf(-3.14, PositionMatch::Less));
    CPPUNIT_ASSERT(!sd.indexOf(-3.14, PositionMatch::LessOrEqual));
    CPPUNIT_ASSERT(!sd.indexOf(-3.14, PositionMatch::Equal));
    CPPUNIT_ASSERT(sd.indexOf(-3.14, PositionMatch::GreaterOrEqual) && *sd.indexOf(-3.14, PositionMatch::GreaterOrEqual) == 0) ;
    CPPUNIT_ASSERT(sd.indexOf(-3.14, PositionMatch::Greater) && *sd.indexOf(-3.14, PositionMatch::Greater) == 0);
     
    CPPUNIT_ASSERT(sd.indexOf(0.0, PositionMatch::Less) && *sd.indexOf(0.0, PositionMatch::Less) == 0);
    CPPUNIT_ASSERT(sd.indexOf(0.0, PositionMatch::LessOrEqual) && *sd.indexOf(0.0, PositionMatch::LessOrEqual) == 1);
    CPPUNIT_ASSERT(sd.indexOf(0.0, PositionMatch::Equal) && *sd.indexOf(0.0, PositionMatch::Equal) == 1);
    CPPUNIT_ASSERT(sd.indexOf(0.0, PositionMatch::GreaterOrEqual) && *sd.indexOf(0.0, PositionMatch::GreaterOrEqual) == 1);
    CPPUNIT_ASSERT(sd.indexOf(0.0, PositionMatch::Greater) && *sd.indexOf(0.0, PositionMatch::Greater) == 2);

    // test ranges offset = -1, sampling interval = 1
    boost::optional<std::pair<ndsize_t, ndsize_t>> range = sd.indexOf(0.0, 0.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 1 && (*range).second == 1);
    range = sd.indexOf(0.0, 0.0, RangeMatch::Exclusive);    
    CPPUNIT_ASSERT(!range);
    range = sd.indexOf(-1.5, 2.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 3);
    range = sd.indexOf(-1.5, 2.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 2);
    range = sd.indexOf(2., -2.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(!range); // && (*range).first == 0 && (*range).second == 3);
    range = sd.indexOf(2., -2.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(!range); // && (*range).first == 0 && (*range).second == 2);
    range = sd.indexOf(2.0, 2.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 3 && (*range).second == 3);
    range = sd.indexOf(2., 2.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(!range);

    // test vector of ranges, offset = -1, sampling interval = 1
    CPPUNIT_ASSERT_THROW(sd.indexOf({1.0, 20.0, 40.0}, {10.9, 12.}, RangeMatch::Exclusive), std::runtime_error);
    
    std::vector<boost::optional<std::pair<ndsize_t, ndsize_t>>> ranges = sd.indexOf({1.0, 12.0, 1.0, 5.0}, {10.9, 20.0, 40.0, 5.0}, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(ranges.size() == 4);
    CPPUNIT_ASSERT(ranges[0] && (*ranges[0]).first == 2 && (*ranges[0]).second == 11 && 
                   sd.positionAt((*ranges[0]).first) >= 1.0 && sd.positionAt((*ranges[0]).second) <= 10.9);
    CPPUNIT_ASSERT(ranges[1] && (*ranges[1]).first == 13 && (*ranges[1]).second == 21 && 
                   sd.positionAt((*ranges[1]).first) == 12 && sd.positionAt((*ranges[1]).second) == 20);
    CPPUNIT_ASSERT(ranges[2] && (*ranges[2]).first == 2 && (*ranges[2]).second == 41 && 
                   sd.positionAt((*ranges[2]).first) == 1.0 && sd.positionAt((*ranges[2]).second) == 40);
    CPPUNIT_ASSERT(ranges[3] && (*ranges[3]).first == 6 && (*ranges[3]).second == 6 && 
                   sd.positionAt((*ranges[3]).first) == 5 && sd.positionAt((*ranges[3]).second) == 5);

    ranges = sd.indexOf({1.0, 12.0, 1.0, 5.0}, {10.9, 20.0, 40.0, 5.0}, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(ranges.size() == 4);
    CPPUNIT_ASSERT(ranges[0] && (*ranges[0]).first == 2 && (*ranges[0]).second == 11 && 
                   sd.positionAt((*ranges[0]).first) >= 1.0 && sd.positionAt((*ranges[0]).second) <= 10.9);
    CPPUNIT_ASSERT(ranges[1] && (*ranges[1]).first == 13 && (*ranges[1]).second == 20 && 
                   sd.positionAt((*ranges[1]).first) == 12 && sd.positionAt((*ranges[1]).second) == 19);
    CPPUNIT_ASSERT(ranges[2] && (*ranges[2]).first == 2 && (*ranges[2]).second == 40 && 
                   sd.positionAt((*ranges[2]).first) == 1.0 && sd.positionAt((*ranges[2]).second) == 39);
    CPPUNIT_ASSERT(!ranges[3]);
    data_array.deleteDimensions();
}


void BaseTestDimension::testSampledDimPositionAt() {
    double offset = 1.0;
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);

    SampledDimension sd;
    sd = d;
    sd.offset(offset);
    CPPUNIT_ASSERT(sd.positionAt(0) == offset);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        200 * samplingInterval + offset,
        sd.positionAt(200),
        std::numeric_limits<double>::round_error());

    CPPUNIT_ASSERT(sd[0] == offset);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        200 * samplingInterval + offset,
        sd[200],
        std::numeric_limits<double>::round_error());

    data_array.deleteDimensions();
}


void BaseTestDimension::testSampledDimAxis() {
    double offset = 1.0;
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);

    SampledDimension sd;
    sd = d;
    sd.offset(offset);

    std::vector<double> axis = sd.axis(100);
    CPPUNIT_ASSERT(axis.size() == 100);
    CPPUNIT_ASSERT(axis[0] == offset);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        99 * samplingInterval + offset,
        axis.back(),
        std::numeric_limits<double>::round_error());

    axis = sd.axis(100, 10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        10 * samplingInterval + offset,
        axis[0],
        std::numeric_limits<double>::round_error());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        109 * samplingInterval + offset,
        axis.back(),
        std::numeric_limits<double>::round_error());

    data_array.deleteDimensions();
}


void BaseTestDimension::testSampledDimOperators() {
    double samplingInterval = boost::math::constants::pi<double>();
    Dimension d = data_array.appendSampledDimension(samplingInterval);
    Dimension d2 = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);
    CPPUNIT_ASSERT(d2.dimensionType() == DimensionType::Sample);
    CPPUNIT_ASSERT_THROW(d.asRangeDimension(), nix::IncompatibleDimensions);
    CPPUNIT_ASSERT_THROW(d.asSetDimension(), nix::IncompatibleDimensions);
    SampledDimension sd1, sd2, sd3;
    sd1 = d;
    sd2 = d2;
    sd3 = data_array.getDimension(d.index());
    CPPUNIT_ASSERT(sd1.index() == d.index() && sd2.index() == d2.index());
    CPPUNIT_ASSERT(sd1 != sd2);
    CPPUNIT_ASSERT(sd1 != sd3);
    data_array.deleteDimensions();
    Dimension dim = data_array.appendSetDimension();
    CPPUNIT_ASSERT_THROW(dim.asSampledDimension(), IncompatibleDimensions);
    SampledDimension sampled = data_array.appendSampledDimension(samplingInterval);
    RangeDimension range = data_array.appendRangeDimension(std::vector<double>({1, 2}));
    SetDimension set = data_array.appendSetDimension();
    std::stringstream s_stream, r_stream, set_stream;
    s_stream << sampled.dimensionType();
    r_stream << range.dimensionType();
    set_stream << set.dimensionType();
    CPPUNIT_ASSERT(s_stream.str() == "Sample");
    CPPUNIT_ASSERT(set_stream.str() == "Set");
    CPPUNIT_ASSERT(r_stream.str() == "Range");
    CPPUNIT_ASSERT(d != none);
    CPPUNIT_ASSERT(sampled != none);
    CPPUNIT_ASSERT(range != none);
    CPPUNIT_ASSERT(set != none);

    SampledDimension sd4;
    CPPUNIT_ASSERT_THROW(sd4 = range, nix::IncompatibleDimensions);
    CPPUNIT_ASSERT_NO_THROW(sd4 = sampled);
    RangeDimension rd;
    CPPUNIT_ASSERT_THROW(rd = sampled, nix::IncompatibleDimensions);
    CPPUNIT_ASSERT_NO_THROW(rd = range);
    SetDimension st;
    CPPUNIT_ASSERT_THROW(st = sampled, nix::IncompatibleDimensions);
    CPPUNIT_ASSERT_NO_THROW(st = set);

    d = none; sampled = none; range = none; set = none;
    CPPUNIT_ASSERT(d == none);
    CPPUNIT_ASSERT(sampled == none);
    CPPUNIT_ASSERT(range == none);
    CPPUNIT_ASSERT(set == none);
}


void BaseTestDimension::testSetDimLabel() {
    std::string label = "colors";
    std::string other_label = "colours";

    Dimension d = data_array.appendSetDimension({"red", "green", "blue"});
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Set);

    SetDimension sd;
    sd = d;
    sd.label(label);
    CPPUNIT_ASSERT(*(sd.label()) == label);
    sd.label(other_label);
    CPPUNIT_ASSERT(*(sd.label()) == other_label);
    CPPUNIT_ASSERT_NO_THROW(sd.label(none));
    CPPUNIT_ASSERT(sd.label() == none);

    data_array.deleteDimensions();
}


void BaseTestDimension::testSetDimLabels() {
    std::vector<std::string> labels = {"label_a", "label_b","label_c","label_d","label_e"};
    std::vector<std::string> new_labels = {"new label_a", "new label_b","new label_c"};

    Dimension d = data_array.appendSetDimension();
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Set);

    SetDimension sd;
    sd = d;
    CPPUNIT_ASSERT(sd);
    sd.labels(labels);
    std::vector<std::string> retrieved_labels = sd.labels();
    CPPUNIT_ASSERT(retrieved_labels.size() == labels.size());
    for (size_t i = 0; i < labels.size(); i++){
        CPPUNIT_ASSERT(labels[i] == retrieved_labels[i]);
    }

    sd.labels(new_labels);
    retrieved_labels = sd.labels();
    CPPUNIT_ASSERT(retrieved_labels.size() == new_labels.size());
    for (size_t i = 0; i < new_labels.size(); i++){
        CPPUNIT_ASSERT(new_labels[i] == retrieved_labels[i]);
    }

    sd.labels(boost::none);
    retrieved_labels = sd.labels();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), retrieved_labels.size());

    data_array.deleteDimensions();
}

void BaseTestDimension::testSetDimIndexOf() {
    std::vector<std::string> labels = {"label_a", "label_b","label_c","label_d","label_e"};

    Dimension d = data_array.appendSetDimension();
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Set);
    SetDimension sd;
    sd = d;
    sd.labels(labels);

    boost::optional<ndsize_t> index;
    index = sd.indexOf(-1.0, PositionMatch::Less);
    CPPUNIT_ASSERT(!index);
    index = sd.indexOf(-1.0, PositionMatch::LessOrEqual);
    CPPUNIT_ASSERT(!index);
    index = sd.indexOf(-1.0, PositionMatch::Equal);
    CPPUNIT_ASSERT(!index);
    index = sd.indexOf(-1.0, PositionMatch::GreaterOrEqual);
    CPPUNIT_ASSERT(index && *index == 0);
    index = sd.indexOf(-1.0, PositionMatch::Greater);
    CPPUNIT_ASSERT(index && *index == 0);

    index = sd.indexOf(0.0, PositionMatch::Less);
    CPPUNIT_ASSERT(!index);
    index = sd.indexOf(0.0, PositionMatch::LessOrEqual);
    CPPUNIT_ASSERT(index && *index == 0);
    index = sd.indexOf(0.0, PositionMatch::Equal);
    CPPUNIT_ASSERT(index && *index == 0);
    index = sd.indexOf(0.0000001, PositionMatch::Equal);
    CPPUNIT_ASSERT(!index);
    index = sd.indexOf(0.0, PositionMatch::GreaterOrEqual);
    CPPUNIT_ASSERT(index && *index == 0);
    index = sd.indexOf(0.0, PositionMatch::Greater);
    CPPUNIT_ASSERT(index && *index == 1);

    index = sd.indexOf(4.0, PositionMatch::Less);
    CPPUNIT_ASSERT(index && *index == 3);
    index = sd.indexOf(4.0, PositionMatch::LessOrEqual);
    CPPUNIT_ASSERT(index && *index == 4);
    index = sd.indexOf(4.0, PositionMatch::Equal);
    CPPUNIT_ASSERT(index && *index == 4);
    index = sd.indexOf(4.0, PositionMatch::GreaterOrEqual);
    CPPUNIT_ASSERT(index && *index == 4);
    index = sd.indexOf(4.0, PositionMatch::Greater);
    CPPUNIT_ASSERT(!index);

    index = sd.indexOf(5.0, PositionMatch::Less);
    CPPUNIT_ASSERT(index && *index == 4);
    index = sd.indexOf(5.0, PositionMatch::LessOrEqual);
    CPPUNIT_ASSERT(index && *index == 4);
    index = sd.indexOf(5.0, PositionMatch::Equal);
    CPPUNIT_ASSERT(!index);
    index = sd.indexOf(5.0, PositionMatch::GreaterOrEqual);
    CPPUNIT_ASSERT(!index);
    index = sd.indexOf(5.0, PositionMatch::Greater);
    CPPUNIT_ASSERT(!index);

    sd.labels(boost::none);
    index = sd.indexOf(5.0, PositionMatch::Less);
    CPPUNIT_ASSERT(index && *index == 4);
    index = sd.indexOf(5.0, PositionMatch::LessOrEqual);
    CPPUNIT_ASSERT(index && *index == 5);
    index = sd.indexOf(5.0, PositionMatch::Equal);
    CPPUNIT_ASSERT(index && *index == 5);
    index = sd.indexOf(5.0, PositionMatch::GreaterOrEqual);
    CPPUNIT_ASSERT(index && *index == 5);
    index = sd.indexOf(5.0, PositionMatch::Greater);
    CPPUNIT_ASSERT(index && *index == 6);

    index = sd.indexOf(5.5, PositionMatch::Less);
    CPPUNIT_ASSERT(index && *index == 5);
    index = sd.indexOf(5.5, PositionMatch::LessOrEqual);
    CPPUNIT_ASSERT(index && *index == 5);
    index = sd.indexOf(5.5, PositionMatch::Equal);
    CPPUNIT_ASSERT(!index);
    index = sd.indexOf(5.5, PositionMatch::GreaterOrEqual);
    CPPUNIT_ASSERT(index && *index == 6);
    index = sd.indexOf(5.5, PositionMatch::Greater);
    CPPUNIT_ASSERT(index && *index == 6);

    boost::optional<std::pair<ndsize_t, ndsize_t>> range = sd.indexOf(0.0, 0.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 0);
    range = sd.indexOf(0.0, 0.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(!range);
    range = sd.indexOf(0.0, 3.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 3);
    range = sd.indexOf(0.0, 3.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 2);
    range = sd.indexOf(3.0, 0.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(!range);
    range = sd.indexOf(3.0, 0.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(!range);

    sd.labels(labels);
    range = sd.indexOf(0.0, 0.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 0);
    range = sd.indexOf(1.0, 1.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(!range);
    range = sd.indexOf(0.0, 3.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 3);
    range = sd.indexOf(0.0, 3.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 2);
    range = sd.indexOf(3.0, 0.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(!range);
    range = sd.indexOf(3.0, 0.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(!range);
    range = sd.indexOf(0.0, 7.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 4);
    range = sd.indexOf(3.0, 7.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(range && (*range).first == 3 && (*range).second == 4);

    std::vector<boost::optional<std::pair<ndsize_t, ndsize_t>>> ranges;
    CPPUNIT_ASSERT_THROW(sd.indexOf({0.0, -1.0, 1.0}, {1.0, 2.0}, RangeMatch::Inclusive), std::runtime_error);
    ranges = sd.indexOf({0.0, -1.0, 1.0, 1.0}, {1.0, 4.0, 9.0, 1.0}, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(ranges.size() == 4);
    CPPUNIT_ASSERT(ranges[0] && (*ranges[0]).first == 0 && (*ranges[0]).second == 1);
    CPPUNIT_ASSERT(ranges[1] && (*ranges[1]).first == 0 && (*ranges[1]).second == 4);
    CPPUNIT_ASSERT(ranges[2] && (*ranges[2]).first == 1 && (*ranges[2]).second == 4);
    CPPUNIT_ASSERT(ranges[3] && (*ranges[3]).first == 1 && (*ranges[3]).second == 1);

    ranges = sd.indexOf({0.0, -1.0, 1.0, 1.0}, {1.0, 4.0, 9.0, 1.0}, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(ranges.size() == 4);
    CPPUNIT_ASSERT(ranges[0] && (*ranges[0]).first == 0 && (*ranges[0]).second == 0);
    CPPUNIT_ASSERT(ranges[1] && (*ranges[1]).first == 0 && (*ranges[1]).second == 3);
    CPPUNIT_ASSERT(ranges[2] && (*ranges[2]).first == 1 && (*ranges[2]).second == 4);
    CPPUNIT_ASSERT(!ranges[3]);
}

void BaseTestDimension::testRangeDimLabel() {
    std::string label = "aLabel";
    std::string other_label = "anotherLabel";
    std::vector<double> ticks;
    for (size_t i = 0; i < 5; i++) {
        ticks.push_back(i * boost::math::constants::pi<double>());
    }

    Dimension d = data_array.appendRangeDimension(ticks);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Range);

    RangeDimension rd;
    rd = d;
    rd.label(label);
    CPPUNIT_ASSERT(*(rd.label()) == label);
    rd.label(other_label);
    CPPUNIT_ASSERT(*(rd.label()) == other_label);
    rd.label(none);
    CPPUNIT_ASSERT(rd.label() == none);

    data_array.deleteDimensions();
}


void BaseTestDimension::testRangeDimUnit() {
    std::string invalidUnit = "invalidunit";
    std::string validUnit = "ms";

    std::vector<double> ticks;
    for (size_t i = 0; i < 5; i++) {
        ticks.push_back(i * boost::math::constants::pi<double>());
    }
    Dimension d = data_array.appendRangeDimension(ticks);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Range);

    RangeDimension rd;
    rd = d;
    CPPUNIT_ASSERT_THROW(rd.unit(invalidUnit), InvalidUnit);
    CPPUNIT_ASSERT_NO_THROW(rd.unit(validUnit));
    CPPUNIT_ASSERT(*(rd.unit()) == validUnit);
    CPPUNIT_ASSERT_NO_THROW(rd.unit(none));
    CPPUNIT_ASSERT(rd.unit() == none);

    data_array.deleteDimensions();
}


void BaseTestDimension::testRangeTicks() {
    std::vector<double> ticks = {1.0, 2.0, 3.4, 42.0};
    std::vector<double> new_ticks = {-100.0, -10.0, 0.0, 10.0, 100.0};
    std::vector<double> unordered_ticks = {-20.0, -100.0, 10.0, -10.0, 0.0};
    std::vector<double> double_ticks = {-20.0, -10.0, 10.0, -10.0, -20.0};

    Dimension d = data_array.appendRangeDimension(ticks);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Range);

    RangeDimension rd;
    rd = d;
    CPPUNIT_ASSERT(rd.ticks().size() == ticks.size());
    std::vector<double> retrieved_ticks = rd.ticks();
    CPPUNIT_ASSERT(retrieved_ticks.size() == ticks.size());
    for (size_t i = 0; i < ticks.size(); i++) {
        CPPUNIT_ASSERT(ticks[i] == retrieved_ticks[i]);
    }
    CPPUNIT_ASSERT_THROW(rd.ticks(unordered_ticks), UnsortedTicks);
    CPPUNIT_ASSERT_THROW(rd.ticks(double_ticks), UnsortedTicks);
    rd.ticks(new_ticks);
    retrieved_ticks = rd.ticks();
    CPPUNIT_ASSERT(retrieved_ticks.size() == new_ticks.size());
    for (size_t i = 0; i < new_ticks.size(); i++) {
        CPPUNIT_ASSERT(new_ticks[i] == retrieved_ticks[i]);
    }

    ticks.resize(100);
    for (size_t i = 0; i < 100; ++i) {
        ticks[i] = i * 3.14;
    }
    rd.ticks(ticks);
    CPPUNIT_ASSERT(rd.ticks(0, 10).size() == 10);
    CPPUNIT_ASSERT(rd.ticks(1, 99).size() == 99);
    CPPUNIT_ASSERT(rd.ticks(10, 1)[0] == 10 * 3.14);
    CPPUNIT_ASSERT(rd.ticks(20, 1)[0] == 20 * 3.14);

    CPPUNIT_ASSERT_THROW(rd.ticks(0, ticks.size() + 10), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(rd.ticks(20, ticks.size() - 10), nix::OutOfBounds);
    data_array.deleteDimensions();
}


void BaseTestDimension::testRangeDimIndexOfOld() {
    std::vector<double> ticks = {-100.0, -10.0, 0.0, 10.0, 100.0};
    Dimension d = data_array.appendRangeDimension(ticks);

    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Range);

    RangeDimension rd;
    rd = d;
    CPPUNIT_ASSERT(rd.indexOf(-100.) == 0);
    CPPUNIT_ASSERT(rd.indexOf(-50.) == 0);
    CPPUNIT_ASSERT(rd.indexOf(-70.) == 0);
    CPPUNIT_ASSERT(rd.indexOf(-10.0) == 1);
    CPPUNIT_ASSERT(rd.indexOf(-5.0) == 1);
    CPPUNIT_ASSERT(rd.indexOf(5.0) == 2);

    CPPUNIT_ASSERT_NO_THROW(rd.indexOf(257.28));
    CPPUNIT_ASSERT_THROW(rd.indexOf(257.28, false), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(rd.indexOf(-257.28), nix::OutOfBounds);
    CPPUNIT_ASSERT_NO_THROW(rd.indexOf(-257.28, false));

    CPPUNIT_ASSERT_THROW(rd.indexOf(110., 120.), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(rd.indexOf(-120., -110.), nix::OutOfBounds);
    std::pair<ndsize_t, ndsize_t> range = rd.indexOf(-100., 100.);
    CPPUNIT_ASSERT(range.first == 0 && range.second == 4);
    range = rd.indexOf(-200., 200.);
    CPPUNIT_ASSERT(range.first == 0 && range.second == 4);

    CPPUNIT_ASSERT_THROW(rd.indexOf({-100.0, -90, 0.0}, {10.}), std::runtime_error);
    CPPUNIT_ASSERT_NO_THROW(rd.indexOf({-100.0, 20.0, 40.0}, {-45, 120., 100.}));
    CPPUNIT_ASSERT(rd.indexOf({-100.0, 20.0, 40.0}, {-45, 120., 100.}, false).size() == 3);
    data_array.deleteDimensions();
}


void BaseTestDimension::testRangeDimIndexOf() {
    std::vector<double> ticks = {-100.0, -10.0, 0.0, 10.0, 100.0};
    Dimension d = data_array.appendRangeDimension(ticks);

    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Range);

    RangeDimension rd;
    rd = d;

    CPPUNIT_ASSERT(*rd.indexOf(-110., PositionMatch::GreaterOrEqual) == 0);
    CPPUNIT_ASSERT(*rd.indexOf(-110., PositionMatch::GreaterOrEqual) == 0);
    CPPUNIT_ASSERT(!rd.indexOf(-110., PositionMatch::LessOrEqual));
    CPPUNIT_ASSERT(!rd.indexOf(-110., PositionMatch::Less));
    CPPUNIT_ASSERT(!rd.indexOf(-110., PositionMatch::Equal));

    CPPUNIT_ASSERT(*rd.indexOf(-100., PositionMatch::GreaterOrEqual) == 0);
    CPPUNIT_ASSERT(*rd.indexOf(-100., PositionMatch::Greater) == 1);
    CPPUNIT_ASSERT(*rd.indexOf(-100., PositionMatch::LessOrEqual) == 0);
    CPPUNIT_ASSERT(!rd.indexOf(-100., PositionMatch::Less));
    CPPUNIT_ASSERT(*rd.indexOf(-100., PositionMatch::Equal) == 0);

    CPPUNIT_ASSERT(*rd.indexOf(-50., PositionMatch::GreaterOrEqual) == 1);
    CPPUNIT_ASSERT(*rd.indexOf(-50., PositionMatch::Greater) == 1);
    CPPUNIT_ASSERT(*rd.indexOf(-50., PositionMatch::LessOrEqual) == 0);
    CPPUNIT_ASSERT(*rd.indexOf(-50., PositionMatch::Less) == 0);
    CPPUNIT_ASSERT(!rd.indexOf(-50., PositionMatch::Equal));

    CPPUNIT_ASSERT(*rd.indexOf(7., PositionMatch::GreaterOrEqual) == 3);
    CPPUNIT_ASSERT(*rd.indexOf(7., PositionMatch::Greater) == 3);
    CPPUNIT_ASSERT(*rd.indexOf(7., PositionMatch::LessOrEqual) == 2);
    CPPUNIT_ASSERT(*rd.indexOf(7., PositionMatch::Less) == 2);
    CPPUNIT_ASSERT(!rd.indexOf(7., PositionMatch::Equal));

    CPPUNIT_ASSERT(*rd.indexOf(10., PositionMatch::GreaterOrEqual) == 3);
    CPPUNIT_ASSERT(*rd.indexOf(10., PositionMatch::Greater) == 4);
    CPPUNIT_ASSERT(*rd.indexOf(10., PositionMatch::LessOrEqual) == 3);
    CPPUNIT_ASSERT(*rd.indexOf(10., PositionMatch::Less) == 2);
    CPPUNIT_ASSERT(*rd.indexOf(10., PositionMatch::Equal) == 3);

    CPPUNIT_ASSERT(!rd.indexOf(110., PositionMatch::GreaterOrEqual));
    CPPUNIT_ASSERT(!rd.indexOf(110., PositionMatch::Greater));
    CPPUNIT_ASSERT(*rd.indexOf(110., PositionMatch::LessOrEqual) == 4);
    CPPUNIT_ASSERT(*rd.indexOf(110., PositionMatch::Less) == 4);
    CPPUNIT_ASSERT(!rd.indexOf(110., PositionMatch::Equal));

    boost::optional<std::pair<ndsize_t, ndsize_t>> range;
    range = rd.indexOf(40., 100., {}, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 4 && (*range).second == 4);
    range = rd.indexOf(40., 100., {}, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(!range);
    range = rd.indexOf(-100., -45., {}, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 0);
    range = rd.indexOf(-100., -45., {}, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 0);
    range = rd.indexOf(10., 120., {}, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 3 && (*range).second == 4);
    range = rd.indexOf(10., 120., {}, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(range && (*range).first == 3 && (*range).second == 4);
    range = rd.indexOf(100., 120., {}, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 4 && (*range).second == 4);
    range = rd.indexOf(100., 120., {}, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(range && (*range).first == 4 && (*range).second == 4);
    range = rd.indexOf(110., 150., {}, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(!range);
    range = rd.indexOf(100., -100., {}, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(!range);
    range = rd.indexOf(100., -100., {}, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(!range);

    range = rd.indexOf(100., -100., rd.ticks(), RangeMatch::Exclusive);
    CPPUNIT_ASSERT(!range);

    std::vector<std::pair<ndsize_t, ndsize_t>> ranges;
    ranges = rd.indexOf({40., -100.}, {100., 100.}, true, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(ranges.size() == 2);
    CPPUNIT_ASSERT(ranges[0].first == 4 && ranges[0].second == 4);
    CPPUNIT_ASSERT(ranges[1].first == 0 && ranges[1].second == 4);
    
    CPPUNIT_ASSERT_THROW(rd.indexOf({40., -100., -100.}, {100., 100., 101.}, true, RangeMatch::Exclusive), nix::OutOfBounds);
    
    ranges = rd.indexOf({40., -100., -100.}, {100., 100., 101.}, false, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(ranges.size() == 2);
    CPPUNIT_ASSERT(ranges[0].first == 0 && ranges[0].second == 3);
    CPPUNIT_ASSERT(ranges[1].first == 0 && ranges[1].second == 4);
    
    std::vector<boost::optional<std::pair<ndsize_t, ndsize_t>>> optranges;
    optranges = rd.indexOf({40., -100.}, {100., 100.}, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(optranges.size() == 2);
    CPPUNIT_ASSERT(optranges[0] && (*optranges[0]).first == 4 && (*optranges[0]).second == 4);
    CPPUNIT_ASSERT(optranges[1] && (*optranges[1]).first == 0 && (*optranges[1]).second == 4);

    optranges = rd.indexOf({40., -100., -100.}, {100., 100., 101.}, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(optranges.size() == 3);
    CPPUNIT_ASSERT(!optranges[0]);
    CPPUNIT_ASSERT(optranges[1] && (*optranges[1]).first == 0 && (*optranges[1]).second == 3);
    CPPUNIT_ASSERT(optranges[2] && (*optranges[2]).first == 0 && (*optranges[2]).second == 4);
    
    data_array.deleteDimensions();
}


void BaseTestDimension::testRangeDimTickAt() {
    std::vector<double> ticks = {-100.0, -10.0, 0.0, 10.0, 100.0};
    Dimension d = data_array.appendRangeDimension(ticks);

    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Range);

    RangeDimension rd;
    rd = d;
    CPPUNIT_ASSERT(rd.tickAt(0) == -100.);
    CPPUNIT_ASSERT(rd.tickAt(4) == 100.);
    CPPUNIT_ASSERT_THROW(rd.tickAt(10), OutOfBounds);

    CPPUNIT_ASSERT(rd[0] == -100.);
    CPPUNIT_ASSERT(rd[4] == 100.);
    CPPUNIT_ASSERT_THROW(rd[10], OutOfBounds);

    data_array.deleteDimensions();
}


void BaseTestDimension::testRangeDimAxis() {
    std::vector<double> ticks = {-100.0, -10.0, 0.0, 10.0, 100.0};
    Dimension d = data_array.appendRangeDimension(ticks);

    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Range);

    RangeDimension rd;
    rd = d;

    std::vector<double> axis = rd.axis(2);
    CPPUNIT_ASSERT(axis.size() == 2);
    CPPUNIT_ASSERT(axis[0] == -100.0);
    CPPUNIT_ASSERT(axis[1] == -10.0);

    axis = rd.axis(2, 2);
    CPPUNIT_ASSERT(axis.size() == 2);
    CPPUNIT_ASSERT(axis[0] == 0.0);
    CPPUNIT_ASSERT(axis[1] == 10.0);

    CPPUNIT_ASSERT_THROW(rd.axis(10), OutOfBounds);
    CPPUNIT_ASSERT_THROW(rd.axis(2, 10), OutOfBounds);
    CPPUNIT_ASSERT_THROW(rd.axis(std::numeric_limits<ndsize_t>::max(), static_cast<size_t>(1)), OutOfBounds);
}


void BaseTestDimension::testRangeDimPositionInRange() {
    std::vector<double> ticks = {-100.0, -10.0, 0.0, 10.0, 100.0};
    Dimension d = data_array.appendRangeDimension(ticks);

    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Range);
    RangeDimension rd = d.asRangeDimension();

    CPPUNIT_ASSERT(rd.positionInRange(-99.0) == nix::PositionInRange::InRange);
    CPPUNIT_ASSERT(rd.positionInRange(99.0) == nix::PositionInRange::InRange);
    CPPUNIT_ASSERT(rd.positionInRange(100.1) == nix::PositionInRange::Greater);
    CPPUNIT_ASSERT(rd.positionInRange(-100.1) == nix::PositionInRange::Less);

    rd.ticks({});
    CPPUNIT_ASSERT(rd.positionInRange(0.1) == nix::PositionInRange::NoRange);
}


void BaseTestDimension::testDataFrameDimIndexOf() {
    std::vector<nix::Column> cols = {{"current", "nA", nix::DataType::Double},
                                     {"note", "", nix::DataType::String}};
    nix::DataFrame df = block.createDataFrame("conditions", "test", cols);
    std::vector<nix::Variant> vals(2);
    df.rows(10);
    for (int i = 0; i < 10; ++i) {
        vals[0].set(i * 2.5);
        vals[1].set("test");
        df.writeRow(i, vals);
    }

    Dimension d = data_array.appendDataFrameDimension(df, 0);

    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::DataFrame);
    DataFrameDimension dfDim;
    dfDim = d;

    boost::optional<ndsize_t> pos = dfDim.indexOf(12.2, PositionMatch::GreaterOrEqual);
    CPPUNIT_ASSERT(!pos);
    pos = dfDim.indexOf(12.2, PositionMatch::Greater);
    CPPUNIT_ASSERT(!pos);
    pos = dfDim.indexOf(12.2, PositionMatch::Equal);
    CPPUNIT_ASSERT(!pos);
    pos = dfDim.indexOf(12.2, PositionMatch::LessOrEqual);
    CPPUNIT_ASSERT(pos && *pos == 9);
    pos = dfDim.indexOf(12.2, PositionMatch::Less);
    CPPUNIT_ASSERT(pos && *pos == 9);

    pos = dfDim.indexOf(9, PositionMatch::Greater);
    CPPUNIT_ASSERT(!pos);
    pos = dfDim.indexOf(9, PositionMatch::GreaterOrEqual);
    CPPUNIT_ASSERT(pos && *pos == 9);
    pos = dfDim.indexOf(9, PositionMatch::Equal);
    CPPUNIT_ASSERT(pos && *pos == 9);
    pos = dfDim.indexOf(9, PositionMatch::LessOrEqual);
    CPPUNIT_ASSERT(pos && *pos == 9);
    pos = dfDim.indexOf(9, PositionMatch::Less);
    CPPUNIT_ASSERT(pos && *pos == 8);

    pos = dfDim.indexOf(5, PositionMatch::Greater);
    CPPUNIT_ASSERT(pos && *pos == 6);
    pos = dfDim.indexOf(5, PositionMatch::GreaterOrEqual);
    CPPUNIT_ASSERT(pos && *pos == 5);
    pos = dfDim.indexOf(5, PositionMatch::Equal);
    CPPUNIT_ASSERT(pos && *pos == 5);
    pos = dfDim.indexOf(5, PositionMatch::LessOrEqual);
    CPPUNIT_ASSERT(pos && *pos == 5);
    pos = dfDim.indexOf(5, PositionMatch::Less);
    CPPUNIT_ASSERT(pos && *pos == 4);

    pos = dfDim.indexOf(0.5, PositionMatch::Greater);
    CPPUNIT_ASSERT(pos && *pos == 1);
    pos = dfDim.indexOf(0.5, PositionMatch::GreaterOrEqual);
    CPPUNIT_ASSERT(pos && *pos == 1);
    pos = dfDim.indexOf(0.5, PositionMatch::Equal);
    CPPUNIT_ASSERT(!pos);
    pos = dfDim.indexOf(0.5, PositionMatch::LessOrEqual);
    CPPUNIT_ASSERT(pos && *pos == 0);
    pos = dfDim.indexOf(0.5, PositionMatch::Less);
    CPPUNIT_ASSERT(pos && *pos == 0);

    pos = dfDim.indexOf(-0.5, PositionMatch::Greater);
    CPPUNIT_ASSERT(pos && *pos == 0);
    pos = dfDim.indexOf(-0.5, PositionMatch::GreaterOrEqual);
    CPPUNIT_ASSERT(pos && *pos == 0);
    pos = dfDim.indexOf(-0.5, PositionMatch::Equal);
    CPPUNIT_ASSERT(!pos);
    pos = dfDim.indexOf(-0.5, PositionMatch::LessOrEqual);
    CPPUNIT_ASSERT(!pos);
    pos = dfDim.indexOf(-0.5, PositionMatch::Less);
    CPPUNIT_ASSERT(!pos);
 
    boost::optional<std::pair<ndsize_t, ndsize_t>> range = dfDim.indexOf(0.0, 0.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 0);
    range = dfDim.indexOf(0.0, 0.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(!range);
    range = dfDim.indexOf(0.0, 3.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 3);
    range = dfDim.indexOf(0.0, 3.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 2);
    range = dfDim.indexOf(3.0, 0.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(!range);
    range = dfDim.indexOf(3.0, 0.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(!range);
    
    range = dfDim.indexOf(0.0, 0.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 0);
    range = dfDim.indexOf(1.0, 1.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(!range);
    range = dfDim.indexOf(0.0, 3.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 3);
    range = dfDim.indexOf(0.0, 3.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 2);
    range = dfDim.indexOf(3.0, 0.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(!range);
    range = dfDim.indexOf(3.0, 0.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(!range);
    range = dfDim.indexOf(0.0, 9.0, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(range && (*range).first == 0 && (*range).second == 9);
    range = dfDim.indexOf(3.0, 9.0, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(range && (*range).first == 3 && (*range).second == 8);

    std::vector<boost::optional<std::pair<ndsize_t, ndsize_t>>> ranges;
    CPPUNIT_ASSERT_THROW(dfDim.indexOf({0.0, -1.0, 1.0}, {1.0, 2.0}, RangeMatch::Inclusive), std::runtime_error);
    ranges = dfDim.indexOf({0.0, -1.0, 1.0, 1.0}, {1.0, 4.0, 9.0, 1.0}, RangeMatch::Inclusive);
    CPPUNIT_ASSERT(ranges.size() == 4);
    CPPUNIT_ASSERT(ranges[0] && (*ranges[0]).first == 0 && (*ranges[0]).second == 1);
    CPPUNIT_ASSERT(ranges[1] && (*ranges[1]).first == 0 && (*ranges[1]).second == 4);
    CPPUNIT_ASSERT(ranges[2] && (*ranges[2]).first == 1 && (*ranges[2]).second == 9);
    CPPUNIT_ASSERT(ranges[3] && (*ranges[3]).first == 1 && (*ranges[3]).second == 1);

    ranges = dfDim.indexOf({0.0, -1.0, 1.0, 1.0}, {1.0, 4.0, 9.0, 1.0}, RangeMatch::Exclusive);
    CPPUNIT_ASSERT(ranges.size() == 4);
    CPPUNIT_ASSERT(ranges[0] && (*ranges[0]).first == 0 && (*ranges[0]).second == 0);
    CPPUNIT_ASSERT(ranges[1] && (*ranges[1]).first == 0 && (*ranges[1]).second == 3);
    CPPUNIT_ASSERT(ranges[2] && (*ranges[2]).first == 1 && (*ranges[2]).second == 8);
    CPPUNIT_ASSERT(!ranges[3]);
}

void BaseTestDimension::testAsDimensionMethods() {
    std::vector<double> ticks = {-100.0, -10.0, 0.0, 10.0, 100.0};
    Dimension x;
    Dimension d = data_array.appendRangeDimension(ticks);
    CPPUNIT_ASSERT_THROW(d.asSampledDimension(), IncompatibleDimensions);
    CPPUNIT_ASSERT_THROW(d.asSetDimension(), IncompatibleDimensions);
    x = d;
    CPPUNIT_ASSERT(x.dimensionType() == DimensionType::Range);
    std::stringstream sa_str;
    sa_str << x.dimensionType();
    CPPUNIT_ASSERT(sa_str.str() == "Range");

    data_array.deleteDimensions();
    d = data_array.appendSampledDimension(0.1);
    CPPUNIT_ASSERT_THROW(d.asRangeDimension(), IncompatibleDimensions);
    CPPUNIT_ASSERT_THROW(d.asSetDimension(), IncompatibleDimensions);
    x = d;
    CPPUNIT_ASSERT(x.dimensionType() == DimensionType::Sample);
    std::stringstream range_str;
    range_str << x.dimensionType();
    CPPUNIT_ASSERT(range_str.str() == "Sample");

    data_array.deleteDimensions();
    d = data_array.appendSetDimension();
    CPPUNIT_ASSERT_THROW(d.asRangeDimension(), IncompatibleDimensions);
    CPPUNIT_ASSERT_THROW(d.asSampledDimension(), IncompatibleDimensions);
    x = d;
    CPPUNIT_ASSERT(x.dimensionType() == DimensionType::Set);
    std::stringstream set_str;
    set_str << x.dimensionType();
    CPPUNIT_ASSERT(set_str.str() == "Set");
}
