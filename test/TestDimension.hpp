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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include <nix.hpp>

class TestDimension: public CPPUNIT_NS::TestFixture {
private:

    CPPUNIT_TEST_SUITE(TestDimension);

    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testSampledDimLabel);
    CPPUNIT_TEST(testSampledDimOffset);
    CPPUNIT_TEST(testSampledDimUnit);
    CPPUNIT_TEST(testSampledDimSamplingInterval);
    CPPUNIT_TEST(testSampledDimOperators);
    /*
    CPPUNIT_TEST(testSetDimLabels);
    CPPUNIT_TEST(testSetDimOperators);

    CPPUNIT_TEST(testRangeDimLabel);
    CPPUNIT_TEST(testRangeDimUnit);
    CPPUNIT_TEST(testRangeDimOperators);
	*/
    CPPUNIT_TEST_SUITE_END ();

    nix::File file;
    nix::DataArray data_array;

public:

    void setUp();
    void tearDown();

    void testId();

    void testSampledDimLabel();
    void testSampledDimOffset();
    void testSampledDimUnit();
    void testSampledDimSamplingInterval();
    void testSampledDimOperators();

    void testSetDimLabels();
    void testSetDimOperators();

    void testRangeDimLabel();
    void testRangeDimUnit();
    void testRangeDimOperators();


};

