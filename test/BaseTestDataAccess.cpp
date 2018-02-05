// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>

#include <nix/hydra/multiArray.hpp>
#include <nix/util/dataAccess.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include "BaseTestDataAccess.hpp"

using namespace nix;


void BaseTestDataAccess::testPositionToIndexRangeDimension() {
    std::string unit = "ms";
    std::string invalid_unit = "kV";
    std::string scaled_unit = "s";
    CPPUNIT_ASSERT_THROW(util::positionToIndex(5.0, invalid_unit, rangeDim), nix::IncompatibleDimensions);
    CPPUNIT_ASSERT(util::positionToIndex(1.0, unit, rangeDim) == 0);
    CPPUNIT_ASSERT(util::positionToIndex(8.0, unit, rangeDim) == 4);
    CPPUNIT_ASSERT(util::positionToIndex(0.001, scaled_unit, rangeDim) == 0);
    CPPUNIT_ASSERT(util::positionToIndex(0.008, scaled_unit, rangeDim) == 4);
    CPPUNIT_ASSERT(util::positionToIndex(3.4, unit, rangeDim) == 2);
    CPPUNIT_ASSERT(util::positionToIndex(3.6, unit, rangeDim) == 2);
    CPPUNIT_ASSERT(util::positionToIndex(4.0, unit, rangeDim) == 2);
    CPPUNIT_ASSERT(util::positionToIndex(0.0036, scaled_unit, rangeDim) == 2);
}


void BaseTestDataAccess::testPositionToIndexSampledDimension() {
    std::string unit = "ms";
    std::string invalid_unit = "kV";
    std::string scaled_unit = "s";

    CPPUNIT_ASSERT_THROW(util::positionToIndex(-1.0, unit, sampledDim), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(util::positionToIndex(0.005, invalid_unit, sampledDim), nix::IncompatibleDimensions);
    CPPUNIT_ASSERT(util::positionToIndex(5.0, unit, sampledDim) == 5);
    CPPUNIT_ASSERT(util::positionToIndex(0.005, scaled_unit, sampledDim) == 5);
}


void BaseTestDataAccess::testPositionToIndexSetDimension() {
    std::string unit = "ms";

    CPPUNIT_ASSERT_THROW(util::positionToIndex(5.8, "none", setDim), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(util::positionToIndex(0.5, unit, setDim), nix::IncompatibleDimensions);
    CPPUNIT_ASSERT_NO_THROW(util::positionToIndex(0.5, "none", setDim));
    CPPUNIT_ASSERT(util::positionToIndex(0.5, "none", setDim) == 1);
    CPPUNIT_ASSERT(util::positionToIndex(0.45, "none", setDim) == 0);
}


void BaseTestDataAccess::testOffsetAndCount() {
    NDSize offsets, counts;
    util::getOffsetAndCount(position_tag, data_array, offsets, counts);

    CPPUNIT_ASSERT(offsets.size() == 3);
    CPPUNIT_ASSERT(counts.size() == 3);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 2 && offsets[2] == 2);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 1 && counts[2] == 1);

    position_tag.units(std::vector<std::string>());
    util::getOffsetAndCount(position_tag, data_array, offsets, counts);

    CPPUNIT_ASSERT(position_tag.units().size() == 0);
    CPPUNIT_ASSERT(offsets.size() == 3);
    CPPUNIT_ASSERT(counts.size() == 3);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 2 && offsets[2] == 2);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 1 && counts[2] == 1);

    util::getOffsetAndCount(segment_tag, data_array, offsets, counts);
    CPPUNIT_ASSERT(offsets.size() == 3);
    CPPUNIT_ASSERT(counts.size() == 3);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 2 && offsets[2] == 2);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 7 && counts[2] == 2);

    segment_tag.units(std::vector<std::string>());
    util::getOffsetAndCount(segment_tag, data_array, offsets, counts);
    CPPUNIT_ASSERT(offsets.size() == 3);
    CPPUNIT_ASSERT(counts.size() == 3);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 2 && offsets[2] == 2);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 7 && counts[2] == 2);

    CPPUNIT_ASSERT_THROW(util::getOffsetAndCount(multi_tag, data_array, -1, offsets, counts), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(util::getOffsetAndCount(multi_tag, data_array, 3, offsets, counts), nix::OutOfBounds);

    util::getOffsetAndCount(multi_tag, data_array, 0, offsets, counts);
    CPPUNIT_ASSERT(offsets.size() == 3);
    CPPUNIT_ASSERT(counts.size() == 3);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 3 && offsets[2] == 2);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 7 && counts[2] == 2);

    util::getOffsetAndCount(multi_tag, data_array, 1, offsets, counts);
    CPPUNIT_ASSERT(offsets.size() == 3);
    CPPUNIT_ASSERT(counts.size() == 3);
    CPPUNIT_ASSERT(offsets[0] == 0 && offsets[1] == 8 && offsets[2] == 1);
    CPPUNIT_ASSERT(counts[0] == 1 && counts[1] == 4 && counts[2] == 2);
}


void BaseTestDataAccess::testPositionInData() {
    NDSize offsets, counts;
    util::getOffsetAndCount(multi_tag, data_array, 0, offsets, counts);
    CPPUNIT_ASSERT(util::positionInData(data_array, offsets));
    CPPUNIT_ASSERT(util::positionAndExtentInData(data_array, offsets, counts));

    util::getOffsetAndCount(multi_tag, data_array, 1, offsets, counts);
    CPPUNIT_ASSERT(util::positionInData(data_array, offsets));
    CPPUNIT_ASSERT(!util::positionAndExtentInData(data_array, offsets, counts));
}


void BaseTestDataAccess::testRetrieveData() {
    CPPUNIT_ASSERT_THROW(util::retrieveData(multi_tag, 0, -1), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(util::retrieveData(multi_tag, 0, 1), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(util::retrieveData(multi_tag, -1, 0), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(util::retrieveData(multi_tag, 10, 0), nix::OutOfBounds);

    DataView data_view = util::retrieveData(multi_tag, 0,0);
    NDSize data_size = data_view.dataExtent();

    CPPUNIT_ASSERT(data_size.size() == 3);
    CPPUNIT_ASSERT(data_size[0] == 1 && data_size[1] == 7 && data_size[2] == 2);
    CPPUNIT_ASSERT_THROW(util::retrieveData(multi_tag, 1, 0), nix::OutOfBounds);

    data_view = util::retrieveData(position_tag, 0);
    data_size = data_view.dataExtent();
    CPPUNIT_ASSERT(data_size.size() == 3);
    CPPUNIT_ASSERT(data_size[0] == 1 && data_size[1] == 1 && data_size[2] == 1);

    data_view = util::retrieveData(segment_tag, 0);
    data_size = data_view.dataExtent();
    CPPUNIT_ASSERT(data_size.size() == 3);
    CPPUNIT_ASSERT(data_size[0] == 1 && data_size[1] == 6 && data_size[2] == 2);

    DataView times_view = util::retrieveData(times_tag, 0);
    data_size = times_view.dataExtent();
    std::vector<double> times(data_size.size());
    times_view.getData(times);
    CPPUNIT_ASSERT(data_size.size() == 1);
    CPPUNIT_ASSERT(data_size[0] == 77);
}

void BaseTestDataAccess::testTagFeatureData() {
    DataArray number_feat = block.createDataArray("number feature", "test", nix::DataType::Double, {1});
    std::vector<double> number = {10.0};
    number_feat.setData(number);
    DataArray ramp_feat = block.createDataArray("ramp feature", "test", nix::DataType::Double, {10});
    ramp_feat.label("voltage");
    ramp_feat.unit("mV");
    SampledDimension dim = ramp_feat.appendSampledDimension(1.0);
    dim.unit("ms");
    std::vector<double> ramp_data = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    ramp_feat.setData(ramp_data);

    Tag pos_tag = block.createTag("feature test", "test", {5.0});
    pos_tag.units({"ms"});

    Feature f1 = pos_tag.createFeature(number_feat, nix::LinkType::Untagged);
    Feature f2 = pos_tag.createFeature(ramp_feat, nix::LinkType::Tagged);
    Feature f3 = pos_tag.createFeature(ramp_feat, nix::LinkType::Untagged);

    DataView data1 = util::retrieveFeatureData(pos_tag, 0);
    DataView data2 = util::retrieveFeatureData(pos_tag, 1);
    DataView data3 = util::retrieveFeatureData(pos_tag, 2);

    CPPUNIT_ASSERT(pos_tag.featureCount() == 3);
    CPPUNIT_ASSERT(data1.dataExtent().nelms() == 1);
    CPPUNIT_ASSERT(data2.dataExtent().nelms() == 1);
    CPPUNIT_ASSERT(data3.dataExtent().nelms() == ramp_data.size());

    data1 = util::retrieveFeatureData(pos_tag, f1);
    data2 = util::retrieveFeatureData(pos_tag, f2);
    data3 = util::retrieveFeatureData(pos_tag, f3);

    CPPUNIT_ASSERT(pos_tag.featureCount() == 3);
    CPPUNIT_ASSERT(data1.dataExtent().nelms() == 1);
    CPPUNIT_ASSERT(data2.dataExtent().nelms() == 1);
    CPPUNIT_ASSERT(data3.dataExtent().nelms() == ramp_data.size());

    // make tag pointing to a slice
    pos_tag.extent({2.0});
    data1 = util::retrieveFeatureData(pos_tag, 0);
    data2 = util::retrieveFeatureData(pos_tag, 1);
    std::cerr << data2.dataExtent();
    data3 = util::retrieveFeatureData(pos_tag, 2);

    CPPUNIT_ASSERT(data1.dataExtent().nelms() == 1);
    CPPUNIT_ASSERT(data2.dataExtent().nelms() == 2);
    CPPUNIT_ASSERT(data3.dataExtent().nelms() == ramp_data.size());

    pos_tag.deleteFeature(f1.id());
    pos_tag.deleteFeature(f2.id());
    pos_tag.deleteFeature(f3.id());
    block.deleteDataArray(number_feat.id());
    block.deleteDataArray(ramp_feat.id());
    block.deleteTag(pos_tag);
}


void BaseTestDataAccess::testMultiTagFeatureData() {
    DataArray index_data = block.createDataArray("indexed feature data", "test", nix::DataType::Double, {10, 10});
    SampledDimension dim1 = index_data.appendSampledDimension(1.0);
    dim1.unit("ms");
    SampledDimension dim2 = index_data.appendSampledDimension(1.0);
    dim2.unit("ms");
    typedef boost::multi_array<double, 2> two_d_array;
    typedef two_d_array::index index;
    two_d_array data1(boost::extents[10][10]);
    index value;
    double total = 0.0;
    for(index i = 0; i != 10; ++i) {
        value = 100 * i;
        for(index j = 0; j != 10; ++j) {
            data1[i][j] = static_cast<double>(value++);
            total += data1[i][j];
        }
    }
    index_data.setData(data1);

    DataArray tagged_data = block.createDataArray("tagged feature data", "test", nix::DataType::Double, {10, 20, 10});
    dim1 = tagged_data.appendSampledDimension(1.0);
    dim1.unit("ms");
    dim2 = tagged_data.appendSampledDimension(1.0);
    dim2.unit("ms");
    SampledDimension dim3;
    dim3 = tagged_data.appendSampledDimension(1.0);
    dim3.unit("ms");
    typedef boost::multi_array<double, 3> three_d_array;
    typedef three_d_array::index three_d_index;
    three_d_array data2(boost::extents[10][20][10]);

    for(three_d_index i = 0; i != 2; ++i) {
        value = 100 * i;
        for(three_d_index j = 0; j != 20; ++j) {
            for(three_d_index k = 0; k != 10; ++k) {
                data2[i][j][k] = static_cast<double>(value++);
            }
        }
    }
    tagged_data.setData(data2);

    Feature index_feature = multi_tag.createFeature(index_data, nix::LinkType::Indexed);
    Feature tagged_feature = multi_tag.createFeature(tagged_data, nix::LinkType::Tagged);
    Feature untagged_feature = multi_tag.createFeature(index_data, nix::LinkType::Untagged);

    // preparations done, actually test
    CPPUNIT_ASSERT(multi_tag.featureCount() == 3);
    // indexed feature
    DataView data_view = util::retrieveFeatureData(multi_tag, 0, 0);
    NDSize data_size = data_view.dataExtent();

    CPPUNIT_ASSERT(data_size.size() == 2);
    CPPUNIT_ASSERT(data_size.nelms() == 10);
    double sum = 0.;
    double temp;
    NDSize offset(data_view.dataExtent().size(), 0);

    for (size_t i = 0; i < data_size[1]; ++i){
        offset[1] = i;
        data_view.getData<double>(temp, offset);
        sum += temp;
    }

    CPPUNIT_ASSERT(sum == 45);

    data_view = util::retrieveFeatureData(multi_tag, 9, 0);
    sum = 0;
    for (size_t i = 0; i < data_view.dataExtent()[1]; ++i){
        offset[1] = i;
        data_view.getData<double>(temp, offset);
        sum += temp;
    }
    CPPUNIT_ASSERT(sum == 9045);
    // untagged feature
    data_view = util::retrieveFeatureData(multi_tag, 0, 2);
    CPPUNIT_ASSERT(data_view.dataExtent().nelms() == 100);


    data_view = util::retrieveFeatureData(multi_tag, 1, 2);
    data_size = data_view.dataExtent();
    CPPUNIT_ASSERT(data_size.nelms() == 100);
    sum = 0;

    for (size_t i = 0; i < data_size[0]; ++i) {
        offset[0] = i;
        for (size_t j = 0; j < data_size[1]; ++j) {
            offset[1] = j;
            data_view.getData<double>(temp, offset);
            sum += temp;
        }
    }
    CPPUNIT_ASSERT(sum == total);
    // tagged feature
    data_view = util::retrieveFeatureData(multi_tag, 0, 1);
    data_size = data_view.dataExtent();
    CPPUNIT_ASSERT(data_size.size() == 3);

    data_view = util::retrieveFeatureData(multi_tag, 0, tagged_feature);
    data_size = data_view.dataExtent();
    CPPUNIT_ASSERT(data_size.size() == 3);

    data_view = util::retrieveFeatureData(multi_tag, 1, 1);
    data_size = data_view.dataExtent();
    CPPUNIT_ASSERT(data_size.size() == 3);

    CPPUNIT_ASSERT_THROW(util::retrieveFeatureData(multi_tag, 2, 1), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(util::retrieveFeatureData(multi_tag, 2, 3), nix::OutOfBounds);

    // clean up
    multi_tag.deleteFeature(index_feature.id());
    multi_tag.deleteFeature(tagged_feature.id());
    multi_tag.deleteFeature(untagged_feature.id());
    block.deleteDataArray(tagged_data.id());
    block.deleteDataArray(index_data.id());
}


void BaseTestDataAccess::testMultiTagUnitSupport() {
    std::vector<std::string> valid_units{"none","ms","s"};
    std::vector<std::string> invalid_units{"mV", "Ohm", "muV"};

    MultiTag testTag = block.createMultiTag("test", "testTag", multi_tag.positions());
    testTag.units(valid_units);
    testTag.addReference(data_array);
    CPPUNIT_ASSERT_NO_THROW(util::retrieveData(testTag, 0, 0));
    testTag.units(none);
    CPPUNIT_ASSERT_NO_THROW(util::retrieveData(testTag, 0, 0));
    testTag.units(invalid_units);
    CPPUNIT_ASSERT_THROW(util::retrieveData(testTag, 0, 0), nix::IncompatibleDimensions);
}


void BaseTestDataAccess::testDataView() {
    NDSize zcount = {2, 5, 2};
    NDSize zoffset = {0, 5, 2};

    DataView io = DataView(data_array, zcount, zoffset);

    CPPUNIT_ASSERT_EQUAL(zcount, io.dataExtent());
    CPPUNIT_ASSERT_EQUAL(data_array.dataType(), io.dataType());

    typedef boost::multi_array<double, 3> array_type;
    array_type data(boost::extents[2][5][2]);
    io.getData(data);

    const array_type::size_type *ext = data.shape();
    for (size_t i = 0; i < 3; i++) {
        CPPUNIT_ASSERT_EQUAL(static_cast<array_type::size_type >(zcount[i]), ext[i]);
    }

    array_type ref;
    data_array.getData(ref);

    for(size_t i = 0; i < zcount[0]; ++i) {
        for(size_t j = 0; j < zcount[1]; ++j) {
            for(size_t k = 0; k < zcount[2]; ++k) {
                CPPUNIT_ASSERT_DOUBLES_EQUAL(ref[i + 0][j + 5][k + 2],
                                             data[i][j][k],
                                             std::numeric_limits<double>::epsilon());
                data[i][j][k] = 0.0;
            }
        }
    }

    io.setData(data, {0, 0, 0});
    data_array.getData(ref);

    for(size_t i = 0; i < zcount[0]; ++i) {
        for(size_t j = 0; j < zcount[1]; ++j) {
            for(size_t k = 0; k < zcount[2]; ++k) {
                CPPUNIT_ASSERT_DOUBLES_EQUAL(ref[i + 0][j + 5][k + 2],
                                             0.0,
                                             std::numeric_limits<double>::epsilon());
            }
        }
    }


    double val = 0.0;
    CPPUNIT_ASSERT_THROW(io.getData(val, {}, {0, 0, 3}), OutOfBounds);

    array_type r2d2(boost::extents[3][3][3]);
    CPPUNIT_ASSERT_THROW(io.getData(r2d2, {3, 3, 3}, {}), OutOfBounds);

    CPPUNIT_ASSERT_THROW(io.dataExtent(zcount), std::runtime_error);


}
