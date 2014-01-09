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

#include "pandora/File.hpp"
#include "pandora/Block.hpp"
#include "pandora/Section.hpp"
#include "pandora/Source.hpp"

using namespace std;
using namespace pandora;

class TestSource:public CPPUNIT_NS::TestFixture {
private:

    CPPUNIT_TEST_SUITE(TestSource);

    CPPUNIT_TEST(testCreateAndRemove);
    CPPUNIT_TEST(testIterators);
    CPPUNIT_TEST(testFindSources);

    CPPUNIT_TEST_SUITE_END ();

    File *f1;

public:

    void setUp();
    void tearDown();
    void testCreateAndRemove();
    void testIterators();
    void testFindSources();

};

