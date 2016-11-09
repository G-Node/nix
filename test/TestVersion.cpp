// Copyright © 2016 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include "TestVersion.hpp"

void TestVersion::setUp() {
}

void TestVersion::tearDown() {
}

struct FVTest {
    nix::FormatVersion a;
    nix::FormatVersion b;

    bool equal;    // a == b
    bool smaller;  // a <  b

    bool AcanWriteB;  //a.canWrite(b)
    bool AcanReadB;   // a.canRead(b)
    bool BcanReadA;   // b.canRead(A)
};

FVTest fvt_table[] = {
    {{1, 2, 3}, {1, 2, 3}, true,  false, true,  true,  true },
    {{1, 2, 0}, {1, 2, 3}, false, true,  false, true,  true },
    {{1, 2, 4}, {1, 2, 3}, false, false, false, true,  true},
    {{1, 1, 3}, {1, 2, 3}, false, true,  false, false, true },
    {{1, 0, 0}, {1, 2, 3}, false, true,  false, false, true },
    {{0, 0, 0}, {1, 2, 3}, false, true,  false, false, false},
    {{0, 1, 1}, {1, 2, 3}, false, true,  false, false, false},
    {{0, 1, 0}, {1, 2, 3}, false, true,  false, false, false},
    {{0, 0, 1}, {1, 2, 3}, false, true,  false, false, false},
};

void TestVersion::testFormatVersion() {

    std::cerr << std::endl;
    for (int i = 0; i < static_cast<int>(sizeof(fvt_table)/sizeof(FVTest)); i++) {
        const FVTest &t = fvt_table[i];
        std::cerr << i << " versions => a: " << t.a << ", b: " << t.b << std::endl;

        if (i < 3) {
            CPPUNIT_ASSERT_EQUAL(i+1, t.b[i]);
        } else {
            CPPUNIT_ASSERT_THROW(t.b[i], std::out_of_range);
        }

        CPPUNIT_ASSERT_EQUAL(t.equal,        t.a == t.b);
        CPPUNIT_ASSERT_EQUAL(t.smaller,      t.a <  t.b);
        CPPUNIT_ASSERT_EQUAL(t.a < t.b,      !(t.a >= t.b));
        CPPUNIT_ASSERT_EQUAL(t.AcanWriteB, t.a.canWrite(t.b));
        CPPUNIT_ASSERT_EQUAL(t.AcanReadB,  t.a.canRead(t.b));
        CPPUNIT_ASSERT_EQUAL(t.BcanReadA,  t.b.canRead(t.a));
    }

}

void TestVersion::testAPIVersion() {
    std::vector<int> v = nix::apiVersion();
    CPPUNIT_ASSERT(v.size() == 3);
}
