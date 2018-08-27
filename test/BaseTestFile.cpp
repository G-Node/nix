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
#include <ctime>
#include <boost/filesystem.hpp>

using namespace nix;
using namespace valid;
namespace bfs = boost::filesystem;


void BaseTestFile::testOpen() {
    CPPUNIT_ASSERT_THROW(File::open("dummy", FileMode::ReadOnly), std::runtime_error);
    CPPUNIT_ASSERT_THROW(File::open("dummy", FileMode::Overwrite, "grewe"), std::runtime_error);
}


void BaseTestFile::testFlush() {
    CPPUNIT_ASSERT(file_open.flush());
    for (int i = 0; i < 10; ++i) {
        file_open.createBlock("test_" + nix::util::numToStr(i), "test");
        CPPUNIT_ASSERT(file_open.flush());
    }
}

void BaseTestFile::testValidate() {
    valid::Result result = validate(file_open);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
}


void BaseTestFile::testFormat() {
    CPPUNIT_ASSERT(file_open.format() == "nix");
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

    file_open = openFile("test_file_b", FileMode::Overwrite);
    b = file_open.createBlock("b", "b");

    CPPUNIT_ASSERT(file_open.fileMode() == FileMode::Overwrite);
}

#define ASSERT_FLAGS_EQUAL(want, have) \
    CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(have), \
                         static_cast<unsigned long>(want));

void BaseTestFile::testFlags() {
    nix::OpenFlags flags = nix::OpenFlags::None;

    ASSERT_FLAGS_EQUAL(nix::OpenFlags::None, flags & nix::OpenFlags::Force);
    ASSERT_FLAGS_EQUAL(nix::OpenFlags::Force, flags | nix::OpenFlags::Force);

    flags = nix::OpenFlags::Force;
    ASSERT_FLAGS_EQUAL(nix::OpenFlags::Force, flags & nix::OpenFlags::Force);
    ASSERT_FLAGS_EQUAL(nix::OpenFlags::Force, flags | nix::OpenFlags::Force);

    flags = static_cast<OpenFlags>(0xFF); // simulate we have more flags
    ASSERT_FLAGS_EQUAL(nix::OpenFlags::Force, flags & nix::OpenFlags::Force);
}
