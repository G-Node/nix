// Copyright © 2013,2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <nix.hpp>
#include "nix/hdf5/H5Group.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include <boost/math/constants/constants.hpp>
#include <boost/iterator/zip_iterator.hpp>



class TestH5 : public CPPUNIT_NS::TestFixture {
public:

    void setUp();
    void tearDown();
    static unsigned int &open_mode();

    void testBase();
    void testDataType();
    void testDataSpace();

private:
    hid_t h5file;
    nix::hdf5::H5Group h5group;

    CPPUNIT_TEST_SUITE(TestH5);
    CPPUNIT_TEST(testBase);
    CPPUNIT_TEST(testDataType);
    CPPUNIT_TEST(testDataSpace);
    CPPUNIT_TEST_SUITE_END ();

};