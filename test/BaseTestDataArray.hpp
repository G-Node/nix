// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <nix/hydra/multiArray.hpp>
#include <nix.hpp>

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include <boost/math/constants/constants.hpp>
#include <boost/math/tools/rational.hpp>
#include <boost/iterator/zip_iterator.hpp>

class BaseTestDataArray : public CPPUNIT_NS::TestFixture {

public:

    void setUp();
    void tearDown();

    void testId();
    void testType();
    void testName();
    void testDefinition();
    void testData();
    void testPolynomial();
    void testLabel();
    void testUnit();
    void testDimension();
    void testAliasRangeDimension();
    void testOperator();
    void testValidate();

private:

    CPPUNIT_TEST_SUITE(BaseTestDataArray);

    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testType);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testDefinition);
    CPPUNIT_TEST(testData);
    CPPUNIT_TEST(testPolynomial);
    CPPUNIT_TEST(testLabel);
    CPPUNIT_TEST(testUnit);
    CPPUNIT_TEST(testDimension);
    CPPUNIT_TEST(testAliasRangeDimension);
    CPPUNIT_TEST(testOperator);
    CPPUNIT_TEST(testValidate);

    CPPUNIT_TEST_SUITE_END ();

    nix::File file;
    nix::Block block;
    nix::DataArray array1, array2, array3;
    time_t startup_time;
};
