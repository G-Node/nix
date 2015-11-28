// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "BaseTestFile.hpp"

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>
#include <nix/file/AttributesFS.hpp>
#include <ctime>
#include <boost/filesystem.hpp>

using namespace nix;
using namespace valid;
namespace bfs = boost::filesystem;


void BaseTestFile::testOpen() {
    CPPUNIT_ASSERT_THROW(File::open("dummy", FileMode::Overwrite, "grewe"), std::runtime_error);
}


void BaseTestFile::testValidate() {
    valid::Result result = validate(file_open);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
}


void BaseTestFile::testFormat() {
    CPPUNIT_ASSERT(file_open.format() == "nix");
}


void BaseTestFile::testLocation() {
    CPPUNIT_ASSERT(file_open.location() == "test_file.h5");
    CPPUNIT_ASSERT(file_other.location() == "test_file_other.h5");
    /*
    // TODO this will not work with the filesystem...
    bfs::path p = boost::filesystem::current_path();
    bfs::path p_file("test_file");
    bfs::path p_other("other_file");
    CPPUNIT_ASSERT(file_fs.location() ==  (p / p_file).string());
    CPPUNIT_ASSERT(file_other_fs.location() == (p / p_other).string());
    */
}


void BaseTestFile::testVersion() {
    std::vector<int> version{1, 0, 0};
    CPPUNIT_ASSERT(file_open.version() == version);
}


void BaseTestFile::testCreatedAt() {
    CPPUNIT_ASSERT(file_open.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    file_open.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(file_open.createdAt() == past_time);
}


void BaseTestFile::testUpdatedAt() {
    CPPUNIT_ASSERT(file_open.updatedAt() >= startup_time);
    time_t now = time(NULL);
    file_open.forceUpdatedAt();
    CPPUNIT_ASSERT(file_open.updatedAt() >= now);
}


void BaseTestFile::testBlockAccess() {
    std::vector<std::string> names = { "block_a", "block_b", "block_c", "block_d", "block_e" };
    Block b;

    CPPUNIT_ASSERT(file_open.blockCount() == 0);
    CPPUNIT_ASSERT(file_open.blocks().size() == 0);
    CPPUNIT_ASSERT(file_open.getBlock("invalid_id") == false);
    CPPUNIT_ASSERT_EQUAL(false, file_open.hasBlock("invalid_id"));
    CPPUNIT_ASSERT(!file_open.hasBlock(b));

    std::vector<std::string> ids;
    for (const auto &name : names) {
        Block bl = file_open.createBlock(name, "dataset");
        CPPUNIT_ASSERT(bl.name() == name);
        CPPUNIT_ASSERT(file_open.hasBlock(bl));
        CPPUNIT_ASSERT(file_open.hasBlock(name));

        ids.push_back(bl.id());
    }

    CPPUNIT_ASSERT_THROW(file_open.createBlock(names[0], "dataset"), DuplicateName);
    CPPUNIT_ASSERT_THROW(file_open.createBlock("", "dataset"), EmptyString);
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
        CPPUNIT_ASSERT(file_open.hasBlock(id));
        CPPUNIT_ASSERT(bl.id() == id);

        file_open.deleteBlock(id);
    }

    CPPUNIT_ASSERT(!file_open.deleteBlock(b));
    b = file_open.createBlock("test","test");
    CPPUNIT_ASSERT_NO_THROW(file_open.getBlock(0));
    CPPUNIT_ASSERT_THROW(file_open.getBlock(file_open.blockCount()), nix::OutOfBounds);
    CPPUNIT_ASSERT(file_open.deleteBlock(b));
    CPPUNIT_ASSERT(file_open.blockCount() == 0);
    CPPUNIT_ASSERT(file_open.blocks().size() == 0);
    CPPUNIT_ASSERT(file_open.getBlock("invalid_id") == false);
}


void BaseTestFile::testSectionAccess() {
    std::vector<std::string> names = {"section_a", "section_b", "section_c", "section_d", "section_e" };
    Section s;
    CPPUNIT_ASSERT(file_open.sectionCount() == 0);
    CPPUNIT_ASSERT(file_open.sections().size() == 0);
    CPPUNIT_ASSERT(file_open.getSection("invalid_id") == false);
    CPPUNIT_ASSERT(!file_open.hasSection(s));

    std::vector<std::string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        Section sec = file_open.createSection(*it, "root section");
        CPPUNIT_ASSERT(sec.name() == *it);

        ids.push_back(sec.id());
    }
    CPPUNIT_ASSERT_THROW(file_open.createSection(names[0], "root section"), DuplicateName);
    CPPUNIT_ASSERT_THROW(file_open.createSection("", "root section"), EmptyString);

    CPPUNIT_ASSERT(file_open.sectionCount() == names.size());
    CPPUNIT_ASSERT(file_open.sections().size() == names.size());

    for (auto it = ids.begin(); it != ids.end(); it++) {
        Section sec = file_open.getSection(*it);
        CPPUNIT_ASSERT(file_open.hasSection(*it));
        CPPUNIT_ASSERT(sec.id() == *it);

        file_open.deleteSection(*it);
    }

    CPPUNIT_ASSERT(!file_open.deleteSection(s));
    s = file_open.createSection("test","test");
    CPPUNIT_ASSERT_NO_THROW(file_open.getSection(0));
    CPPUNIT_ASSERT_THROW(file_open.getSection(file_open.sectionCount()), nix::OutOfBounds);
    CPPUNIT_ASSERT(file_open.hasSection(s));
    CPPUNIT_ASSERT(file_open.deleteSection(s));
    CPPUNIT_ASSERT(file_open.sectionCount() == 0);
    CPPUNIT_ASSERT(file_open.sections().size() == 0);
    CPPUNIT_ASSERT(file_open.getSection("invalid_id") == false);
}


void BaseTestFile::testOperators(){
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


void BaseTestFile::testReopen() {
    Block b = file_open.createBlock("a", "a");
    b = none;
    file_open.close();

    file_open = nix::File::open("test_file_b.h5", FileMode::Overwrite);
    b = file_open.createBlock("b", "b");

    CPPUNIT_ASSERT(file_open.fileMode() == FileMode::Overwrite);
    // CPPUNIT_ASSERT_THROW(nix::File::open("test_file_b.h5", FileMode::Overwrite, (Implementation)2), std::runtime_error);
}

/*
void BaseTestFile::testCheckHeader() {
    file::AttributesFS attr(file_fs.location(), file_fs.fileMode());
    string frmt("xin");
    attr.set("format", frmt);
    CPPUNIT_ASSERT_THROW(File::open("test_file", FileMode::ReadWrite, Implementation::FileSys), std::runtime_error);
    attr.set("format", "nix");

    vector<int> version{2, 0, 0};
    attr.set("version", version);
    CPPUNIT_ASSERT_THROW(File::open("test_file", FileMode::ReadWrite, Implementation::FileSys), std::runtime_error);
}
*/
