// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestFile.hpp"

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>

#include <ctime>
#include <boost/filesystem.hpp>

using namespace std;
using namespace nix;
using namespace valid;
namespace fs = boost::filesystem;


void TestFile::setUp() {
    startup_time = time(NULL);
    file_open = File::open("test_file.h5", FileMode::Overwrite);
    file_other = File::open("test_file_other.h5", FileMode::Overwrite);
    file_null = nix::none;

    file_fs = File::open("test_file", FileMode::Overwrite, Implementation::FileSys);
    file_other_fs = File::open("other_file", FileMode::Overwrite, Implementation::FileSys);
}


void TestFile::tearDown() {
    file_open.close();
    file_other.close();
    fs::remove_all(fs::path(file_fs.location().c_str()));
    fs::remove_all(fs::path(file_other_fs.location().c_str()));
}


void TestFile::testValidate() {
    valid::Result result = validate(file_open);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
}


void TestFile::testFormat() {
    CPPUNIT_ASSERT(file_open.format() == "nix");
    CPPUNIT_ASSERT(file_fs.format() == "nix");
}


void TestFile::testLocation() {
    CPPUNIT_ASSERT(file_open.location() == "test_file.h5");
    CPPUNIT_ASSERT(file_other.location() == "test_file_other.h5");
    fs::path p = boost::filesystem::current_path();
    fs::path p_file("test_file");
    fs::path p_other("other_file");
    CPPUNIT_ASSERT(file_fs.location() ==  (p / p_file).string());
    CPPUNIT_ASSERT(file_other_fs.location() == (p / p_other).string());
}


void TestFile::testVersion() {
    vector<int> version{1, 0, 0};
    CPPUNIT_ASSERT(file_open.version() == version);
    CPPUNIT_ASSERT(file_fs.version() == version);
}


void TestFile::testCreatedAt() {
    CPPUNIT_ASSERT(file_open.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    file_open.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(file_open.createdAt() == past_time);

    CPPUNIT_ASSERT(file_fs.createdAt() >= startup_time);
    file_fs.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(file_fs.createdAt() == past_time);
}


void TestFile::testUpdatedAt() {
    CPPUNIT_ASSERT(file_open.updatedAt() >= startup_time);
    CPPUNIT_ASSERT(file_fs.updatedAt() >= startup_time);
    time_t now = time(NULL);
    file_fs.forceUpdatedAt();
    file_open.forceUpdatedAt();
    CPPUNIT_ASSERT(file_open.updatedAt() >= now);
    CPPUNIT_ASSERT(file_fs.updatedAt() >= now);
}


void TestFile::testBlockAccess() {
    vector<string> names = { "block_a", "block_b", "block_c", "block_d", "block_e" };
    Block b;
    CPPUNIT_ASSERT(file_open.blockCount() == 0);
    CPPUNIT_ASSERT(file_open.blocks().size() == 0);
    CPPUNIT_ASSERT(file_open.getBlock("invalid_id") == false);
    CPPUNIT_ASSERT_EQUAL(false, file_open.hasBlock("invalid_id"));
    CPPUNIT_ASSERT_THROW(file_open.hasBlock(b), UninitializedEntity);
    
    vector<string> ids;
    for (const auto &name : names) {
        Block bl = file_open.createBlock(name, "dataset");
        CPPUNIT_ASSERT(bl.name() == name);
        CPPUNIT_ASSERT(file_open.hasBlock(bl));
        CPPUNIT_ASSERT(file_open.hasBlock(name));

        ids.push_back(bl.id());
    }
    CPPUNIT_ASSERT_THROW(file_open.createBlock(names[0], "dataset"),
                         DuplicateName);

    CPPUNIT_ASSERT(file_open.blockCount() == names.size());
    CPPUNIT_ASSERT(file_open.blocks().size() == names.size());

    for (const auto &name : names) {
        Block bl_name = file_open.getBlock(name);
        CPPUNIT_ASSERT(bl_name);

        Block bl_id = file_open.getBlock(bl_name.id());
        CPPUNIT_ASSERT(bl_id);
        CPPUNIT_ASSERT_EQUAL(bl_name.name(), bl_id.name());
    }
    
    for (const auto &id: ids) {
        Block bl = file_open.getBlock(id);
        CPPUNIT_ASSERT(file_open.hasBlock(id) == true);
        CPPUNIT_ASSERT(bl.id() == id);

        file_open.deleteBlock(id);
    }
    CPPUNIT_ASSERT_THROW(file_open.deleteBlock(b), UninitializedEntity);
    b = file_open.createBlock("test","test");
    CPPUNIT_ASSERT_NO_THROW(file_open.getBlock(0));
    CPPUNIT_ASSERT_THROW(file_open.getBlock(file_open.blockCount()), nix::OutOfBounds);
    CPPUNIT_ASSERT(file_open.deleteBlock(b));
    CPPUNIT_ASSERT(file_open.blockCount() == 0);
    CPPUNIT_ASSERT(file_open.blocks().size() == 0);
    CPPUNIT_ASSERT(file_open.getBlock("invalid_id") == false);
}


void TestFile::testSectionAccess() {
    vector<string> names = {"section_a", "section_b", "section_c", "section_d", "section_e" };
    Section s;
    CPPUNIT_ASSERT(file_open.sectionCount() == 0);
    CPPUNIT_ASSERT(file_open.sections().size() == 0);
    CPPUNIT_ASSERT(file_open.getSection("invalid_id") == false);
    CPPUNIT_ASSERT_THROW(file_open.hasSection(s), UninitializedEntity);

    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        Section sec = file_open.createSection(*it, "root section");
        CPPUNIT_ASSERT(sec.name() == *it);

        ids.push_back(sec.id());
    }
    CPPUNIT_ASSERT_THROW(file_open.createSection(names[0], "root section"),
                         DuplicateName);

    CPPUNIT_ASSERT(file_open.sectionCount() == names.size());
    CPPUNIT_ASSERT(file_open.sections().size() == names.size());

    for (auto it = ids.begin(); it != ids.end(); it++) {
        Section sec = file_open.getSection(*it);
        CPPUNIT_ASSERT(file_open.hasSection(*it) == true);
        CPPUNIT_ASSERT(sec.id() == *it);

        file_open.deleteSection(*it);
    }
    CPPUNIT_ASSERT_THROW(file_open.deleteSection(s), UninitializedEntity);
    s = file_open.createSection("test","test");
    CPPUNIT_ASSERT_NO_THROW(file_open.getSection(0));
    CPPUNIT_ASSERT_THROW(file_open.getSection(file_open.sectionCount()), nix::OutOfBounds);
    CPPUNIT_ASSERT(file_open.hasSection(s));
    CPPUNIT_ASSERT(file_open.deleteSection(s));
    CPPUNIT_ASSERT(file_open.sectionCount() == 0);
    CPPUNIT_ASSERT(file_open.sections().size() == 0);
    CPPUNIT_ASSERT(file_open.getSection("invalid_id") == false);

    // Tests for the filesystem backend
    CPPUNIT_ASSERT(file_fs.sectionCount() == 0);
    CPPUNIT_ASSERT(file_fs.sections().size() == 0);
    CPPUNIT_ASSERT(file_fs.getSection("invalid_id") == false);

    s = file_fs.createSection("test_sec", "test");
    CPPUNIT_ASSERT(file_fs.sectionCount() == 1);

    CPPUNIT_ASSERT(file_fs.hasSection("test_sec"));
    CPPUNIT_ASSERT(!file_fs.hasSection("unknown section"));

    Section s2 = file_fs.getSection("test_sec");
    CPPUNIT_ASSERT(s.compare(s2) == 0 );

    s2 = file_fs.getSection(s.id());
    CPPUNIT_ASSERT(s.compare(s2) == 0 );

    s2 = file_fs.getSection(0);
    CPPUNIT_ASSERT(s.compare(s2) == 0 );
    CPPUNIT_ASSERT_THROW(file_fs.getSection(1), nix::OutOfBounds);

    CPPUNIT_ASSERT(file_fs.deleteSection(s.name()));
    CPPUNIT_ASSERT(file_fs.sectionCount() == 0);

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

    CPPUNIT_ASSERT(file_fs != file_other_fs);
    CPPUNIT_ASSERT(!(file_fs == file_other_fs));

    CPPUNIT_ASSERT(file_fs != false);
    CPPUNIT_ASSERT(file_fs != none);
}

void TestFile::testReopen() {
    //file_open is currently open
    Block b = file_open.createBlock("a", "a");
    b = none;
    file_open.close();

    file_open = nix::File::open("test_file_b.h5", FileMode::Overwrite);
    b = file_open.createBlock("b", "b");
}

