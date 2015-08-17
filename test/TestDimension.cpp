// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in Section and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <limits>
#include <sstream>

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

    file_fs = File::open("test_dimension", FileMode::Overwrite, Implementation::FileSys);
    block_fs = file_fs.createBlock("dimensionTest", "test");
    data_array_fs = block_fs.createDataArray("dimensionTest", "test", DataType::Double, NDSize({ 0 }));
}


void TestDimension::tearDown() {
    file.deleteBlock(block.id());
    file.close();
}


void TestDimension::testValidate() {
    test_validate(data_array);
    test_validate(data_array_fs);
}


void TestDimension::test_validate(DataArray &a) {
    Dimension d = a.appendSetDimension();
    valid::Result result = validate(d);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void TestDimension::testSetValidate() {
    test_set_validate(data_array);
    test_set_validate(data_array_fs);
}


void TestDimension::test_set_validate(DataArray &a) {
    SetDimension d = a.appendSetDimension();
    valid::Result result = validate(d);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void TestDimension::testRangeValidate() {
    test_range_validate(data_array);
    // test_range_validate(data_array_fs); FIXME
}


void TestDimension::test_range_validate(DataArray &a) {
    std::vector<double> ticks;
    for (size_t i = 0; i < 5; i++) {
        ticks.push_back(i * boost::math::constants::pi<double>());
    }
    RangeDimension d = a.appendRangeDimension(ticks);
    valid::Result result = validate(d);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void TestDimension::testSampleValidate() {
    test_sample_validate(data_array);
    test_sample_validate(data_array_fs);
}


void TestDimension::test_sample_validate(DataArray &a) {
    double samplingInterval = boost::math::constants::pi<double>();
    SampledDimension d = a.appendSampledDimension(samplingInterval);
    valid::Result result = validate(d);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void TestDimension::testIndex() {
    test_index(data_array);
    test_index(data_array_fs);
}

void TestDimension::test_index(DataArray &a) {
    Dimension sd = a.appendSetDimension();
    CPPUNIT_ASSERT(a.dimensionCount() == 1 && sd.index() == 1);
    a.deleteDimension(sd.index());
    CPPUNIT_ASSERT(a.dimensionCount() == 0);
}


void TestDimension::testSampledDimLabel() {
    test_sample_dim_label(data_array);
    test_sample_dim_label(data_array_fs);
}

void TestDimension::test_sample_dim_label(nix::DataArray &a) {
    std::string label = "aLabel";
    std::string other_label = "anotherLabel";
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = a.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);

    SampledDimension sd;
    sd = d;
    sd.label(label);
    CPPUNIT_ASSERT(*(sd.label()) == label);
    sd.label(other_label);
    CPPUNIT_ASSERT(*(sd.label()) == other_label);
    CPPUNIT_ASSERT_NO_THROW(sd.label(none));
    CPPUNIT_ASSERT(sd.label() == none);

    a.deleteDimension(d.index());
}


void TestDimension::testSampledDimUnit() {
    test_sample_dim_unit(data_array);
    test_sample_dim_unit(data_array_fs);
}

void TestDimension::test_sample_dim_unit(nix::DataArray &a) {
    std::string invalidUnit = "invalidunit";
    std::string validUnit = "mV^2";
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = a.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);

    SampledDimension sd;
    sd = d;
    CPPUNIT_ASSERT_THROW(sd.unit(invalidUnit), InvalidUnit);
    CPPUNIT_ASSERT_NO_THROW(sd.unit(validUnit));
    CPPUNIT_ASSERT(*(sd.unit()) == validUnit);
    CPPUNIT_ASSERT_NO_THROW(sd.unit(boost::none));
    CPPUNIT_ASSERT(sd.unit() == boost::none);
    a.deleteDimension(d.index());
}


void TestDimension::testSampledDimSamplingInterval() {
    test_sample_dim_sampling_interval(data_array);
    test_sample_dim_sampling_interval(data_array_fs);
}

void TestDimension::test_sample_dim_sampling_interval(nix::DataArray &a) {
    double impossible_sampling_interval = -1.0;
    double invalid_sampling_interval = 0.0;
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = a.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);

    SampledDimension sd;
    sd = d;
    CPPUNIT_ASSERT(sd.samplingInterval() == boost::math::constants::pi<double>());
    CPPUNIT_ASSERT_THROW(sd.samplingInterval(impossible_sampling_interval), std::runtime_error);
    CPPUNIT_ASSERT_THROW(sd.samplingInterval(invalid_sampling_interval), std::runtime_error);
    CPPUNIT_ASSERT_NO_THROW(sd.samplingInterval(samplingInterval));
    CPPUNIT_ASSERT(sd.samplingInterval() == samplingInterval);

    a.deleteDimension(d.index());
}


void TestDimension::testSampledDimOffset() {
    test_sample_dim_offset(data_array);
    test_sample_dim_offset(data_array_fs);
}

void TestDimension::test_sample_dim_offset(DataArray &a) {
    double offset = 1.0;
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = a.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);

    SampledDimension sd;
    sd = d;
    CPPUNIT_ASSERT_NO_THROW(sd.offset(offset));
    CPPUNIT_ASSERT(*(sd.offset()) == offset);
    CPPUNIT_ASSERT_NO_THROW(sd.offset(boost::none));
    CPPUNIT_ASSERT(sd.offset() == boost::none);

    a.deleteDimension(d.index());
}


void TestDimension::testSampledDimIndexOf() {
    test_sample_dim_index_of(data_array);
    test_sample_dim_index_of(data_array_fs);
}

void TestDimension::test_sample_dim_index_of(nix::DataArray &a) {
    double offset = 1.0;
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = a.appendSampledDimension(samplingInterval);
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

    a.deleteDimension(d.index());
}


void TestDimension::testSampledDimPositionAt() {
    test_sample_dim_position_at(data_array);
    test_sample_dim_position_at(data_array_fs);
}

void TestDimension::test_sample_dim_position_at(nix::DataArray &a) {
    double offset = 1.0;
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = a.appendSampledDimension(samplingInterval);
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

    a.deleteDimension(d.index());
}


void TestDimension::testSampledDimAxis() {
    test_sample_dim_axis(data_array);
    test_sample_dim_axis(data_array_fs);
}

void TestDimension::test_sample_dim_axis(nix::DataArray &a) {
    double offset = 1.0;
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = a.appendSampledDimension(samplingInterval);
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

    a.deleteDimension(d.index());
}


void TestDimension::testSampledDimOperators() {

}

void TestDimension::test_sample_dim_operators(nix::DataArray &a) {
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = a.appendSampledDimension(samplingInterval);
    Dimension d2 = a.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == DimensionType::Sample);
    CPPUNIT_ASSERT(d2.dimensionType() == DimensionType::Sample);

    SampledDimension sd1, sd2, sd3;
    sd1 = d;
    sd2 = d2;
    sd3 = a.getDimension(d.index());
    CPPUNIT_ASSERT(sd1.index() == d.index() && sd2.index() == d2.index());
    CPPUNIT_ASSERT(sd1 != sd2);
    CPPUNIT_ASSERT(sd1 != sd3);

    a.deleteDimension(d.index());
    a.deleteDimension(d2.index());

    Dimension dim = a.appendSampledDimension(samplingInterval);
    SampledDimension sampled = a.appendSampledDimension(samplingInterval);
    RangeDimension range = a.appendRangeDimension(std::vector<double>({1, 2}));
    SetDimension set = a.appendSetDimension();

    stringstream s_stream, r_stream, set_stream;
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

    sd.labels(boost::none);
    retrieved_labels = sd.labels();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), retrieved_labels.size());

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
    CPPUNIT_ASSERT(rd.indexOf(-70.) == 1);
    CPPUNIT_ASSERT(rd.indexOf(5.0) == 3);
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
