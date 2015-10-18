// Copyright © 2013,2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <nix/hydra/multiArray.hpp>
#include <nix.hpp>

#include <nix/hdf5/Group.hpp>

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


class TestGroup:public CPPUNIT_NS::TestFixture {

public:

    void setUp();
    void tearDown();

    void testRefCount();
    void testBaseTypes();
    void testMultiArray();
    void testVector();

    void testArray();

    void testOpen();

    template<typename T>
    static void assert_vectors_equal(std::vector<T> &a, std::vector<T> &b) {

        //typedef const T &value_ref;
        auto ibeg = boost::make_tuple(a.begin(), b.begin());
        auto iend = boost::make_tuple(a.end(), b.end());
        auto zibeg = boost::make_zip_iterator(ibeg);
        auto ziend = boost::make_zip_iterator(iend);
        typedef const boost::tuple<const T&, const T&>& tuple_ref;

        std::for_each(zibeg, ziend, [](tuple_ref t) {
            CPPUNIT_ASSERT_EQUAL(boost::get<0>(t), boost::get<1>(t));
        });
    }

private:

    static unsigned int &open_mode();

    hid_t h5file;
    hid_t h5group;

    CPPUNIT_TEST_SUITE(TestGroup);
    CPPUNIT_TEST(testRefCount);
    CPPUNIT_TEST(testOpen);
    CPPUNIT_TEST(testBaseTypes);
    CPPUNIT_TEST(testVector);
    CPPUNIT_TEST(testMultiArray);
    CPPUNIT_TEST(testArray);
    CPPUNIT_TEST_SUITE_END ();
};
