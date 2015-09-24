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
#include <nix/file/AttributesFS.hpp>
#include <ctime>
#include <boost/filesystem.hpp>

using namespace std;
using namespace nix;
using namespace valid;
namespace bfs = boost::filesystem;


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
    bfs::remove_all(bfs::path(file_fs.location().c_str()));
    bfs::remove_all(bfs::path(file_other_fs.location().c_str()));
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
    bfs::path p = boost::filesystem::current_path();
    bfs::path p_file("test_file");
    bfs::path p_other("other_file");
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
    test_block_access(file_open);
    test_block_access(file_fs);
}


void TestFile::test_block_access(nix::File &f) {
    vector<string> names = { "block_a", "block_b", "block_c", "block_d", "block_e" };
    Block b;

    CPPUNIT_ASSERT(f.blockCount() == 0);
    CPPUNIT_ASSERT(f.blocks().size() == 0);
    CPPUNIT_ASSERT(f.getBlock("invalid_id") == false);
    CPPUNIT_ASSERT_EQUAL(false, f.hasBlock("invalid_id"));
    CPPUNIT_ASSERT_THROW(f.hasBlock(b), UninitializedEntity);
    
    vector<string> ids;
    for (const auto &name : names) {
        Block bl = f.createBlock(name, "dataset");
        CPPUNIT_ASSERT(bl.name() == name);
        CPPUNIT_ASSERT(f.hasBlock(bl));
        CPPUNIT_ASSERT(f.hasBlock(name));

        ids.push_back(bl.id());
    }
    CPPUNIT_ASSERT_THROW(f.createBlock(names[0], "dataset"), DuplicateName);
    CPPUNIT_ASSERT_THROW(f.createBlock("", "dataset"), EmptyString);

    CPPUNIT_ASSERT(f.blockCount() == names.size());
    CPPUNIT_ASSERT(f.blocks().size() == names.size());

    for (const auto &name : names) {
        Block bl_name = f.getBlock(name);
        CPPUNIT_ASSERT(bl_name);

        Block bl_id = f.getBlock(bl_name.id());
        CPPUNIT_ASSERT(bl_id);
        CPPUNIT_ASSERT_EQUAL(bl_name.name(), bl_id.name());
    }

    for (const auto &id: ids) {
        Block bl = f.getBlock(id);
        CPPUNIT_ASSERT(f.hasBlock(id));
        CPPUNIT_ASSERT(bl.id() == id);

        f.deleteBlock(id);
    }
    CPPUNIT_ASSERT_THROW(f.deleteBlock(b), UninitializedEntity);
    b = f.createBlock("test","test");
    CPPUNIT_ASSERT_NO_THROW(f.getBlock(0));
    CPPUNIT_ASSERT_THROW(f.getBlock(f.blockCount()), nix::OutOfBounds);
    CPPUNIT_ASSERT(f.deleteBlock(b));
    CPPUNIT_ASSERT(f.blockCount() == 0);
    CPPUNIT_ASSERT(f.blocks().size() == 0);
    CPPUNIT_ASSERT(f.getBlock("invalid_id") == false);
}


void TestFile::testSectionAccess() {
    test_section_access(file_open);
    test_section_access(file_fs);
}


void TestFile::test_section_access(File &f) {
    vector<string> names = {"section_a", "section_b", "section_c", "section_d", "section_e" };
    Section s;
    CPPUNIT_ASSERT(file_open.sectionCount() == 0);
    CPPUNIT_ASSERT(file_open.sections().size() == 0);
    CPPUNIT_ASSERT(file_open.getSection("invalid_id") == false);
    CPPUNIT_ASSERT_THROW(file_open.hasSection(s), UninitializedEntity);

    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        Section sec = f.createSection(*it, "root section");
        CPPUNIT_ASSERT(sec.name() == *it);

        ids.push_back(sec.id());
    }
    CPPUNIT_ASSERT_THROW(f.createSection(names[0], "root section"), DuplicateName);
    CPPUNIT_ASSERT_THROW(f.createSection("", "root section"), EmptyString);

    CPPUNIT_ASSERT(f.sectionCount() == names.size());
    CPPUNIT_ASSERT(f.sections().size() == names.size());

    for (auto it = ids.begin(); it != ids.end(); it++) {
        Section sec = f.getSection(*it);
        CPPUNIT_ASSERT(f.hasSection(*it));
        CPPUNIT_ASSERT(sec.id() == *it);

        f.deleteSection(*it);
    }

    CPPUNIT_ASSERT_THROW(f.deleteSection(s), UninitializedEntity);
    s = f.createSection("test","test");
    CPPUNIT_ASSERT_NO_THROW(f.getSection(0));
    CPPUNIT_ASSERT_THROW(f.getSection(f.sectionCount()), nix::OutOfBounds);
    CPPUNIT_ASSERT(f.hasSection(s));
    CPPUNIT_ASSERT(f.deleteSection(s));
    CPPUNIT_ASSERT(f.sectionCount() == 0);
    CPPUNIT_ASSERT(f.sections().size() == 0);
    CPPUNIT_ASSERT(f.getSection("invalid_id") == false);
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

    CPPUNIT_ASSERT(file_open.fileMode() == FileMode::Overwrite);
    CPPUNIT_ASSERT(file_fs.fileMode() == FileMode::Overwrite);
    CPPUNIT_ASSERT_THROW(nix::File::open("test_file_b.h5", FileMode::Overwrite, (Implementation)2), std::runtime_error);
}


void TestFile::testCheckHeader() {
    file::AttributesFS attr(file_fs.location(), file_fs.fileMode());
    string frmt("xin");
    attr.set("format", frmt);
    CPPUNIT_ASSERT_THROW(File::open("test_file", FileMode::ReadWrite, Implementation::FileSys), std::runtime_error);
    attr.set("format", "nix");

    vector<int> version{2, 0, 0};
    attr.set("version", version);
    CPPUNIT_ASSERT_THROW(File::open("test_file", FileMode::ReadWrite, Implementation::FileSys), std::runtime_error);
}
