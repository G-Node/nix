// Copyright © 2016 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>


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
#include <boost/iterator/zip_iterator.hpp>


class TestVersion : public CPPUNIT_NS::TestFixture {

public:

    void setUp();
    void tearDown();

    void testFormatVersion();

private:

    CPPUNIT_TEST_SUITE(TestVersion);
    CPPUNIT_TEST(testFormatVersion);
    CPPUNIT_TEST_SUITE_END ();

};
