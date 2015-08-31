// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestEntityWithMetadata.hpp"
#include <nix/util/util.hpp>

using namespace std;
using namespace nix;


void TestEntityWithMetadata::setUp() {
    file = File::open("test_block.h5", FileMode::Overwrite);
    section = file.createSection("foo_section", "metadata");
    block = file.createBlock("block_one", "dataset");

    file_fs = File::open("test_block", FileMode::Overwrite, Implementation::FileSys);
    section_fs = file_fs.createSection("foo_section", "metadata");
    block_fs = file_fs.createBlock("block_one", "dataset");
}


void TestEntityWithMetadata::tearDown() {
    file.close();
}


void TestEntityWithMetadata::testMetadataAccess() {
    test_metadata_access(file, block, section, section_fs);
    test_metadata_access(file_fs, block_fs, section_fs, section);
}

void TestEntityWithMetadata::test_metadata_access(File &f, Block &b, Section &s, Section &wrong) {
    Section sec = f.createSection("new_sec", "test");
    CPPUNIT_ASSERT(!b.metadata());

    b.metadata(s);
    CPPUNIT_ASSERT(b.metadata());
    CPPUNIT_ASSERT(b.metadata().id() == s.id());

    b.metadata(sec);
    CPPUNIT_ASSERT(b.metadata().id() == sec.id());

    CPPUNIT_ASSERT_THROW(b.metadata(wrong.id()), runtime_error);
    CPPUNIT_ASSERT_THROW(b.metadata(""), EmptyString);
    // test none-unsetter
    b.metadata(none);
    CPPUNIT_ASSERT(!b.metadata());
    // test deleter removing link too
    b.metadata(s);
    f.deleteSection(s.id());
    CPPUNIT_ASSERT(!b.metadata());
    // re-create section
    s = f.createSection("foo_section", "metadata");
    f.deleteSection(sec);
}




