// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestGroup.hpp"


unsigned int & TestGroup::open_mode()
{
    static unsigned int openMode = H5F_ACC_TRUNC;
    return openMode;
}

void TestGroup::setUp() {
    unsigned int &openMode = open_mode();

    h5file = H5::H5File("test_group.h5", openMode);
    if (openMode == H5F_ACC_TRUNC) {
        h5group = h5file.createGroup("charon");
    } else {
        h5group = h5file.openGroup("charon");
    }
    openMode = H5F_ACC_RDWR;
}

void TestGroup::tearDown() {
    h5group.close();
    h5file.close();
}

void TestGroup::groupIface() {
    pandora::Group group(h5group);

    bool hasObj = group.hasObject("nonexisting");
    CPPUNIT_ASSERT_EQUAL(hasObj, false);

    CPPUNIT_ASSERT(group.hasGroup("nonexisting") ==
                   group.hasObject("nonexisting"));

    pandora::Group g2 = group.openGroup("subgroup", true);
    hasObj = group.hasObject("subgroup");
    CPPUNIT_ASSERT(group.hasGroup("subgroup") == hasObj);
    CPPUNIT_ASSERT_EQUAL(hasObj, true);


    size_t objcount = group.objectCount();
    CPPUNIT_ASSERT_EQUAL(objcount, 1UL);
    std::string objname = group.objectName(0);
    CPPUNIT_ASSERT(objname == "subgroup");

    group.removeGroup("subgroup");
    hasObj = group.hasObject("subgroup");
    CPPUNIT_ASSERT(group.hasGroup("subgroup") == hasObj);
    CPPUNIT_ASSERT_EQUAL(hasObj, false);
}
