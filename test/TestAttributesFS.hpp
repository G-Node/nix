// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix.hpp>
#include "nix/file/AttributesFS.hpp"

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <boost/filesystem.hpp>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>


class TestAttributesFS : public CPPUNIT_NS::TestFixture {
private:
    boost::filesystem::path location;

    CPPUNIT_TEST_SUITE(TestAttributesFS);
    CPPUNIT_TEST(testOpenCreate);
    CPPUNIT_TEST(testHasField);
    CPPUNIT_TEST(testWriteField);
    CPPUNIT_TEST(testReadField);
    CPPUNIT_TEST_SUITE_END ();

    nix::File file;
    nix::DataArray data_array;
    nix::Block block;
    nix::Tag tag;

public:

    void setUp();

    void tearDown();

    void testOpenCreate();

    void testHasField();

    void testWriteField();

    void testReadField();

};
