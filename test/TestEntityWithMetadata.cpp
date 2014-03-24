// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include "TestEntityWithMetadata.hpp"
#include "TestSource.hpp"

using namespace std;
using namespace nix;


void TestEntityWithMetadata::setUp() {
    file = File::open("test_block.h5", FileMode::Overwrite);

    section = file.createSection("foo_section", "metadata");

    block = file.createBlock("block_one", "dataset");
}


void TestEntityWithMetadata::tearDown() {
    file.close();
}


void TestEntityWithMetadata::testMetadataAccess() {
    CPPUNIT_ASSERT(!block.metadata());

    block.metadata(section);
    CPPUNIT_ASSERT(block.metadata());

    block.metadata(boost::none);
    CPPUNIT_ASSERT(!block.metadata());
}






