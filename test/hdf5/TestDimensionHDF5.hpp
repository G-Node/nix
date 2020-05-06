// Copyright (c) 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in Section and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_TESTDIMENSIONHDF5_HPP
#define NIX_TESTDIMENSIONHDF5_HPP

#include "BaseTestDimension.hpp"

class TestDimensionHDF5 : public BaseTestDimension {

    CPPUNIT_TEST_SUITE(TestDimensionHDF5);
    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testSetValidate);
    CPPUNIT_TEST(testSampleValidate);
    CPPUNIT_TEST(testRangeValidate);
    CPPUNIT_TEST(testIndex);
    CPPUNIT_TEST(testSampledDimLabel);
    CPPUNIT_TEST(testSampledDimOffset);
    CPPUNIT_TEST(testSampledDimUnit);
    CPPUNIT_TEST(testSampledDimSamplingInterval);
    CPPUNIT_TEST(testSampledDimOperators);
    CPPUNIT_TEST(testSampledDimIndexOf);
    CPPUNIT_TEST(testSampledDimPositionAt);
    CPPUNIT_TEST(testSampledDimAxis);
    CPPUNIT_TEST(testSetDimLabels);
    CPPUNIT_TEST(testRangeDimLabel);
    CPPUNIT_TEST(testRangeDimUnit);
    CPPUNIT_TEST(testRangeTicks);
    CPPUNIT_TEST(testRangeDimIndexOf);
    CPPUNIT_TEST(testRangeDimPositionInRange);
    CPPUNIT_TEST(testRangeDimTickAt);
    CPPUNIT_TEST(testRangeDimAxis);
    CPPUNIT_TEST(testAsDimensionMethods);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        file = nix::File::open("test_dimension.h5", nix::FileMode::Overwrite);
        block = file.createBlock("dimensionTest","test");
        data_array = block.createDataArray("dimensionTest", "Test",
                                           nix::DataType::Double, nix::NDSize({ 0 }));
    }


    void tearDown() {
        file.deleteBlock(block.id());
        file.close();
    }
};

#endif //NIX_TESTDIMENSIONHDF5_HPP
