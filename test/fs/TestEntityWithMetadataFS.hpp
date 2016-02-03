// Copyright (c) 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
#ifndef NIX_TESTENTITYWITHMETADATAFS_HPP
#define NIX_TESTENTITYWITHMETADATAFS_HPP

#include "BaseTestEntityWithMetadata.hpp"

class TestEntityWithMetadataFS : public BaseTestEntityWithMetadata {

    CPPUNIT_TEST_SUITE(TestEntityWithMetadataFS);
    CPPUNIT_TEST(testMetadataAccess);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        file = nix::File::open("test_block", nix::FileMode::Overwrite, "file");
        section = file.createSection("foo_section", "metadata");
        wrong = file.createSection("bar_section", "metadata");
        block = file.createBlock("block_one", "dataset");
    }

    void tearDown() {
        file.close();
    }
};

#endif //NIX_TESTENTITYWITHMETADATAFS_HPP
