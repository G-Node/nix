// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#include <nix.hpp>
#include <nix/hdf5/BaseTagHDF5.hpp>
#include <nix/util/util.hpp>

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


class TestBaseTag: public CPPUNIT_NS::TestFixture {

private:

    nix::File file;
    nix::Block block;
    std::vector<nix::DataArray> refs;

    CPPUNIT_TEST_SUITE(TestBaseTag);

    CPPUNIT_TEST(testReferences);
    CPPUNIT_TEST(testFeatures);
    CPPUNIT_TEST_SUITE_END ();

public:

    void setUp();
    void tearDown();

    void testReferences();
    void testFeatures();
};
