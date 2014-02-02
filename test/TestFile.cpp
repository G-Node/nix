// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include "TestFile.hpp"

using namespace std;
using namespace nix;


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


void TestFile::testFormat() {
    CPPUNIT_ASSERT(file_open.format() == "pandora");
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

        file_open.removeBlock(*it);
    }

    CPPUNIT_ASSERT(file_open.blockCount() == 0);
    CPPUNIT_ASSERT(file_open.blocks().size() == 0);
}


void TestFile::testSectionAccess() {
    vector<string> names = {"section_a", "section_b", "section_c", "section_d", "section_e" };

    CPPUNIT_ASSERT(file_open.sectionCount() == 0);
    CPPUNIT_ASSERT(file_open.sections().size() == 0);

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

        file_open.removeSection(*it);
    }

    CPPUNIT_ASSERT(file_open.sectionCount() == 0);
    CPPUNIT_ASSERT(file_open.sections().size() == 0);
}


void TestFile::testOperators(){
    CPPUNIT_ASSERT(file_null == NULL);
    CPPUNIT_ASSERT(file_null == nullptr);

    CPPUNIT_ASSERT(file_open != NULL);
    CPPUNIT_ASSERT(file_open != nullptr);

    CPPUNIT_ASSERT(file_open == file_open);
    CPPUNIT_ASSERT(file_open != file_other);

    file_other = file_open;
    CPPUNIT_ASSERT(file_other == file_open);

    file_other = nullptr;
    CPPUNIT_ASSERT(file_null == NULL);
    CPPUNIT_ASSERT(file_null == nullptr);
}

