// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix.hpp>

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <boost/math/constants/constants.hpp>

class TestDimension: public CPPUNIT_NS::TestFixture {
private:

    CPPUNIT_TEST_SUITE(TestDimension);

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
    CPPUNIT_TEST(testRangeDimTickAt);
    CPPUNIT_TEST(testRangeDimAxis);
    CPPUNIT_TEST(testAsDimensionMethods);

    CPPUNIT_TEST_SUITE_END ();

    nix::File file, file_fs;
    nix::DataArray data_array, data_array_fs;
    nix::Block block, block_fs;


    void test_validate(nix::DataArray &array);
    void test_set_validate(nix::DataArray &array);
    void test_sample_validate(nix::DataArray &array);
    void test_range_validate(nix::DataArray &array);
    void test_index(nix::DataArray & array);
    void test_sample_dim_label(nix::DataArray &a);
    void test_sample_dim_offset(nix::DataArray &a);
    void test_sample_dim_unit(nix::DataArray &a);
    void test_sample_dim_sampling_interval(nix::DataArray &a);
    void test_sample_dim_operators(nix::DataArray &a);
    void test_sample_dim_index_of(nix::DataArray &a);
    void test_sample_dim_position_at(nix::DataArray &a);
    void test_sample_dim_axis(nix::DataArray &a);
    void test_as_dimension(nix::DataArray &da);

public:

    void setUp();
    void tearDown();

    void testValidate();
    void testSetValidate();
    void testSampleValidate();
    void testRangeValidate();
    
    void testIndex();

    void testSampledDimLabel();
    void testSampledDimOffset();
    void testSampledDimUnit();
    void testSampledDimSamplingInterval();
    void testSampledDimOperators();
    void testSampledDimIndexOf();
    void testSampledDimPositionAt();
    void testSampledDimAxis();

    void testSetDimLabels();

    void testRangeDimLabel();
    void testRangeTicks();
    void testRangeDimUnit();
    void testRangeDimIndexOf();
    void testRangeDimTickAt();
    void testRangeDimAxis();

    void testAsDimensionMethods();
};

