// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include <nix/util/util.hpp>
#include "TestEntity.hpp"

using namespace std;
using namespace nix;


void TestEntity::setUp() {
    startup_time = time(NULL);
    file = File::open("test_block.h5", FileMode::Overwrite);
    block = file.createBlock("block_one", "dataset");
}


void TestEntity::tearDown() {
    file.close();
}


void TestEntity::testId() {
    CPPUNIT_ASSERT(block.id().size() == 36);
}


void TestEntity::testName() {
    CPPUNIT_ASSERT(block.name() == "block_one");
}


void TestEntity::testType() {
    CPPUNIT_ASSERT(block.type() == "dataset");
    string typ = util::createId();
    block.type(typ);
    CPPUNIT_ASSERT(block.type() == typ);
}


void TestEntity::testDefinition() {
    string def = util::createId();
    block.definition(def);
    CPPUNIT_ASSERT(*block.definition() == def);
    block.definition(nix::none);
    CPPUNIT_ASSERT(block.definition() == nix::none);
}


void TestEntity::testCreatedAt() {
    CPPUNIT_ASSERT(block.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    block.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(block.createdAt() == past_time);
}


void TestEntity::testUpdatedAt() {
    CPPUNIT_ASSERT(block.updatedAt() >= startup_time);
}




