// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in Section and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <limits>

#include "TestDimension.hpp"

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>

using namespace std;
using namespace nix;
using namespace valid;


void TestDimension::setUp() {
    file = File::open("test_dimension.h5", FileMode::Overwrite);
    block = file.createBlock("dimensionTest","test");
    data_array = block.createDataArray("dimensionTest", "Test",
                                       DataType::Double, NDSize({ 0 }));
}


void TestDimension::tearDown() {
    file.deleteBlock(block.id());
    file.close();
}


void TestDimension::testValidate() {
    Dimension d = data_array.appendSetDimension();
    
    valid::Result result = validate(d);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void TestDimension::testSetValidate() {
    SetDimension d = data_array.appendSetDimension();
    
    valid::Result result = validate(d);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void TestDimension::testRangeValidate() {
    std::vector<double> ticks;
    for (size_t i = 0; i < 5; i++) {
        ticks.push_back(i * boost::math::constants::pi<double>());
    }
    
    RangeDimension d = data_array.appendRangeDimension(ticks);
    
    valid::Result result = validate(d);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void TestDimension::testSampleValidate() {
    double samplingInterval = boost::math::constants::pi<double>();
    
    SampledDimension d = data_array.appendSampledDimension(samplingInterval);
    
    valid::Result result = validate(d);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void TestDimension::testIndex() {
    Dimension sd = data_array.appendSetDimension();
    CPPUNIT_ASSERT(data_array.dimensionCount() == 1 && sd.index() == 1);
    data_array.deleteDimension(sd.index());
    CPPUNIT_ASSERT(data_array.dimensionCount() == 0);
}


void TestDimension::testSampledDimLabel() {
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

    data_array.deleteDimension(d.index());
}


void TestDimension::testSampledDimUnit() {
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

    data_array.deleteDimension(d.index());
}


void TestDimension::testSampledDimSamplingInterval() {
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

    data_array.deleteDimension(d.index());
}


void TestDimension::testSampledDimOffset() {
    double offset = 1.0;
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);

    SampledDimension sd;
    sd = d;
    CPPUNIT_ASSERT_NO_THROW(sd.offset(offset));
    CPPUNIT_ASSERT(*(sd.offset()) == offset);
    CPPUNIT_ASSERT_NO_THROW(sd.offset(boost::none));
    CPPUNIT_ASSERT(sd.offset() == 0);

    data_array.deleteDimension(d.index());
}


void TestDimension::testSampledDimIndexOf() {
    double offset = 1.0;
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);

    SampledDimension sd;
    sd = d;
    CPPUNIT_ASSERT_THROW(sd.indexOf(-3.14), nix::OutOfBounds);
    CPPUNIT_ASSERT_NO_THROW(sd.indexOf(-1.14));
    CPPUNIT_ASSERT(sd.indexOf(3.14) == 1);
    CPPUNIT_ASSERT(sd.indexOf(6.28) == 2);
    CPPUNIT_ASSERT(sd.indexOf(4.28) == 1);
    CPPUNIT_ASSERT(sd.indexOf(7.28) == 2);
    
    sd.offset(offset);
    CPPUNIT_ASSERT(*(sd.offset()) == offset);
    CPPUNIT_ASSERT(sd.indexOf(-1 * samplingInterval / 2 + offset + 0.001) == 0);
    CPPUNIT_ASSERT_THROW(sd.indexOf(-3.14), nix::OutOfBounds);
    CPPUNIT_ASSERT(sd.indexOf(2.14) == 0);
    CPPUNIT_ASSERT(sd.indexOf(6.28) == 2);
    CPPUNIT_ASSERT(sd.indexOf(4.28) == 1);
    CPPUNIT_ASSERT(sd.indexOf(7.28) == 2);
    
    data_array.deleteDimension(d.index());
}


void TestDimension::testSampledDimPositionAt() {
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

    data_array.deleteDimension(d.index());
}


void TestDimension::testSampledDimAxis() {
    double offset = 1.0;
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);

    SampledDimension sd;
    sd = d;
    sd.offset(offset);

    vector<double> axis = sd.axis(100);
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
    
    data_array.deleteDimension(d.index());
}


void TestDimension::testSampledDimOperators() {
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = data_array.appendSampledDimension(samplingInterval);
    Dimension d2 = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);
    CPPUNIT_ASSERT(d2.dimensionType() == DimensionType::Sample);

    SampledDimension sd1, sd2, sd3;
    sd1 = d;
    sd2 = d2;
    sd3 = data_array.getDimension(d.index());
    CPPUNIT_ASSERT(sd1.index() == d.index() && sd2.index() == d2.index());
    CPPUNIT_ASSERT(sd1 != sd2);
    CPPUNIT_ASSERT(sd1 != sd3);

    data_array.deleteDimension(d.index());
    data_array.deleteDimension(d2.index());

    Dimension dim = data_array.appendSampledDimension(samplingInterval);
    SampledDimension sampled = data_array.appendSampledDimension(samplingInterval);
    RangeDimension range = data_array.appendRangeDimension(std::vector<double>({1, 2}));
    SetDimension set = data_array.appendSetDimension();
    CPPUNIT_ASSERT(d != none);
    CPPUNIT_ASSERT(sampled != none);
    CPPUNIT_ASSERT(range != none);
    CPPUNIT_ASSERT(set != none);
    d = none; sampled = none; range = none; set = none;
    CPPUNIT_ASSERT(d == none);
    CPPUNIT_ASSERT(sampled == none);
    CPPUNIT_ASSERT(range == none);
    CPPUNIT_ASSERT(set == none);
}


void TestDimension::testSetDimLabels() {
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

    data_array.deleteDimension(d.index());
}


void TestDimension::testRangeDimLabel() {
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

    data_array.deleteDimension(d.index());
}


void TestDimension::testRangeDimUnit() {
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

    data_array.deleteDimension(d.index());
}


void TestDimension::testRangeTicks() {
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
    for (size_t i = 0; i < ticks.size(); i++){
        CPPUNIT_ASSERT(ticks[i] == retrieved_ticks[i]);
    }
    CPPUNIT_ASSERT_THROW(rd.ticks(unordered_ticks), UnsortedTicks);
    CPPUNIT_ASSERT_THROW(rd.ticks(double_ticks), UnsortedTicks);
    rd.ticks(new_ticks);
    retrieved_ticks = rd.ticks();
    CPPUNIT_ASSERT(retrieved_ticks.size() == new_ticks.size());
    for (size_t i = 0; i < new_ticks.size(); i++){
        CPPUNIT_ASSERT(new_ticks[i] == retrieved_ticks[i]);
    }

    data_array.deleteDimension(d.index());
}

void TestDimension::testRangeDimIndexOf() {
    std::vector<double> ticks = {-100.0, -10.0, 0.0, 10.0, 100.0};
    Dimension d = data_array.appendRangeDimension(ticks);
   
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Range);

    RangeDimension rd;
    rd = d;
    CPPUNIT_ASSERT(rd.indexOf(-100.) == 0);
    CPPUNIT_ASSERT(rd.indexOf(-50.) == 1);
    CPPUNIT_ASSERT(rd.indexOf(-70.) == 0);
    CPPUNIT_ASSERT(rd.indexOf(5.0) == 2);
    CPPUNIT_ASSERT(rd.indexOf(257.28) == 4);
    CPPUNIT_ASSERT(rd.indexOf(-257.28) == 0);

    data_array.deleteDimension(d.index());
}

void TestDimension::testRangeDimTickAt() {
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

    data_array.deleteDimension(d.index());
}

void TestDimension::testRangeDimAxis() {
    std::vector<double> ticks = {-100.0, -10.0, 0.0, 10.0, 100.0};
    Dimension d = data_array.appendRangeDimension(ticks);
   
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Range);

    RangeDimension rd;
    rd = d;
    
    vector<double> axis = rd.axis(2);
    CPPUNIT_ASSERT(axis.size() == 2);
    CPPUNIT_ASSERT(axis[0] == -100.0);
    CPPUNIT_ASSERT(axis[1] == -10.0);
    
    axis = rd.axis(2, 2);
    CPPUNIT_ASSERT(axis.size() == 2);
    CPPUNIT_ASSERT(axis[0] == 0.0);
    CPPUNIT_ASSERT(axis[1] == 10.0);

    CPPUNIT_ASSERT_THROW(rd.axis(10), OutOfBounds);
    CPPUNIT_ASSERT_THROW(rd.axis(2, 10), OutOfBounds);
}
