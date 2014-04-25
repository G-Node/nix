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
    data_array = block.createDataArray("dimensionTest", "test");
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
}


void TestDataAccess::tearDown() {
    file.deleteBlock(block.id());
    file.close();
}


void TestDataAccess::testPositionToIndexRangeDimension() {
    string unit = "ms";
    string invalid_unit = "kV";
    string scaled_unit = "s";

    CPPUNIT_ASSERT_THROW(util::positionToIndex(5.0, "", sampledDim), nix::IncompatibleDimensions);
    CPPUNIT_ASSERT_THROW(util::positionToIndex(5.0, invalid_unit, sampledDim), nix::IncompatibleDimensions);
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
    CPPUNIT_ASSERT_THROW(util::positionToIndex(0.005, "", sampledDim), nix::IncompatibleDimensions);
    CPPUNIT_ASSERT(util::positionToIndex(5.0, unit, sampledDim) == 5);
    CPPUNIT_ASSERT(util::positionToIndex(0.005, scaled_unit, sampledDim) == 5);
}


void TestDataAccess::testPositionToIndexSetDimension() {
    string unit = "ms";

    CPPUNIT_ASSERT_THROW(util::positionToIndex(5.8, "", setDim), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(util::positionToIndex(0.5, unit, setDim), nix::IncompatibleDimensions);
    CPPUNIT_ASSERT_NO_THROW(util::positionToIndex(0.5, "", setDim));
    CPPUNIT_ASSERT(util::positionToIndex(0.5, "", setDim) == 1);
    CPPUNIT_ASSERT(util::positionToIndex(0.45, "", setDim) == 0);
}
