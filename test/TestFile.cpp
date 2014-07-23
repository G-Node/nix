// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include "TestFile.hpp"
#include <nix/util/util.hpp>

#include <nix/valid/validate.hpp>

using namespace std;
using namespace nix;
using namespace valid;


void TestFile::setUp() {
    statup_time = time(NULL);
    file_open = File::open("test_file.h5", FileMode::Overwrite);
    file_other = File::open("test_file_other.h5", FileMode::Overwrite);
    file_null = NULL;
}


void TestFile::tearDown() {
    file_open.close();
    file_other.close();
}


void TestFile::testValidate() {
    valid::Result result = validate(file_open);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
}


void TestFile::testFormat() {
    CPPUNIT_ASSERT(file_open.format() == "nix");
}


void TestFile::testLocation() {
    CPPUNIT_ASSERT(file_open.location() == "test_file.h5");
    CPPUNIT_ASSERT(file_other.location() == "test_file_other.h5");
}


void TestFile::testVersion() {
    CPPUNIT_ASSERT(file_open.version() == "1.0");
}


void TestFile::testCreatedAt() {
    CPPUNIT_ASSERT(file_open.createdAt() >= statup_time);
    time_t past_time = time(NULL) - 10000000;
    file_open.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(file_open.createdAt() == past_time);
}


void TestFile::testUpdatedAt() {
    CPPUNIT_ASSERT(file_open.updatedAt() >= statup_time);
}


void TestFile::testBlockAccess() {
    vector<string> names = { "block_a", "block_b", "block_c", "block_d", "block_e" };

    CPPUNIT_ASSERT(file_open.blockCount() == 0);
    CPPUNIT_ASSERT(file_open.blocks().size() == 0);
    CPPUNIT_ASSERT(file_open.getBlock("invalid_id") == false);

    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        Block bl = file_open.createBlock(*it, "dataset");
        CPPUNIT_ASSERT(bl.name() == *it);

        ids.push_back(bl.id());
    }

    CPPUNIT_ASSERT(file_open.blockCount() == names.size());
    CPPUNIT_ASSERT(file_open.blocks().size() == names.size());

    for (auto it = ids.begin(); it != ids.end(); it++) {
        Block bl = file_open.getBlock(*it);
        CPPUNIT_ASSERT(file_open.hasBlock(*it) == true);
        CPPUNIT_ASSERT(bl.id() == *it);

        file_open.deleteBlock(*it);
    }

    CPPUNIT_ASSERT(file_open.blockCount() == 0);
    CPPUNIT_ASSERT(file_open.blocks().size() == 0);
    CPPUNIT_ASSERT(file_open.getBlock("invalid_id") == false);
}


void TestFile::testSectionAccess() {
    vector<string> names = {"section_a", "section_b", "section_c", "section_d", "section_e" };

    CPPUNIT_ASSERT(file_open.sectionCount() == 0);
    CPPUNIT_ASSERT(file_open.sections().size() == 0);
    CPPUNIT_ASSERT(file_open.getSection("invalid_id") == false);

    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        Section sec = file_open.createSection(*it, "root section");
        CPPUNIT_ASSERT(sec.name() == *it);

        ids.push_back(sec.id());
    }

    CPPUNIT_ASSERT(file_open.sectionCount() == names.size());
    CPPUNIT_ASSERT(file_open.sections().size() == names.size());

    for (auto it = ids.begin(); it != ids.end(); it++) {
        Section sec = file_open.getSection(*it);
        CPPUNIT_ASSERT(file_open.hasSection(*it) == true);
        CPPUNIT_ASSERT(sec.id() == *it);

        file_open.deleteSection(*it);
    }

    CPPUNIT_ASSERT(file_open.sectionCount() == 0);
    CPPUNIT_ASSERT(file_open.sections().size() == 0);
    CPPUNIT_ASSERT(file_open.getSection("invalid_id") == false);
}


void TestFile::testOperators(){
    CPPUNIT_ASSERT(file_null == false);
    CPPUNIT_ASSERT(file_null == none);

    CPPUNIT_ASSERT(file_open != false);
    CPPUNIT_ASSERT(file_open != none);

    CPPUNIT_ASSERT(file_open == file_open);
    CPPUNIT_ASSERT(file_open != file_other);

    file_other = file_open;
    CPPUNIT_ASSERT(file_other == file_open);

    file_other = none;
    CPPUNIT_ASSERT(file_null == false);
    CPPUNIT_ASSERT(file_null == none);
}

void TestFile::testReopen() {

    //file_open is currently open
    Block b = file_open.createBlock("a", "a");
    b = none;
    file_open.close();

    file_open = nix::File::open("test_file_b.h5");
    b = file_open.createBlock("b", "b");
}

