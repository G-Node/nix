// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in Section and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>


#include "TestDimension.hpp"
#include <nix/util/util.hpp>

using namespace std;
using namespace nix;


void TestDimension::setUp() {
    file = File::open("test_dimension.h5", FileMode::Overwrite);
    block = file.createBlock("dimensionTest","test");
    data_array = block.createDataArray("dimensionTest", "Test");
}


void TestDimension::tearDown() {
    file.deleteBlock(block.id());
    file.close();
}


void TestDimension::testId() {
    Dimension sd = data_array.appendSetDimension();
    CPPUNIT_ASSERT(data_array.dimensionCount() == 1 && sd.id() == 1);
    data_array.deleteDimension(sd.id());
    CPPUNIT_ASSERT(data_array.dimensionCount() == 0);
}


void TestDimension::testSampledDimLabel() {
    std::string label = "aLabel";
    std::string other_label = "anotherLabel";    
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == nix::DimensionType::Sample);

    SampledDimension sd;
    sd = d;
    sd.label(label);
    CPPUNIT_ASSERT(*(sd.label()) == label);
    sd.label(other_label);
    CPPUNIT_ASSERT(*(sd.label()) == other_label);
    CPPUNIT_ASSERT_NO_THROW(sd.label(nix::none));
    CPPUNIT_ASSERT(sd.label() == nix::none);

    data_array.deleteDimension(d.id());
}


void TestDimension::testSampledDimUnit() {
    std::string invalidUnit = "invalidunit";
    std::string validUnit = "mV^2";
    double samplingInterval = boost::math::constants::pi<double>();
    
    Dimension d = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == nix::DimensionType::Sample);
    
    SampledDimension sd;
    sd = d;
    CPPUNIT_ASSERT_THROW(sd.unit(invalidUnit), nix::InvalidUnit);
    CPPUNIT_ASSERT_NO_THROW(sd.unit(validUnit));
    CPPUNIT_ASSERT(*(sd.unit()) == validUnit);
    CPPUNIT_ASSERT_NO_THROW(sd.unit(boost::none));
    CPPUNIT_ASSERT(sd.unit() == boost::none);

    data_array.deleteDimension(d.id());
}


void TestDimension::testSampledDimSamplingInterval() {
    double impossible_sampling_interval = -1.0;
    double invalid_sampling_interval = 0.0;
    double samplingInterval = boost::math::constants::pi<double>();
        
    Dimension d = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == nix::DimensionType::Sample);
    
    SampledDimension sd;
    sd = d;
    CPPUNIT_ASSERT(sd.samplingInterval() == boost::math::constants::pi<double>());
    CPPUNIT_ASSERT_THROW(sd.samplingInterval(impossible_sampling_interval), std::runtime_error);
    CPPUNIT_ASSERT_THROW(sd.samplingInterval(invalid_sampling_interval), std::runtime_error);
    CPPUNIT_ASSERT_NO_THROW(sd.samplingInterval(samplingInterval));
    CPPUNIT_ASSERT(sd.samplingInterval() == samplingInterval);

    data_array.deleteDimension(d.id());
}


void TestDimension::testSampledDimOffset() {
    double offset = 1.0;
    double samplingInterval = boost::math::constants::pi<double>();
        
    Dimension d = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == nix::DimensionType::Sample);
    
    SampledDimension sd;
    sd = d;
    CPPUNIT_ASSERT_NO_THROW(sd.offset(offset));
    CPPUNIT_ASSERT(*(sd.offset()) == offset);
    CPPUNIT_ASSERT_NO_THROW(sd.offset(boost::none));
    CPPUNIT_ASSERT(sd.offset() == 0);

    data_array.deleteDimension(d.id());
}


void TestDimension::testSampledDimOperators() {
    double samplingInterval = boost::math::constants::pi<double>();

    Dimension d = data_array.appendSampledDimension(samplingInterval);
    Dimension d2 = data_array.appendSampledDimension(samplingInterval);
    CPPUNIT_ASSERT(d.dimensionType() == nix::DimensionType::Sample);
    CPPUNIT_ASSERT(d2.dimensionType() == nix::DimensionType::Sample);
    
    SampledDimension sd1, sd2, sd3;
    sd1 = d;
    sd2 = d2;
    sd3 = data_array.getDimension(d.id());
    CPPUNIT_ASSERT(sd1.id() == d.id() && sd2.id() == d2.id());
    CPPUNIT_ASSERT(sd1 != sd2);
    CPPUNIT_ASSERT(sd1 != sd3);

    data_array.deleteDimension(d.id());
    data_array.deleteDimension(d2.id());
}


void TestDimension::testSetDimLabels() {
    std::vector<std::string> labels = {"label_a", "label_b","label_c","label_d","label_e"};
    std::vector<std::string> new_labels = {"new label_a", "new label_b","new label_c"};

    Dimension d = data_array.appendSetDimension();
    CPPUNIT_ASSERT(d.dimensionType() == nix::DimensionType::Set);

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

    data_array.deleteDimension(d.id());
}


void TestDimension::testRangeDimLabel() {
    std::string label = "aLabel";
    std::string other_label = "anotherLabel";
    std::vector<double> ticks;
    for (size_t i = 0; i < 5; i++) {
        ticks.push_back(i * boost::math::constants::pi<double>());
    }

    Dimension d = data_array.appendRangeDimension(ticks);
    CPPUNIT_ASSERT(d.dimensionType() == nix::DimensionType::Range);

    RangeDimension rd;
    rd = d;
    rd.label(label);
    CPPUNIT_ASSERT(*(rd.label()) == label);
    rd.label(other_label);
    CPPUNIT_ASSERT(*(rd.label()) == other_label);
    rd.label(nix::none);
    CPPUNIT_ASSERT(rd.label() == nix::none);

    data_array.deleteDimension(d.id());
}


void TestDimension::testRangeDimUnit() {
    std::string invalidUnit = "invalidunit";
    std::string validUnit = "ms";
    
    std::vector<double> ticks;
    for (size_t i = 0; i < 5; i++) {
        ticks.push_back(i * boost::math::constants::pi<double>());
    }
    Dimension d = data_array.appendRangeDimension(ticks);
    CPPUNIT_ASSERT(d.dimensionType() == nix::DimensionType::Range);
    
    RangeDimension rd;
    rd = d;
    CPPUNIT_ASSERT_THROW(rd.unit(invalidUnit), nix::InvalidUnit);
    CPPUNIT_ASSERT_NO_THROW(rd.unit(validUnit));
    CPPUNIT_ASSERT(*(rd.unit()) == validUnit);
    CPPUNIT_ASSERT_NO_THROW(rd.unit(nix::none));
    CPPUNIT_ASSERT(rd.unit() == nix::none);
    
    data_array.deleteDimension(d.id());
}


void TestDimension::testRangeTicks() {
    std::vector<double> ticks = {1.0, 2.0, 3.4, 42.0};
    std::vector<double> new_ticks = {-100.0, -10.0, 0.0, 10.0, 100.0};
    std::vector<double> unordered_ticks = {-20.0, -100.0, 10.0, -10.0, 0.0};
    std::vector<double> double_ticks = {-20.0, -10.0, 10.0, -10.0, -20.0};

    Dimension d = data_array.appendRangeDimension(ticks);
    CPPUNIT_ASSERT(d.dimensionType() == nix::DimensionType::Range);

    RangeDimension rd;
    rd = d;
    CPPUNIT_ASSERT(rd.ticks().size() == ticks.size());
    std::vector<double> retrieved_ticks = rd.ticks();
    CPPUNIT_ASSERT(retrieved_ticks.size() == ticks.size());
    for (size_t i = 0; i < ticks.size(); i++){
        CPPUNIT_ASSERT(ticks[i] == retrieved_ticks[i]);
    }
    CPPUNIT_ASSERT_THROW(rd.ticks(unordered_ticks), nix::UnsortedTicks);
    CPPUNIT_ASSERT_THROW(rd.ticks(double_ticks), nix::UnsortedTicks);
    rd.ticks(new_ticks);
    retrieved_ticks = rd.ticks();
    CPPUNIT_ASSERT(retrieved_ticks.size() == new_ticks.size());
    for (size_t i = 0; i < new_ticks.size(); i++){
        CPPUNIT_ASSERT(new_ticks[i] == retrieved_ticks[i]);
    }

    data_array.deleteDimension(d.id());
}
