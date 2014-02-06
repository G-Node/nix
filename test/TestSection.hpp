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

class TestSection: public CPPUNIT_NS::TestFixture {
private:

    CPPUNIT_TEST_SUITE(TestSection);

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
    CPPUNIT_TEST(testPropertyAccess);

    CPPUNIT_TEST(testOperators);
    CPPUNIT_TEST(testUpdatedAt);
    CPPUNIT_TEST(testCreatedAt);

    CPPUNIT_TEST_SUITE_END ();

    nix::File file;
    nix::Section section, section_other, section_null;
    time_t startup_time;

public:

    void setUp();
    void tearDown();

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
    void testPropertyAccess();

    void testOperators();
    void testUpdatedAt();
    void testCreatedAt();

    /*
    void testAddAndRemove();
    void testDepthRetrieving();
    void testRelatedSections();
    void testAddingProperties();
    void testAccessingProperties();
    void testRemovingProperties();
    void testSectionLinks();
    */

};

