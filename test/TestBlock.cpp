// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include "TestBlock.hpp"

using namespace std;
using namespace nix;


void TestBlock::setUp() {
    startup_time = time(NULL);
    file = File::open("test_block.h5", FileMode::Overwrite);

    block = file.createBlock("block_one", "dataset");
    block_other = file.createBlock("block_two", "dataset");
    block_null  = nullptr;
}


void TestBlock::tearDown() {
    file.removeBlock(block.id());
    file.removeBlock(block_other.id());

    file.close();
}


void TestBlock::testCreatedAt() {
    CPPUNIT_ASSERT(block.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    block.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(block.createdAt() == past_time);
}


void TestBlock::testUpdatedAt() {
    CPPUNIT_ASSERT(block.updatedAt() >= startup_time);
}
