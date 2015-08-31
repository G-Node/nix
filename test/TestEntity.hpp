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
#include <limits>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>


class TestEntity : public CPPUNIT_NS::TestFixture {

private:

    CPPUNIT_TEST_SUITE(TestEntity);

    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testType);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testDefinition);

    CPPUNIT_TEST(testUpdatedAt);
    CPPUNIT_TEST(testCreatedAt);
    CPPUNIT_TEST(testOperators);

    CPPUNIT_TEST_SUITE_END ();

    nix::File file, file_fs;
    nix::Block block, block_other, block_null, block_fs, block_other_fs;
    time_t startup_time;

    void test_type(nix::Block &b);
    void test_definition(nix::Block &b);
    void test_created_at(nix::Block &b);
    void test_operators(nix::Block &b, nix::Block &other, nix::Block &null);

public:

    void setUp();
    void tearDown();

    void testId();
    void testType();
    void testName();
    void testDefinition();
    void testOperators();
    void testUpdatedAt();
    void testCreatedAt();

};
