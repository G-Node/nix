// Copyright (c) 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TESTENTITYWITHSOURCESFS_HPP
#define NIX_TESTENTITYWITHSOURCESFS_HPP

#include "BaseTestEntityWithSources.hpp"

class TestEntityWithSourcesFS : public BaseTestEntityWithSources {

    CPPUNIT_TEST_SUITE(TestEntityWithSourcesFS);
    CPPUNIT_TEST(testSourceAccess);
    CPPUNIT_TEST(testSourceVectorSetter);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        file = nix::File::open("test_entity_sources", nix::FileMode::Overwrite, "file");
        block = file.createBlock("block_one", "dataset");
    }


    void tearDown() {
        file.deleteBlock(block);
        file.close();
    }

};

#endif //NIX_TESTENTITYWITHSOURCESFS_HPP
