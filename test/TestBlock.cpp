// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestBlock.hpp"

#include <nix/util/util.hpp>

#include <nix/valid/validate.hpp>
#include <nix/Exception.hpp>

#include <ctime>

using namespace std;
using namespace nix;
using namespace valid;


void TestBlock::setUp() {
    startup_time = time(NULL);
    file = File::open("test_block.h5", FileMode::Overwrite);

    section = file.createSection("foo_section", "metadata");

    block = file.createBlock("block_one", "dataset");
    block_other = file.createBlock("block_two", "dataset");
    block_null  = nix::none;

    file_fs = File::open("test_block", FileMode::Overwrite, Implementation::FileSys);
    block_fs = file_fs.createBlock("block_one", "dataset");
    section_fs = file_fs.createSection("foo_section", "metadata");

}


void TestBlock::tearDown() {
    file.close();
}


void TestBlock::testValidate() {
    valid::Result result = validate(block);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void TestBlock::testId() {
    CPPUNIT_ASSERT(block.id().size() == 36);
    CPPUNIT_ASSERT(block_fs.id().size() == 36);
}


void TestBlock::testName() {
    CPPUNIT_ASSERT(block.name() == "block_one");
    CPPUNIT_ASSERT(block_fs.name() == "block_one");
}


void TestBlock::testType() {
    CPPUNIT_ASSERT(block.type() == "dataset");
    string typ = util::createId();
    block.type(typ);
    CPPUNIT_ASSERT(block.type() == typ);

    CPPUNIT_ASSERT(block_fs.type() == "dataset");
    block_fs.type(typ);
    CPPUNIT_ASSERT(block_fs.type() == typ);
}


void TestBlock::testDefinition() {
    string def = util::createId();
    block.definition(def);
    CPPUNIT_ASSERT(*block.definition() == def);

    block_fs.definition(def);
    CPPUNIT_ASSERT(*block_fs.definition() == def);
}


void TestBlock::testMetadataAccess() {
    cerr << "\n\t Backend: HDF5\t";
    test_metadata_access(file, block, section);
    cerr << "\t... done!\n";
    cerr << "\t Backend: filesystem\t";
    test_metadata_access(file_fs, block_fs, section_fs);
    cerr << "... done!\n";
}


void TestBlock::test_metadata_access(File &f, Block &b, Section &s) {
    CPPUNIT_ASSERT(!b.metadata());
    b.metadata(s);
    CPPUNIT_ASSERT(b.metadata());
    // test none-unsetter
    b.metadata(none);
    CPPUNIT_ASSERT(!b.metadata());
    // test deleter removing link too
    b.metadata(s);
    f.deleteSection(s.id());
    CPPUNIT_ASSERT(!b.metadata());
    // re-create section
    s = f.createSection("foo_section", "metadata");
}


void TestBlock::testSourceAccess() {
    cerr << "\n\t Backend: HDF5\t";
    test_source_access(file, block);
    cerr << "\t... done!\n";
    cerr << "\t Backend: filesystem\t";
    test_source_access(file_fs, block_fs);
    cerr << "... done!\n";
}

void TestBlock::test_source_access(nix::File &f, nix::Block &b) {
    vector<string> names = { "source_a", "source_b", "source_c", "source_d", "source_e" };
    Source s;

    CPPUNIT_ASSERT_THROW(block.hasSource(s), UninitializedEntity);
    CPPUNIT_ASSERT(block.sourceCount() == 0);
    CPPUNIT_ASSERT(block.sources().size() == 0);
    CPPUNIT_ASSERT(block.getSource("invalid_id") == false);
    CPPUNIT_ASSERT(!block.hasSource("invalid_id"));
    CPPUNIT_ASSERT(b.getSource(b.sourceCount() + 10) == nullptr);

    vector<string> ids;
    for (const auto &name : names) {
        Source src = b.createSource(name, "channel");
        CPPUNIT_ASSERT(src.name() == name);
        CPPUNIT_ASSERT(b.hasSource(name));
        CPPUNIT_ASSERT(b.hasSource(src));

        ids.push_back(src.id());
    }
    CPPUNIT_ASSERT_THROW(b.createSource(names[0], "channel"),
                         DuplicateName);
    CPPUNIT_ASSERT_THROW(b.createSource("", "test"), nix::EmptyString);

    CPPUNIT_ASSERT(b.sourceCount() == names.size());
    CPPUNIT_ASSERT(b.sources().size() == names.size());
    CPPUNIT_ASSERT_THROW(b.getSource(b.sourceCount() + 10), OutOfBounds);

    for (const auto &id : ids) {
        Source src = b.getSource(id);
        CPPUNIT_ASSERT(b.hasSource(id) == true);
        CPPUNIT_ASSERT(src.id() == id);
        b.deleteSource(id);
    }

    s = b.createSource("test", "test");
    CPPUNIT_ASSERT(b.sourceCount() == 1);
    CPPUNIT_ASSERT_NO_THROW(b.deleteSource(s));

    CPPUNIT_ASSERT(b.sourceCount() == 0);
    CPPUNIT_ASSERT(b.sources().size() == 0);
    CPPUNIT_ASSERT(b.getSource("invalid_id") == false);
}

void TestBlock::testDataArrayAccess() {
    cerr << "\n\t Backend: HDF5\t";
    test_data_array_access(block);
    cerr << "\t... done!\n";
    cerr << "\t Backend: filesystem\t";
    test_data_array_access(block_fs);
    cerr << "... done!\n";
}

void TestBlock::test_data_array_access(Block &b) {
    vector<string> names = { "data_array_a", "data_array_b", "data_array_c",
                             "data_array_d", "data_array_e" };
    DataArray data_array, a;

    CPPUNIT_ASSERT(b.dataArrayCount() == 0);
    CPPUNIT_ASSERT(b.dataArrays().size() == 0);
    CPPUNIT_ASSERT(b.getDataArray("invalid_id") == false);

    vector<string> ids;
    for (const auto &name : names) {
        data_array = b.createDataArray(name, "channel", DataType::Double, nix::NDSize({ 0 }));
        CPPUNIT_ASSERT(data_array.name() == name);
        CPPUNIT_ASSERT(data_array.type() == "channel");
        ids.push_back(data_array.id());
    }

    CPPUNIT_ASSERT_THROW(b.createDataArray(names[0], "channel", DataType::Double, nix::NDSize({ 0 })), DuplicateName);
    CPPUNIT_ASSERT_THROW(b.createDataArray("", "channel", DataType::Double, nix::NDSize({ 0 })), EmptyString);

    CPPUNIT_ASSERT(b.hasDataArray(data_array));
    CPPUNIT_ASSERT_THROW(block.hasDataArray(a), UninitializedEntity);
    CPPUNIT_ASSERT(b.dataArrayCount() == names.size());
    CPPUNIT_ASSERT(b.dataArrays().size() == names.size());

    for (const auto &name : names) {
        DataArray da_name = b.getDataArray(name);
        CPPUNIT_ASSERT(da_name);
        DataArray da_id = b.getDataArray(da_name.id());
        CPPUNIT_ASSERT(da_id);
        CPPUNIT_ASSERT_EQUAL(da_name.name(), da_id.name());
    }

    vector<DataArray> filteredArrays = b.dataArrays(util::TypeFilter<DataArray>("channel"));
    CPPUNIT_ASSERT_EQUAL(names.size(), filteredArrays.size());

    filteredArrays = b.dataArrays(util::NameFilter<DataArray>("data_array_c"));
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), filteredArrays.size());
    if (filteredArrays.size() > 0) {
        boost::optional<std::string> name = filteredArrays[0].name();
        CPPUNIT_ASSERT(name && *name == "data_array_c");
    }

    for (auto it = ids.begin(); it != ids.end(); it++) {
        DataArray data_array = b.getDataArray(*it);
        CPPUNIT_ASSERT(b.hasDataArray(*it) == true);
        CPPUNIT_ASSERT(data_array.id() == *it);

        b.deleteDataArray(*it);
    }
    CPPUNIT_ASSERT_THROW(block.deleteDataArray(a), UninitializedEntity);
    CPPUNIT_ASSERT(block.dataArrayCount() == 0);
    CPPUNIT_ASSERT(block.dataArrays().size() == 0);
    CPPUNIT_ASSERT(block.getDataArray("invalid_id") == false);
}


void TestBlock::testTagAccess() {
    cerr << "\n\t Backend: HDF5\t";
    test_tag_access(block);
    cerr << "\t... done!\n";
    cerr << "\t Backend: filesystem\t";
    test_tag_access(block_fs);
    cerr << "... done!\n";
}

void TestBlock::test_tag_access(Block &b) {
    vector<string> names = { "tag_a", "tag_b", "tag_c", "tag_d", "tag_e" };
    vector<string> array_names = { "data_array_a", "data_array_b", "data_array_c",
                                   "data_array_d", "data_array_e" };
    vector<DataArray> refs;
    Tag tag, t;
    for (const auto &name : array_names) {
        refs.push_back(b.createDataArray(name,
                                         "reference",
                                         DataType::Double,
                                         nix::NDSize({ 0 })));
    }

    CPPUNIT_ASSERT(block.tagCount() == 0);
    CPPUNIT_ASSERT(block.tags().size() == 0);
    CPPUNIT_ASSERT(block.getTag("invalid_id") == false);
    CPPUNIT_ASSERT_THROW(block.hasTag(t), UninitializedEntity);

    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); ++it) {
        tag = b.createTag(*it, "segment", {0.0, 2.0, 3.4});
        tag.references(refs);
        CPPUNIT_ASSERT(tag.name() == *it);
        CPPUNIT_ASSERT(b.hasTag(tag));
        ids.push_back(tag.id());
    }
    CPPUNIT_ASSERT_THROW(b.createTag(names[0], "segment", {0.0, 2.0, 3.4}),
                         DuplicateName);

    // CPPUNIT_ASSERT_THROW(b.createTag("", "segment", {0.0, 2.0, 3.4}),
    //                      EmptyString);

    CPPUNIT_ASSERT(b.tagCount() == names.size());
    CPPUNIT_ASSERT(b.tags().size() == names.size());

    for (auto it = ids.begin(); it != ids.end(); ++it) {
        tag = b.getTag(*it);
        CPPUNIT_ASSERT(b.hasTag(*it) == true);
        CPPUNIT_ASSERT(tag.id() == *it);

        b.deleteTag(*it);
    }

    tag = block.createTag("test", "test", {0.0});
    CPPUNIT_ASSERT(block.hasTag(tag));
    CPPUNIT_ASSERT_NO_THROW(block.deleteTag(tag));
    CPPUNIT_ASSERT_THROW(block.deleteTag(t), UninitializedEntity);

    CPPUNIT_ASSERT(block.tagCount() == 0);
    CPPUNIT_ASSERT(block.tags().size() == 0);
    CPPUNIT_ASSERT(block.getTag("invalid_id") == false);
}


void TestBlock::testMultiTagAccess() {
    cerr << "\n\t Backend: HDF5\t";
    test_multi_tag_access(block);
    cerr << "\t... done!\n";
    cerr << "\t Backend: filesystem\t";
    test_multi_tag_access(block_fs);
    cerr << "... done!\n";
}

void TestBlock::test_multi_tag_access(Block &b) {
    vector<string> names = { "tag_a", "tag_b", "tag_c", "tag_d", "tag_e" };
    MultiTag mtag, m;
    // create a valid positions data array below
    DataArray positions = b.createDataArray("array_one",
                                            "testdata",
                                            DataType::Double,
                                            nix::NDSize({ 3, 4, 2 }));

    CPPUNIT_ASSERT(block.multiTagCount() == 0);
    CPPUNIT_ASSERT(block.multiTags().size() == 0);
    CPPUNIT_ASSERT(block.getMultiTag("invalid_id") == false);
    CPPUNIT_ASSERT_THROW(block.hasMultiTag(m), UninitializedEntity);

    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        mtag = b.createMultiTag(*it, "segment", positions);
        CPPUNIT_ASSERT(mtag.name() == *it);
        CPPUNIT_ASSERT(b.hasMultiTag(mtag));
        ids.push_back(mtag.id());
    }
    CPPUNIT_ASSERT_THROW(b.createMultiTag(names[0], "segment", positions),
                         DuplicateName);

    // CPPUNIT_ASSERT_THROW(b.createMultiTag("", "segment", positions),
    //                      EmptyString);

    CPPUNIT_ASSERT(b.multiTagCount() == names.size());
    CPPUNIT_ASSERT(b.multiTags().size() == names.size());

    for (auto it = ids.begin(); it != ids.end(); it++) {
        mtag = b.getMultiTag(*it);
        CPPUNIT_ASSERT(b.hasMultiTag(*it) == true);
        CPPUNIT_ASSERT(mtag.id() == *it);

        b.deleteMultiTag(*it);
    }
    mtag = block.createMultiTag("test", "test", positions);
    CPPUNIT_ASSERT(block.hasMultiTag(mtag));
    CPPUNIT_ASSERT_THROW(block.deleteMultiTag(m), UninitializedEntity);
    CPPUNIT_ASSERT_NO_THROW(block.deleteMultiTag(mtag));
    CPPUNIT_ASSERT(block.multiTagCount() == 0);
    CPPUNIT_ASSERT(block.multiTags().size() == 0);
    CPPUNIT_ASSERT(block.getMultiTag("invalid_id") == false);
}


void TestBlock::testOperators() {
    CPPUNIT_ASSERT(block_null == false);
    CPPUNIT_ASSERT(block_null == none);

    CPPUNIT_ASSERT(block != false);
    CPPUNIT_ASSERT(block != none);

    CPPUNIT_ASSERT(block == block);
    CPPUNIT_ASSERT(block != block_other);

    block_other = block;
    CPPUNIT_ASSERT(block == block_other);

    block_other = none;
    CPPUNIT_ASSERT(block_null == false);
    CPPUNIT_ASSERT(block_null == none);
}


void TestBlock::testCreatedAt() {
    CPPUNIT_ASSERT(block.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    block.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(block.createdAt() == past_time);
}


void TestBlock::testUpdatedAt() {
    CPPUNIT_ASSERT(block.updatedAt() >= startup_time);
}


void TestBlock::testCompare() {
    string other_name = block_other.name();
    string block_name = block.name();
        
    CPPUNIT_ASSERT(block.compare(block) == 0);
    CPPUNIT_ASSERT(block.compare(block_other) ==  block_name.compare(other_name));
}
