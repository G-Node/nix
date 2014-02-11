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
#include <limits>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include <nix.hpp>


class TestEntityWithSources : public CPPUNIT_NS::TestFixture {

private:

    CPPUNIT_TEST_SUITE(TestEntityWithSources);
    CPPUNIT_TEST(testSourceAccess);
    CPPUNIT_TEST_SUITE_END ();

    nix::File file;
    nix::Block block;

public:

    void setUp();
    void tearDown();

    void testSourceAccess();
};
