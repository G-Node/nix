// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

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
#include <boost/iterator/zip_iterator.hpp>

#include <nix.hpp>

class TestProperty : public CPPUNIT_NS::TestFixture {

private:

    CPPUNIT_TEST_SUITE(TestProperty);

    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testDefinition);
    CPPUNIT_TEST(testMapping);

    CPPUNIT_TEST(testValues);
    CPPUNIT_TEST(testDataType);
    CPPUNIT_TEST(testUnit);

    CPPUNIT_TEST(testOperators);
    CPPUNIT_TEST(testUpdatedAt);
    CPPUNIT_TEST(testCreatedAt);

    CPPUNIT_TEST_SUITE_END();

    time_t startup_time;
    nix::File file;
    nix::Section section;
    nix::Property property, property_other, property_null;
    nix::Value int_dummy, str_dummy, double_dummy;
public:

    void setUp();
    void tearDown();

    void testValidate();
    void testId();
    void testType();
    void testName();
    void testDefinition();
    void testMapping();
    void testDataType();
    void testValues();
    void testUnit();

    void testOperators();
    void testUpdatedAt();
    void testCreatedAt();
};
