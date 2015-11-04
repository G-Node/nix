// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "BaseTestEntityWithMetadata.hpp"
#include <nix/util/util.hpp>

using namespace std;
using namespace nix;


void BaseTestEntityWithMetadata::setUp() {
    file = File::open("test_block.h5", FileMode::Overwrite);

    section = file.createSection("foo_section", "metadata");

    block = file.createBlock("block_one", "dataset");
}


void BaseTestEntityWithMetadata::tearDown() {
    file.close();
}


void BaseTestEntityWithMetadata::testMetadataAccess() {
    CPPUNIT_ASSERT(!block.metadata());

    block.metadata(section);
    CPPUNIT_ASSERT(block.metadata());
    
    // test none-unsetter
    block.metadata(none);
    CPPUNIT_ASSERT(!block.metadata());
    // test deleter removing link too
    block.metadata(section);
    file.deleteSection(section.id());
    CPPUNIT_ASSERT(!block.metadata());
    // re-create section
    section = file.createSection("foo_section", "metadata");
}






