// Copyright (c) 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TESTBLOCKHDF5_HPP
#define NIX_TESTBLOCKHDF5_HPP

#include "BaseTestBlock.hpp"

class TestBlockHDF5 : public BaseTestBlock {

    CPPUNIT_TEST_SUITE(TestBlockHDF5);

    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testType);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testDefinition);

    CPPUNIT_TEST(testMetadataAccess);
    CPPUNIT_TEST(testSourceAccess);
    CPPUNIT_TEST(testDataArrayAccess);
    CPPUNIT_TEST(testTagAccess);
    CPPUNIT_TEST(testMultiTagAccess);

    CPPUNIT_TEST(testOperators);
    CPPUNIT_TEST(testUpdatedAt);
    CPPUNIT_TEST(testCreatedAt);

    CPPUNIT_TEST(testCompare);

    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        startup_time = time(NULL);
        file = nix::File::open("test_block.h5", nix::FileMode::Overwrite);

        section = file.createSection("foo_section", "metadata");

        block = file.createBlock("block_one", "dataset");
        block_other = file.createBlock("block_two", "dataset");
        block_null  = nix::none;
    }


    void tearDown() {
        file.close();
    }
    
};

#endif //NIX_TESTBLOCKHDF5_HPP
