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
    block_null  = nullptr;
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
}


void TestBlock::testName() {
    CPPUNIT_ASSERT(block.name() == "block_one");
}


void TestBlock::testType() {
    CPPUNIT_ASSERT(block.type() == "dataset");
    string typ = util::createId();
    block.type(typ);
    CPPUNIT_ASSERT(block.type() == typ);
}


void TestBlock::testDefinition() {
    string def = util::createId();
    block.definition(def);
    CPPUNIT_ASSERT(*block.definition() == def);
}


void TestBlock::testMetadataAccess() {
    CPPUNIT_ASSERT(!block.metadata());

    block.metadata(section);
    CPPUNIT_ASSERT(block.metadata());
    
    // test none-unsetter
    block.metadata(none);
    CPPUNIT_ASSERT(!block.metadata());
    // test deleter removing link too
    block.metadata(section);
    file.deleteSection(section.id());
    CPPUNIT_ASSERT(!block.metadata());
    // re-create section
    section = file.createSection("foo_section", "metadata");
}


void TestBlock::testSourceAccess() {
    vector<string> names = { "source_a", "source_b", "source_c", "source_d", "source_e" };

    CPPUNIT_ASSERT(block.sourceCount() == 0);
    CPPUNIT_ASSERT(block.sources().size() == 0);
    CPPUNIT_ASSERT(block.getSource("invalid_id") == false);
    CPPUNIT_ASSERT(!block.hasSource("invalid_id"));


    vector<string> ids;
    for (const auto &name : names) {
        Source src = block.createSource(name, "channel");
        CPPUNIT_ASSERT(src.name() == name);
        CPPUNIT_ASSERT(block.hasSource(name));
        CPPUNIT_ASSERT(block.hasSource(src));

        ids.push_back(src.id());
    }
    CPPUNIT_ASSERT_THROW(block.createSource(names[0], "channel"),
                         DuplicateName);

    CPPUNIT_ASSERT(block.sourceCount() == names.size());
    CPPUNIT_ASSERT(block.sources().size() == names.size());


    for (const auto &id : ids) {
        Source src = block.getSource(id);
        CPPUNIT_ASSERT(block.hasSource(id) == true);
        CPPUNIT_ASSERT(src.id() == id);

        block.deleteSource(id);
    }

    CPPUNIT_ASSERT(block.sourceCount() == 0);
    CPPUNIT_ASSERT(block.sources().size() == 0);
    CPPUNIT_ASSERT(block.getSource("invalid_id") == false);
}


void TestBlock::testDataArrayAccess() {
    vector<string> names = { "data_array_a", "data_array_b", "data_array_c",
                             "data_array_d", "data_array_e" };

    CPPUNIT_ASSERT(block.dataArrayCount() == 0);
    CPPUNIT_ASSERT(block.dataArrays().size() == 0);
    CPPUNIT_ASSERT(block.getDataArray("invalid_id") == false);

    vector<string> ids;
    for (const auto &name : names) {
        DataArray data_array = block.createDataArray(name,
                                                     "channel",
                                                     DataType::Double,
                                                     nix::NDSize({ 0 }));
        CPPUNIT_ASSERT(data_array.name() == name);
        CPPUNIT_ASSERT(data_array.type() == "channel");

        ids.push_back(data_array.id());
    }
    CPPUNIT_ASSERT_THROW(block.createDataArray(names[0], "channel", DataType::Double, nix::NDSize({ 0 })),
                         DuplicateName);

    CPPUNIT_ASSERT(block.dataArrayCount() == names.size());
    CPPUNIT_ASSERT(block.dataArrays().size() == names.size());

    for (const auto &name : names) {
        DataArray da_name = block.getDataArray(name);
        CPPUNIT_ASSERT(da_name);

        DataArray da_id = block.getDataArray(da_name.id());
        CPPUNIT_ASSERT(da_id);
        CPPUNIT_ASSERT_EQUAL(da_name.name(), da_id.name());
    }

    vector<DataArray> filteredArrays = block.dataArrays(util::TypeFilter<DataArray>("channel"));
    CPPUNIT_ASSERT_EQUAL(names.size(), filteredArrays.size());

    filteredArrays = block.dataArrays(util::NameFilter<DataArray>("data_array_c"));
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), filteredArrays.size());
    if (filteredArrays.size() > 0) {
        boost::optional<std::string> name = filteredArrays[0].name();
        CPPUNIT_ASSERT(name && *name == "data_array_c");
    }

    for (auto it = ids.begin(); it != ids.end(); it++) {
        DataArray data_array = block.getDataArray(*it);
        CPPUNIT_ASSERT(block.hasDataArray(*it) == true);
        CPPUNIT_ASSERT(data_array.id() == *it);

        block.deleteDataArray(*it);
    }

    CPPUNIT_ASSERT(block.dataArrayCount() == 0);
    CPPUNIT_ASSERT(block.dataArrays().size() == 0);
    CPPUNIT_ASSERT(block.getDataArray("invalid_id") == false);

}


void TestBlock::testTagAccess() {
    vector<string> names = { "tag_a", "tag_b", "tag_c", "tag_d", "tag_e" };
    vector<string> array_names = { "data_array_a", "data_array_b", "data_array_c",
                                   "data_array_d", "data_array_e" };
    vector<DataArray> refs;
    for (const auto &name : array_names) {
        refs.push_back(block.createDataArray(name,
                                             "reference",
                                             DataType::Double,
                                             nix::NDSize({ 0 })));
    }

    CPPUNIT_ASSERT(block.tagCount() == 0);
    CPPUNIT_ASSERT(block.tags().size() == 0);
    CPPUNIT_ASSERT(block.getTag("invalid_id") == false);

    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); ++it) {
        Tag tag = block.createTag(*it, "segment", {0.0, 2.0, 3.4});
        tag.references(refs);
        CPPUNIT_ASSERT(tag.name() == *it);

        ids.push_back(tag.id());
    }
    CPPUNIT_ASSERT_THROW(block.createTag(names[0], "segment", {0.0, 2.0, 3.4}),
                         DuplicateName);

    CPPUNIT_ASSERT(block.tagCount() == names.size());
    CPPUNIT_ASSERT(block.tags().size() == names.size());

    for (auto it = ids.begin(); it != ids.end(); ++it) {
        Tag tag = block.getTag(*it);
        CPPUNIT_ASSERT(block.hasTag(*it) == true);
        CPPUNIT_ASSERT(tag.id() == *it);

        block.deleteTag(*it);
    }

    CPPUNIT_ASSERT(block.tagCount() == 0);
    CPPUNIT_ASSERT(block.tags().size() == 0);
    CPPUNIT_ASSERT(block.getTag("invalid_id") == false);
}


void TestBlock::testMultiTagAccess() {
    vector<string> names = { "tag_a", "tag_b", "tag_c", "tag_d", "tag_e" };
    // create a valid positions data array below
    typedef boost::multi_array<double, 3>::index index;
    DataArray positions = block.createDataArray("array_one",
                                                "testdata",
                                                DataType::Double,
                                                nix::NDSize({ 3, 4, 2 }));
    boost::multi_array<double, 3> A(boost::extents[3][4][2]);
    int values = 0;
    for(index i = 0; i != 3; ++i)
        for(index j = 0; j != 4; ++j)
            for(index k = 0; k != 2; ++k)
                A[i][j][k] = values++;

    positions.setData(A);

    CPPUNIT_ASSERT(block.multiTagCount() == 0);
    CPPUNIT_ASSERT(block.multiTags().size() == 0);
    CPPUNIT_ASSERT(block.getMultiTag("invalid_id") == false);

    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        MultiTag tag = block.createMultiTag(*it, "segment", positions);
        CPPUNIT_ASSERT(tag.name() == *it);

        ids.push_back(tag.id());
    }
    CPPUNIT_ASSERT_THROW(block.createMultiTag(names[0], "segment", positions),
                         DuplicateName);

    CPPUNIT_ASSERT(block.multiTagCount() == names.size());
    CPPUNIT_ASSERT(block.multiTags().size() == names.size());

    for (auto it = ids.begin(); it != ids.end(); it++) {
        MultiTag tag = block.getMultiTag(*it);
        CPPUNIT_ASSERT(block.hasMultiTag(*it) == true);
        CPPUNIT_ASSERT(tag.id() == *it);

        block.deleteMultiTag(*it);
    }

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
