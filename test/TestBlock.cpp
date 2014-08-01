// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include <nix/util/util.hpp>
#include "TestBlock.hpp"

#include <nix/valid/validate.hpp>

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
    CPPUNIT_ASSERT(block.id().size() == 22);
}


void TestBlock::testName() {
    CPPUNIT_ASSERT(block.name() == "block_one");
    string name = util::createId("", 32);
    block.name(name);
    CPPUNIT_ASSERT(block.name() == name);
}


void TestBlock::testType() {
    CPPUNIT_ASSERT(block.type() == "dataset");
    string typ = util::createId("", 32);
    block.type(typ);
    CPPUNIT_ASSERT(block.type() == typ);
}


void TestBlock::testDefinition() {
    string def = util::createId("", 128);
    block.definition(def);
    CPPUNIT_ASSERT(*block.definition() == def);
}


void TestBlock::testMetadataAccess() {
    CPPUNIT_ASSERT(!block.metadata());

    block.metadata(section);
    CPPUNIT_ASSERT(block.metadata());

    block.metadata(none);
    CPPUNIT_ASSERT(!block.metadata());
}


void TestBlock::testSourceAccess() {
    vector<string> names = { "source_a", "source_b", "source_c", "source_d", "source_e" };

    CPPUNIT_ASSERT(block.sourceCount() == 0);
    CPPUNIT_ASSERT(block.sources().size() == 0);
    CPPUNIT_ASSERT(block.getSource("invalid_id") == false);

    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        Source src = block.createSource(*it, "channel");
        CPPUNIT_ASSERT(src.name() == *it);

        ids.push_back(src.id());
    }


    CPPUNIT_ASSERT(block.sourceCount() == names.size());
    CPPUNIT_ASSERT(block.sources().size() == names.size());


    for (auto it = ids.begin(); it != ids.end(); it++) {
        Source src = block.getSource(*it);
        CPPUNIT_ASSERT(block.hasSource(*it) == true);
        CPPUNIT_ASSERT(src.id() == *it);

        block.deleteSource(*it);
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
                                                     {0});
        CPPUNIT_ASSERT(data_array.name() == name);
        CPPUNIT_ASSERT(data_array.type() == "channel");

        ids.push_back(data_array.id());
    }

    CPPUNIT_ASSERT(block.dataArrayCount() == names.size());
    CPPUNIT_ASSERT(block.dataArrays().size() == names.size());

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


void TestBlock::testSimpleTagAccess() {
    vector<string> names = { "tag_a", "tag_b", "tag_c", "tag_d", "tag_e" };
    vector<string> array_names = { "data_array_a", "data_array_b", "data_array_c",
                                   "data_array_d", "data_array_e" };
    vector<DataArray> refs;
    for (const auto &name : array_names) {
        refs.push_back(block.createDataArray(name,
                                             "reference",
                                             DataType::Double,
                                             {0}));
    }

    CPPUNIT_ASSERT(block.simpleTagCount() == 0);
    CPPUNIT_ASSERT(block.simpleTags().size() == 0);
    CPPUNIT_ASSERT(block.getSimpleTag("invalid_id") == false);

    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); ++it) {
        SimpleTag tag = block.createSimpleTag(*it, "segment", {0.0, 2.0, 3.4});
	tag.references(refs);
        CPPUNIT_ASSERT(tag.name() == *it);

        ids.push_back(tag.id());
    }

    CPPUNIT_ASSERT(block.simpleTagCount() == names.size());
    CPPUNIT_ASSERT(block.simpleTags().size() == names.size());

    for (auto it = ids.begin(); it != ids.end(); ++it) {
        SimpleTag tag = block.getSimpleTag(*it);
        CPPUNIT_ASSERT(block.hasSimpleTag(*it) == true);
        CPPUNIT_ASSERT(tag.id() == *it);

        block.deleteSimpleTag(*it);
    }

    CPPUNIT_ASSERT(block.simpleTagCount() == 0);
    CPPUNIT_ASSERT(block.simpleTags().size() == 0);
    CPPUNIT_ASSERT(block.getSimpleTag("invalid_id") == false);
}


void TestBlock::testDataTagAccess() {
    vector<string> names = { "tag_a", "tag_b", "tag_c", "tag_d", "tag_e" };
    // create a valid positions data array below
    typedef boost::multi_array<double, 3>::index index;
    DataArray positions = block.createDataArray("array_one",
                                                "testdata",
                                                DataType::Double,
                                                {3, 4, 2});
    boost::multi_array<double, 3> A(boost::extents[3][4][2]);
    int values = 0;
    for(index i = 0; i != 3; ++i)
        for(index j = 0; j != 4; ++j)
            for(index k = 0; k != 2; ++k)
                A[i][j][k] = values++;

    positions.setData(A);

    CPPUNIT_ASSERT(block.dataTagCount() == 0);
    CPPUNIT_ASSERT(block.dataTags().size() == 0);
    CPPUNIT_ASSERT(block.getDataTag("invalid_id") == false);
    
    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        DataTag tag = block.createDataTag(*it, "segment", positions);
        CPPUNIT_ASSERT(tag.name() == *it);

        ids.push_back(tag.id());
    }

    CPPUNIT_ASSERT(block.dataTagCount() == names.size());
    CPPUNIT_ASSERT(block.dataTags().size() == names.size());

    for (auto it = ids.begin(); it != ids.end(); it++) {
        DataTag tag = block.getDataTag(*it);
        CPPUNIT_ASSERT(block.hasDataTag(*it) == true);
        CPPUNIT_ASSERT(tag.id() == *it);

        block.deleteDataTag(*it);
    }

    CPPUNIT_ASSERT(block.dataTagCount() == 0);
    CPPUNIT_ASSERT(block.dataTags().size() == 0);
    CPPUNIT_ASSERT(block.getDataTag("invalid_id") == false);
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
