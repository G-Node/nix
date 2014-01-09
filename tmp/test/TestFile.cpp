// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestFile.hpp"

void TestFile::setUp() {
    f1 = new File("test_block.h5", FileMode::ReadWrite);
}

void TestFile::tearDown() {
    delete f1;
}
/*
 void TestFile::testFormat() {
 string format = f1->format();
 stringstream errmsg;
 errmsg << "Format doesn't match: " << format << " != " << File::FORMAT;
 CPPUNIT_ASSERT_MESSAGE(errmsg.str(), format == File::FORMAT);
 }

 void TestFile::testCreatedAt() {
 CPPUNIT_ASSERT_MESSAGE("Creation time missing", f1->createdAt() > 0);
 }

 void TestFile::testUpdatedAt() {
 CPPUNIT_ASSERT_MESSAGE("Update time missing", f1->updatedAt() > 0);
 }
 */
void TestFile::testBlockAccess() {
    std::vector<std::string> ids;
    size_t count = f1->blockCount();
    const char *names[5] = { "block_a", "block_b", "block_c", "block_d",
                             "block_e" };
    for (int i = 0; i < 5; i++) {
        Block b1 = f1->createBlock(names[i], "dataset");
        Block b2 = f1->getBlock(b1.id());
        ids.push_back(b1.id());
        stringstream errmsg;
        errmsg << "Error while accessing block: b1.id() = " << b1.id()
               << " / b2.id() = " << b2.id();
        CPPUNIT_ASSERT_MESSAGE(errmsg.str(), b1.id().compare(b2.id()) == 0);
    }

    for (size_t i = 0; i < ids.size(); i++) {
        f1->removeBlock(ids[i]);
    }

    stringstream errmsg1;
    errmsg1 << "Error while removing blocks!";
    CPPUNIT_ASSERT_MESSAGE(errmsg1.str(), f1->blockCount() == count);
}

void TestFile::testSectionAccess() {
    const char *names[5] = { "section_a", "section_b", "section_c", "section_d",
                             "section_e" };
    std::vector<std::string> ids;
    size_t count = f1->sectionCount();
    for (int i = 0; i < 5; i++) {
        Section s1(f1->createSection(names[i], "Recording"));
        Section s2 = f1->findSection(s1.id())[0];
        ids.push_back(s1.id());
        stringstream errmsg;
        errmsg << "Error while accessing section: s1.id() = " << s1.id()
               << " / s2.id() = " << s2.id();
        CPPUNIT_ASSERT_MESSAGE(errmsg.str(), s1 == s2);
    }

    for (size_t i = 0; i < ids.size(); i++) {
        f1->removeSection(ids[i]);
    }
    stringstream errmsg1;
    errmsg1 << "Error while removing sections!";
    CPPUNIT_ASSERT_MESSAGE(errmsg1.str(), f1->sectionCount() == count);
}

void TestFile::testOperators(){
    Section s1(f1->createSection("Test", "Recording"));
    Section s2 = f1->findSection(s1.id())[0];
    Section s3(f1->createSection("2ndTest","Recording"));
    Section s4 = s3;
    CPPUNIT_ASSERT_EQUAL(s1, s2);

    CPPUNIT_ASSERT(s1 == s2);
    CPPUNIT_ASSERT(s1 != s3);
    CPPUNIT_ASSERT(s3 == s4);

    f1->removeSection(s1.id());
    f1->removeSection(s3.id());
}
