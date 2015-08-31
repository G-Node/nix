// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestEntity.hpp"

#include <nix/util/util.hpp>
#include <ctime>

using namespace std;
using namespace nix;


void TestEntity::setUp() {
    startup_time = time(NULL);
    file = File::open("test_entity.h5", FileMode::Overwrite);
    block = file.createBlock("block_one", "dataset");
    block_other = file.createBlock("bloxk_two", "dataset");
    block_null = nix::none;

    file_fs = File::open("test_entity", FileMode::Overwrite, Implementation::FileSys);
    block_fs = file_fs.createBlock("block_one", "dataset");
    block_other_fs = file_fs.createBlock("block_two", "dataset");
}


void TestEntity::tearDown() {
    file.close();
}


void TestEntity::testId() {
    CPPUNIT_ASSERT(block.id().size() == 36);
    CPPUNIT_ASSERT(util::toId(block).compare(block.id()) == 0);

    CPPUNIT_ASSERT(block_fs.id().size() == 36);
    CPPUNIT_ASSERT(util::toId(block_fs).compare(block_fs.id()) == 0);
}


void TestEntity::testName() {
    CPPUNIT_ASSERT(block.name() == "block_one");
    CPPUNIT_ASSERT(util::toName(block).compare(block.name()) == 0);

    CPPUNIT_ASSERT(block_fs.name() == "block_one");
    CPPUNIT_ASSERT(util::toName(block_fs).compare(block_fs.name()) == 0);
}


void TestEntity::testType() {
    test_type(block);
    test_type(block_fs);
}

void TestEntity::test_type(Block &b) {
    CPPUNIT_ASSERT(b.type() == "dataset");
    string typ = util::createId();
    b.type(typ);
    CPPUNIT_ASSERT(b.type() == typ);
}


void TestEntity::testDefinition() {
    test_definition(block);
    test_definition(block_fs);
}

void TestEntity::test_definition(Block &b) {
    string def = util::createId();
    b.definition(def);
    CPPUNIT_ASSERT(*b.definition() == def);
    b.definition(nix::none);
    CPPUNIT_ASSERT(b.definition() == nix::none);
}


void TestEntity::testCreatedAt() {
    test_created_at(block);
    test_created_at(block_fs);
}

void TestEntity::test_created_at(nix::Block &b) {
    CPPUNIT_ASSERT(b.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    b.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(b.createdAt() == past_time);
}


void TestEntity::testUpdatedAt() {
    CPPUNIT_ASSERT(block.updatedAt() >= startup_time);
    CPPUNIT_ASSERT(block_fs.updatedAt() >= startup_time);
}


void TestEntity::testOperators() {
    test_operators(block, block_other, block_null);
}

void TestEntity::test_operators(Block &b, Block &other, Block &null) {
    CPPUNIT_ASSERT(null == false);
    CPPUNIT_ASSERT(null == none);

    CPPUNIT_ASSERT(b != false);
    CPPUNIT_ASSERT(b != none);

    CPPUNIT_ASSERT(b == b);
    CPPUNIT_ASSERT(b != other);

    other = b;
    CPPUNIT_ASSERT(other == b);
}
