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


class TestFile: public CPPUNIT_NS::TestFixture {
private:

    CPPUNIT_TEST_SUITE(TestFile);
    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testFormat);
    CPPUNIT_TEST(testLocation);
    CPPUNIT_TEST(testVersion);
    CPPUNIT_TEST(testCreatedAt);
    CPPUNIT_TEST(testUpdatedAt);
    CPPUNIT_TEST(testBlockAccess);
    CPPUNIT_TEST(testSectionAccess);
    CPPUNIT_TEST(testOperators);
    CPPUNIT_TEST(testReopen);
    CPPUNIT_TEST_SUITE_END ();

    nix::File file_open, file_other, file_null;
    time_t statup_time;

public:

    void setUp();
    void tearDown();
    void testValidate();
    void testFormat();
    void testLocation();
    void testVersion();
    void testCreatedAt();
    void testUpdatedAt();
    void testBlockAccess();
    void testSectionAccess();
    void testOperators();
    void testReopen();
};
