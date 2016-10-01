// Copyright (c) 2016, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestNDSize.hpp"

#include <nix/NDSize.hpp>

void TestNDSize::testAll() {
    using namespace nix;

    NDSize invalidSize = {};
    NDSize a({23, 42, 1982});

    CPPUNIT_ASSERT(!invalidSize); // testing operator bool()
    CPPUNIT_ASSERT(a ? true : false);

    typedef NDSize::value_type value_type;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(23),   a[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   a[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), a[2]);

    CPPUNIT_ASSERT_THROW(a[3], std::out_of_range);

    a++;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(24),   a[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(43),   a[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1983), a[2]);

    a--;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(23),   a[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   a[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), a[2]);

    a += 13;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(36),   a[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(55),   a[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1995), a[2]);

    a -= 13;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(23),   a[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   a[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), a[2]);


    NDSize b({19, 1940, 18});

    NDSize c = a + b;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   c[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), c[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(2000), c[2]);

    NDSize d = c - b;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(23),   d[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   d[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), d[2]);

    NDSize f({1, 2, 3, 4});
    CPPUNIT_ASSERT_THROW(a + f, std::out_of_range);

    NDSize g(f.size(), 0);

    g += f;

    CPPUNIT_ASSERT(g == f);
    CPPUNIT_ASSERT(g != a);

    NDSize h = b / b;
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1), h[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1), h[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1), h[2]);

    NDSize j(h.size(), static_cast<value_type>(333));
    NDSize k = h * j;

    CPPUNIT_ASSERT(j == k);

    NDSize::value_type dp = j.dot(h);
    CPPUNIT_ASSERT_EQUAL(static_cast<NDSize::value_type>(999), dp);

    NDSize s({3, 4});
    dp = s.dot(s);
    CPPUNIT_ASSERT_EQUAL(static_cast<NDSize::value_type>(25), dp);

    //comparison tests
    CPPUNIT_ASSERT_THROW(f < s, IncompatibleDimensions);

    NDSize t({4, 5});
    NDSize u({4, 4});

    // actual non-delegation implementations are < and <=
    CPPUNIT_ASSERT(s < t);
    CPPUNIT_ASSERT(u <= t);

    // everything else, i.e. >, >= is delegated
    CPPUNIT_ASSERT(t > s);
    CPPUNIT_ASSERT(t >= u);

    CPPUNIT_ASSERT(!(t <= s));
    CPPUNIT_ASSERT(!(t < u));
}
