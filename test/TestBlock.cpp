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

using namespace std;
using namespace nix;


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
    CPPUNIT_ASSERT(block.definition() == def);
}


void TestBlock::testMetadataAccess() {
    CPPUNIT_ASSERT(!block.hasMetadata());

    block.metadata(section);
    CPPUNIT_ASSERT(block.hasMetadata());
    // TODO This test fails due to operator== of Section
    // CPPUNIT_ASSERT(block.metadata() == section);

    block.removeMetadata();
    CPPUNIT_ASSERT(!block.hasMetadata());
}


void TestBlock::testSourceAccess() {
    vector<string> names = { "source_a", "source_b", "source_c", "source_d", "source_e" };

    CPPUNIT_ASSERT(block.sourceCount() == 0);
    CPPUNIT_ASSERT(block.sources().size() == 0);

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

        block.removeSource(*it);
    }

    CPPUNIT_ASSERT(block.sourceCount() == 0);
    CPPUNIT_ASSERT(block.sources().size() == 0);
}


void TestBlock::testDataArrayAccess() {
    vector<string> names = { "data_array_a", "data_array_b", "data_array_c",
                             "data_array_d", "data_array_e" };

    CPPUNIT_ASSERT(block.dataArrayCount() == 0);
    CPPUNIT_ASSERT(block.dataArrays().size() == 0);


    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        DataArray data_array = block.createDataArray(*it, "channel");
        CPPUNIT_ASSERT(data_array.name() == *it);

        ids.push_back(data_array.id());
    }


    CPPUNIT_ASSERT(block.dataArrayCount() == names.size());
    CPPUNIT_ASSERT(block.dataArrays().size() == names.size());


    for (auto it = ids.begin(); it != ids.end(); it++) {
        DataArray data_array = block.getDataArray(*it);
        CPPUNIT_ASSERT(block.hasDataArray(*it) == true);
        CPPUNIT_ASSERT(data_array.id() == *it);

        block.removeDataArray(*it);
    }

    CPPUNIT_ASSERT(block.dataArrayCount() == 0);
    CPPUNIT_ASSERT(block.dataArrays().size() == 0);
}


void TestBlock::testSimpleTagAccess() {
    vector<string> names = { "tag_a", "tag_b", "tag_c", "tag_d", "tag_e" };

    CPPUNIT_ASSERT(block.simpleTagCount() == 0);
    CPPUNIT_ASSERT(block.simpleTags().size() == 0);

    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        SimpleTag tag = block.createSimpleTag(*it, "segment");
        CPPUNIT_ASSERT(tag.name() == *it);

        ids.push_back(tag.id());
    }


    CPPUNIT_ASSERT(block.simpleTagCount() == names.size());
    CPPUNIT_ASSERT(block.simpleTags().size() == names.size());


    for (auto it = ids.begin(); it != ids.end(); it++) {
        SimpleTag tag = block.getSimpleTag(*it);
        CPPUNIT_ASSERT(block.hasSimpleTag(*it) == true);
        CPPUNIT_ASSERT(tag.id() == *it);

        block.removeSimpleTag(*it);
    }

    CPPUNIT_ASSERT(block.simpleTagCount() == 0);
    CPPUNIT_ASSERT(block.simpleTags().size() == 0);
}


void TestBlock::testDataTagAccess() {
    vector<string> names = { "tag_a", "tag_b", "tag_c", "tag_d", "tag_e" };

    CPPUNIT_ASSERT(block.dataTagCount() == 0);
    CPPUNIT_ASSERT(block.dataTags().size() == 0);

    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        DataTag tag = block.createDataTag(*it, "segment");
        CPPUNIT_ASSERT(tag.name() == *it);

        ids.push_back(tag.id());
    }

    CPPUNIT_ASSERT(block.dataTagCount() == names.size());
    CPPUNIT_ASSERT(block.dataTags().size() == names.size());

    for (auto it = ids.begin(); it != ids.end(); it++) {
        DataTag tag = block.getDataTag(*it);
        CPPUNIT_ASSERT(block.hasDataTag(*it) == true);
        CPPUNIT_ASSERT(tag.id() == *it);

        block.removeDataTag(*it);
    }

    CPPUNIT_ASSERT(block.dataTagCount() == 0);
    CPPUNIT_ASSERT(block.dataTags().size() == 0);
}


void TestBlock::testOperators() {
    CPPUNIT_ASSERT(block_null == NULL);
    CPPUNIT_ASSERT(block_null == nullptr);

    CPPUNIT_ASSERT(block != NULL);
    CPPUNIT_ASSERT(block != nullptr);

    CPPUNIT_ASSERT(block == block);
    CPPUNIT_ASSERT(block != block_other);

    block_other = block;
    CPPUNIT_ASSERT(block == block_other);

    block_other = nullptr;
    CPPUNIT_ASSERT(block_null == NULL);
    CPPUNIT_ASSERT(block_null == nullptr);
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
