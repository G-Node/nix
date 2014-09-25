// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestDataAccess.hpp"

using namespace std;
using namespace nix;

void TestDataAccess::setUp() {
    file = File::open("test_dataAccess.h5", FileMode::Overwrite);
    block = file.createBlock("dimensionTest","test");
    data_array = block.createDataArray("dimensionTest",
                                       "test",
                                       DataType::Double,
                                       NDSize({0, 0, 0}));
    double samplingInterval = 1.0;
    vector<double> ticks {1.2, 2.3, 3.4, 4.5, 6.7};
    string unit = "ms";

    typedef boost::multi_array<double, 3> array_type;
    typedef array_type::index index;
    array_type data(boost::extents[2][10][5]);
    int value;
    for(index i = 0; i != 2; ++i) {
        value = 0;
        for(index j = 0; j != 10; ++j) {
            for(index k = 0; k != 5; ++k) {
                data[i][j][k] = value++;
            }
        }
    }
    data_array.setData(data);

    setDim = data_array.appendSetDimension();
    std::vector<std::string> labels = {"label_a", "label_b"};
    setDim.labels(labels);

    sampledDim = data_array.appendSampledDimension(samplingInterval);
    sampledDim.unit(unit);

    rangeDim = data_array.appendRangeDimension(ticks);
    rangeDim.unit(unit);

    vector<DataArray> refs;
    refs.push_back(data_array);
    vector<double> position {0.0, 2.0, 3.4};
    vector<double> extent {0.0, 6.0, 2.3};
    vector<string> units {"none", "ms", "ms"};

    position_tag = block.createTag("position tag", "event", position);
    position_tag.references(refs);
    position_tag.units(units);

    segment_tag = block.createTag("region tag", "segment", position);
    segment_tag.references(refs);
    segment_tag.extent(extent);
    segment_tag.units(units);

    //setup multiTag
    typedef boost::multi_array<double, 2> position_type;
    position_type event_positions(boost::extents[2][3]);
    position_type event_extents(boost::extents[2][3]);
    event_positions[0][0] = 0.0;
    event_positions[0][1] = 3.0;
    event_positions[0][2] = 3.4;

    event_extents[0][0] = 0.0;
    event_extents[0][1] = 6.0;
    event_extents[0][2] = 2.3;

    event_positions[1][0] = 0.0;
    event_positions[1][1] = 8.0;
    event_positions[1][2] = 2.3;

    event_extents[1][0] = 0.0;
    event_extents[1][1] = 3.0;
    event_extents[1][2] = 2.0;

    std::vector<std::string> event_labels = {"event 1", "event 2"};
    std::vector<std::string> dim_labels = {"dim 0", "dim 1", "dim 2"};

    DataArray event_array = block.createDataArray("positions", "test",
                                                  DataType::Double, NDSize({ 0, 0 }));
    event_array.setData(event_positions);
    SetDimension event_set_dim;
    event_set_dim = event_array.appendSetDimension();
    event_set_dim.labels(event_labels);
    event_set_dim = event_array.appendSetDimension();
    event_set_dim.labels(dim_labels);

    DataArray extent_array = block.createDataArray("extents", "test",
                                                   DataType::Double, NDSize({ 0, 0 }));
    extent_array.setData(event_extents);
    SetDimension extent_set_dim;
    extent_set_dim = extent_array.appendSetDimension();
    extent_set_dim.labels(event_labels);
    extent_set_dim = extent_array.appendSetDimension();
    extent_set_dim.labels(dim_labels);

    multi_tag = block.createMultiTag("multi_tag", "events", event_array);
    multi_tag.extents(extent_array);
    multi_tag.addReference(data_array);
}


void TestDataAccess::tearDown() {
    file.close();
}


void TestDataAccess::testPositionToIndexRangeDimension() {
    string unit = "ms";
    string invalid_unit = "kV";
    string scaled_unit = "s";

    CPPUNIT_ASSERT_THROW(util::positionToIndex(5.0, invalid_unit, rangeDim), nix::IncompatibleDimensions);
    CPPUNIT_ASSERT(util::positionToIndex(1.0, unit, rangeDim) == 0);
    CPPUNIT_ASSERT(util::positionToIndex(8.0, unit, rangeDim) == 4);
    CPPUNIT_ASSERT(util::positionToIndex(0.001, scaled_unit, rangeDim) == 0);
    CPPUNIT_ASSERT(util::positionToIndex(0.008, scaled_unit, rangeDim) == 4);
    CPPUNIT_ASSERT(util::positionToIndex(3.4, unit, rangeDim) == 2);
    CPPUNIT_ASSERT(util::positionToIndex(3.6, unit, rangeDim) == 2);
    CPPUNIT_ASSERT(util::positionToIndex(4.0, unit, rangeDim) == 3);
    CPPUNIT_ASSERT(util::positionToIndex(0.0036, scaled_unit, rangeDim) == 2);
}


void TestDataAccess::testPositionToIndexSampledDimension() {
    string unit = "ms";
    string invalid_unit = "kV";
    string scaled_unit = "s";

    CPPUNIT_ASSERT_THROW(util::positionToIndex(-1.0, unit, sampledDim), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(util::positionToIndex(0.005, invalid_unit, sampledDim), nix::IncompatibleDimensions);
    CPPUNIT_ASSERT(util::positionToIndex(5.0, unit, sampledDim) == 5);
    CPPUNIT_ASSERT(util::positionToIndex(0.005, scaled_unit, sampledDim) == 5);
}


void TestDataAccess::testPositionToIndexSetDimension() {
    string unit = "ms";

    CPPUNIT_ASSERT_THROW(util::positionToIndex(5.8, "none", setDim), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(util::positionToIndex(0.5, unit, setDim), nix::IncompatibleDimensions);
    CPPUNIT_ASSERT_NO_THROW(util::positionToIndex(0.5, "none", setDim));
    CPPUNIT_ASSERT(util::positionToIndex(0.5, "none", setDim) == 1);
    CPPUNIT_ASSERT(util::positionToIndex(0.45, "none", setDim) == 0);
}


void TestDataAccess::testOffsetAndCount() {
    NDSize offsets, counts;
    util::getOffsetAndCount(position_tag, data_array, offsets, counts);

    CPPUNIT_ASSERT(offsets.size() == 3);
    CPPUNIT_ASSERT(counts.size() == 3);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 2 && offsets[2] == 2);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 1 && counts[2] == 1);

    util::getOffsetAndCount(segment_tag, data_array, offsets, counts);
    CPPUNIT_ASSERT(offsets.size() == 3);
    CPPUNIT_ASSERT(counts.size() == 3);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 2 && offsets[2] == 2);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 7 && counts[2] == 3);

    CPPUNIT_ASSERT_THROW(util::getOffsetAndCount(multi_tag, data_array, -1, offsets, counts), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(util::getOffsetAndCount(multi_tag, data_array, 3, offsets, counts), nix::OutOfBounds);

    util::getOffsetAndCount(multi_tag, data_array, 0, offsets, counts);
    CPPUNIT_ASSERT(offsets.size() == 3);
    CPPUNIT_ASSERT(counts.size() == 3);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 3 && offsets[2] == 2);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 7 && counts[2] == 3);

    util::getOffsetAndCount(multi_tag, data_array, 1, offsets, counts);
    CPPUNIT_ASSERT(offsets.size() == 3);
    CPPUNIT_ASSERT(counts.size() == 3);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 8 && offsets[2] == 1);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 4 && counts[2] == 3);
}


void TestDataAccess::testPositionInData() {
    NDSize offsets, counts;
    util::getOffsetAndCount(multi_tag, data_array, 0, offsets, counts);
    CPPUNIT_ASSERT(util::positionInData(data_array, offsets));
    CPPUNIT_ASSERT(util::positionAndExtentInData(data_array, offsets, counts));

    util::getOffsetAndCount(multi_tag, data_array, 1, offsets, counts);
    CPPUNIT_ASSERT(util::positionInData(data_array, offsets));
    CPPUNIT_ASSERT(!util::positionAndExtentInData(data_array, offsets, counts));
}

void TestDataAccess::testRetrieveData() {
    CPPUNIT_ASSERT_THROW(util::retrieveData(multi_tag, 0, -1), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(util::retrieveData(multi_tag, 0, 1), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(util::retrieveData(multi_tag, -1, 0), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(util::retrieveData(multi_tag, 10, 0), nix::OutOfBounds);

    NDArray data = util::retrieveData(multi_tag, 0, 0);
    NDSize data_size = data.size();
    CPPUNIT_ASSERT(data.rank() == 3);
    CPPUNIT_ASSERT(data_size[0] == 1 && data_size[1] == 7 && data_size[2] == 3);

    CPPUNIT_ASSERT_THROW(util::retrieveData(multi_tag, 1, 0), nix::OutOfBounds);

    data = util::retrieveData(position_tag, 0);
    data_size = data.size();
    CPPUNIT_ASSERT(data.rank() == 3);
    CPPUNIT_ASSERT(data_size[0] == 1 && data_size[1] == 1 && data_size[2] == 1);

    data = util::retrieveData(segment_tag, 0);
    data_size = data.size();
    CPPUNIT_ASSERT(data.rank() == 3);
    CPPUNIT_ASSERT(data_size[0] == 1 && data_size[1] == 7 && data_size[2] == 3);
}

void TestDataAccess::testTagFeatureData() {
    DataArray number_feat = block.createDataArray("number feature", "test", nix::DataType::Double, {1});
    vector<double> number = {10.0};
    number_feat.setData(number);
    DataArray ramp_feat = block.createDataArray("ramp feature", "test", nix::DataType::Double, {10});
    ramp_feat.label("voltage");
    ramp_feat.unit("mV");
    SampledDimension dim = ramp_feat.appendSampledDimension(1.0);
    dim.unit("ms");
    vector<double> ramp_data = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    ramp_feat.setData(ramp_data);

    Tag pos_tag = block.createTag("feature test", "test", {5.0});
    pos_tag.units({"ms"});

    Feature f1 = pos_tag.createFeature(number_feat, nix::LinkType::Untagged);
    Feature f2 = pos_tag.createFeature(ramp_feat, nix::LinkType::Tagged);
    Feature f3 = pos_tag.createFeature(ramp_feat, nix::LinkType::Untagged);

    NDArray data1 = util::retrieveFeatureData(pos_tag, 0);
    NDArray data2 = util::retrieveFeatureData(pos_tag, 1);
    NDArray data3 = util::retrieveFeatureData(pos_tag, 2);

    CPPUNIT_ASSERT(pos_tag.featureCount() == 3);
    CPPUNIT_ASSERT(data1.num_elements() == 1);
    CPPUNIT_ASSERT(data2.num_elements() == 1);
    CPPUNIT_ASSERT(data3.num_elements() == ramp_data.size());
    // make tag pointing to a slice
    pos_tag.extent({2.0});
    data1 = util::retrieveFeatureData(pos_tag, 0);
    data2 = util::retrieveFeatureData(pos_tag, 1);
    data3 = util::retrieveFeatureData(pos_tag, 2);

    CPPUNIT_ASSERT(data1.num_elements() == 1);
    CPPUNIT_ASSERT(data2.num_elements() == 3);
    CPPUNIT_ASSERT(data3.num_elements() == ramp_data.size());

    pos_tag.deleteFeature(f1.id());
    pos_tag.deleteFeature(f2.id());
    pos_tag.deleteFeature(f3.id());
    block.deleteDataArray(number_feat.id());
    block.deleteDataArray(ramp_feat.id());
    block.deleteTag(pos_tag);
}


void TestDataAccess::testMultiTagFeatureData() {
    DataArray feat_data = block.createDataArray("feature data", "test", nix::DataType::Double, {10, 10});
    SampledDimension dim1 = feat_data.appendSampledDimension(1.0);
    dim1.unit('ms');
    SampledDimension dim2 = feat_data.appendSampledDimension(1.0);
    dim2.unit('ms');

    multi_tag.

}


void TestDataAccess::testMultiTagUnitSupport() {
    vector<string> valid_units{"none","ms","s"};
    vector<string> invalid_units{"mV", "Ohm", "muV"};

    MultiTag testTag = block.createMultiTag("test", "testTag", multi_tag.positions());
    testTag.units(valid_units);
    testTag.addReference(data_array);
    CPPUNIT_ASSERT_NO_THROW(util::retrieveData(testTag, 0, 0));
    testTag.units(none);
    CPPUNIT_ASSERT_NO_THROW(util::retrieveData(testTag, 0, 0));
    testTag.units(invalid_units);
    CPPUNIT_ASSERT_THROW(util::retrieveData(testTag, 0, 0), nix::IncompatibleDimensions);

}
