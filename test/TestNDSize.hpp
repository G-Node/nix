// Copyright (c) 2016, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#ifndef NIX_TESTNDSIZE
#define NIX_TESTNDSIZE

class TestNDSize: public CPPUNIT_NS::TestFixture {
private:

    CPPUNIT_TEST_SUITE(TestNDSize);
    CPPUNIT_TEST(testAll);
    CPPUNIT_TEST_SUITE_END ();

public:

    void testAll();
};



#endif //NIX_TESTNDSIZE
