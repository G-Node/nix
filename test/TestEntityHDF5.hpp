// Copyright (c) 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TESTENTITYHDF5_HPP
#define NIX_TESTENTITYHDF5_HPP

#include "BaseTestEntity.hpp"

class TestEntityHDF5 : public BaseTestEntity {

    CPPUNIT_TEST_SUITE(TestEntityHDF5);

    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testType);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testDefinition);

    CPPUNIT_TEST(testUpdatedAt);
    CPPUNIT_TEST(testCreatedAt);

    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        startup_time = time(NULL);
        file = nix::File::open("test_block.h5", nix::FileMode::Overwrite);
        block = file.createBlock("block_one", "dataset");
    }

    void tearDown() {
        file.close();
    }
};

#endif //NIX_TESTENTITYHDF5_HPP
