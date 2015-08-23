// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

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


class TestBlock : public CPPUNIT_NS::TestFixture {

private:

    CPPUNIT_TEST_SUITE(TestBlock);

    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testType);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testDefinition);

    CPPUNIT_TEST(testMetadataAccess);
    CPPUNIT_TEST(testSourceAccess);
    CPPUNIT_TEST(testDataArrayAccess);
    CPPUNIT_TEST(testTagAccess);
    CPPUNIT_TEST(testMultiTagAccess);

    CPPUNIT_TEST(testOperators);
    CPPUNIT_TEST(testUpdatedAt);
    CPPUNIT_TEST(testCreatedAt);
    
    CPPUNIT_TEST(testCompare);

    CPPUNIT_TEST_SUITE_END ();

    nix::File file, file_fs;
    nix::Section section, section_fs;
    nix::Block block, block_other, block_null, block_fs;
    time_t startup_time;

    void test_metadata_access(nix::File &f, nix::Block &b, nix::Section &s);
    void test_source_access(nix::File &f, nix::Block &b);
    void test_data_array_access(nix::Block &b);
    void test_tag_access(nix::Block &b);
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
    void testDataArrayAccess();
    void testTagAccess();
    void testMultiTagAccess();

    void testOperators();
    void testUpdatedAt();
    void testCreatedAt();

    void testCompare();
};
