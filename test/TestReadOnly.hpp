// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
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


class TestReadOnly : public CPPUNIT_NS::TestFixture {

private:

    CPPUNIT_TEST_SUITE(TestReadOnly);

    CPPUNIT_TEST(testRead);

    CPPUNIT_TEST_SUITE_END ();

    std::string section_id, block_id, tag_id, mtag_id, property_id, 
                feature_id, data_array_id;
    nix::ndsize_t dim_index, dim_sampled_index, dim_range_index, dim_set_index;
    std::stringstream s;

    time_t startup_time;

public:

    void setUp();
    void tearDown();

    void testRead();

};
