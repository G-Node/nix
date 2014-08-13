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

class TestSource:public CPPUNIT_NS::TestFixture {
private:

    CPPUNIT_TEST_SUITE(TestSource);

    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testType);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testDefinition);

    CPPUNIT_TEST(testMetadataAccess);
    CPPUNIT_TEST(testSourceAccess);
    CPPUNIT_TEST(testFindSource);

    CPPUNIT_TEST(testOperators);
    CPPUNIT_TEST(testUpdatedAt);
    CPPUNIT_TEST(testCreatedAt);

    CPPUNIT_TEST_SUITE_END();

    nix::File file;
    nix::Block block;
    nix::Section section;
    nix::Source source, source_other, source_null;
    nix::DataArray darray;
    nix::MultiTag dtag;
    time_t startup_time;

public:

    void setUp();
    void tearDown();

    void testValidate();
    void testId();
    void testType();
    void testName();
    void testDefinition();

    void testMetadataAccess();
    void testSourceAccess();
    void testFindSource();

    void testOperators();
    void testUpdatedAt();
    void testCreatedAt();
};

