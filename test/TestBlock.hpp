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

#include <pandora.hpp>

using namespace pandora;

class TestBlock : public CPPUNIT_NS::TestFixture {

private:

    CPPUNIT_TEST_SUITE(TestBlock);
    CPPUNIT_TEST(testAddRemoveDataArray);
    CPPUNIT_TEST_SUITE_END ();

    File *f1;
public:

    void setUp() {
        f1 = new File("test_block.h5", FileMode::ReadWrite);
    }

    void tearDown() {
        delete f1;
    }


    void testAddRemoveDataArray() {
        // TODO implement testAddRemoveDataArray()
    }

};
