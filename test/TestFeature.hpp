// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix.hpp>
#include <nix/valid/validate.hpp>

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


class TestFeature: public CPPUNIT_NS::TestFixture {
private:

    CPPUNIT_TEST_SUITE(TestFeature);

    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testLinkType);
    CPPUNIT_TEST(testData);
    CPPUNIT_TEST(testOperator);

    CPPUNIT_TEST_SUITE_END ();

    nix::File file;
    nix::DataArray data_array;
    nix::Block block;
    nix::Tag tag;

public:

    void setUp();
    void tearDown();

    void testValidate();
    void testId();

    void testLinkType();
    void testData();
    void testOperator();

};

