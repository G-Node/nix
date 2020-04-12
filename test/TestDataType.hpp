// Copyright © 2020 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Jan Grewe <jan.grewe@g-node.org>


#include <nix.hpp>

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>
#include <vector>

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


class TestDataType : public CPPUNIT_NS::TestFixture {

public:

    void setUp();
    void tearDown();

    void testToString();
    void testFromString();
    void testSize();

private:
    std::vector<nix::DataType> types;
    std::vector<std::string> names;
    std::vector<std::string> namesUp;
    std::vector<std::string> namesLow;
    std::map<nix::DataType, int> fixed_sizes;

    CPPUNIT_TEST_SUITE(TestDataType);
    CPPUNIT_TEST(testToString);
    CPPUNIT_TEST(testFromString);
    CPPUNIT_TEST(testSize);
    CPPUNIT_TEST_SUITE_END ();
};
