// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
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


class TestSection: public CPPUNIT_NS::TestFixture {
private:

    CPPUNIT_TEST_SUITE(TestSection);

    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testType);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testDefinition);

    CPPUNIT_TEST(testParent);
    CPPUNIT_TEST(testRepository);
    CPPUNIT_TEST(testLink);
    CPPUNIT_TEST(testMapping);
    CPPUNIT_TEST(testSectionAccess);
    CPPUNIT_TEST(testFindSection);
    CPPUNIT_TEST(testFindRelated);
    CPPUNIT_TEST(testPropertyAccess);

    CPPUNIT_TEST(testOperators);
    CPPUNIT_TEST(testUpdatedAt);
    CPPUNIT_TEST(testCreatedAt);

    CPPUNIT_TEST_SUITE_END ();

    nix::File file, file_fs;
    nix::Section section, section_other, section_null, section_fs, section_fs_other;
    time_t startup_time;

    void test_type(nix::Section &s);
    void test_definition(nix::Section &s);
    void test_link(nix::File &f, nix::Section &s, nix::Section &other);
    void test_repository(nix::Section &s);
    void test_mapping(nix::Section &s);
    void test_section_access(nix::Section &s, nix::Implementation impl);
    void test_operators(nix::Section &s, nix::Section &other, nix::Section &null);

public:

    void setUp();
    void tearDown();

    void testValidate();
    void testId();
    void testType();
    void testName();
    void testDefinition();

    void testParent();
    void testRepository();
    void testLink();
    void testMapping();
    void testSectionAccess();
    void testFindSection();
    void testFindRelated();
    void testPropertyAccess();

    void testOperators();
    void testUpdatedAt();
    void testCreatedAt();

};

