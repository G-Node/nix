// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "BaseTestBlock.hpp"

#include <iterator>
#include <boost/math/constants/constants.hpp>

#include <nix/hydra/multiArray.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

using namespace nix;
using namespace valid;


void BaseTestBlock::testValidate() {
    valid::Result result = validate(block);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void BaseTestBlock::testId() {
    CPPUNIT_ASSERT(block.id().size() == 36);
}


void BaseTestBlock::testName() {
    CPPUNIT_ASSERT(block.name() == "block_one");
}


void BaseTestBlock::testType() {
    CPPUNIT_ASSERT(block.type() == "dataset");
    std::string typ = util::createId();
    block.type(typ);
    CPPUNIT_ASSERT(block.type() == typ);
}


void BaseTestBlock::testDefinition() {
    std::string def = util::createId();
    block.definition(def);
    CPPUNIT_ASSERT(*block.definition() == def);
}


void BaseTestBlock::testMetadataAccess() {
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


void BaseTestBlock::testSourceAccess() {
    std::vector<std::string> names = { "source_a", "source_b", "source_c", "source_d", "source_e" };
    Source s;
    CPPUNIT_ASSERT(!block.hasSource(s));
    CPPUNIT_ASSERT(block.sourceCount() == 0);
    CPPUNIT_ASSERT(block.sources().size() == 0);
    CPPUNIT_ASSERT(block.getSource("invalid_id") == false);
    CPPUNIT_ASSERT(!block.hasSource("invalid_id"));
    CPPUNIT_ASSERT_THROW(block.getSource(block.sourceCount() + 10), OutOfBounds);

    std::vector<std::string> ids;
    for (const auto &name : names) {
        Source src = block.createSource(name, "channel");
        CPPUNIT_ASSERT(src.name() == name);
        CPPUNIT_ASSERT(block.hasSource(name));
        CPPUNIT_ASSERT(block.hasSource(src));

        ids.push_back(src.id());
    }
    CPPUNIT_ASSERT_THROW(block.createSource(names[0], "channel"), DuplicateName);
    CPPUNIT_ASSERT_THROW(block.createSource("", "test"), nix::EmptyString);

    CPPUNIT_ASSERT(block.sourceCount() == names.size());
    CPPUNIT_ASSERT(block.sources().size() == names.size());
    CPPUNIT_ASSERT_THROW(block.getSource(block.sourceCount() + 10), OutOfBounds);

    for (const auto &id : ids) {
        Source src = block.getSource(id);
        CPPUNIT_ASSERT(block.hasSource(id) == true);
        CPPUNIT_ASSERT(src.id() == id);
        block.deleteSource(id);
    }

    s = block.createSource("test", "test");
    CPPUNIT_ASSERT(block.sourceCount() == 1);
    CPPUNIT_ASSERT_NO_THROW(block.deleteSource(s));

    CPPUNIT_ASSERT(block.sourceCount() == 0);
    CPPUNIT_ASSERT(block.sources().size() == 0);
    CPPUNIT_ASSERT(block.getSource("invalid_id") == false);
}


void BaseTestBlock::testDataArrayAccess() {
    std::vector<std::string> names = { "data_array_a", "data_array_b", "data_array_c",
                                       "data_array_d", "data_array_e" };
    DataArray data_array, a;

    CPPUNIT_ASSERT(block.dataArrayCount() == 0);
    CPPUNIT_ASSERT(block.dataArrays().size() == 0);
    CPPUNIT_ASSERT(block.getDataArray("invalid_id") == false);
    CPPUNIT_ASSERT_THROW(block.getDataArray(block.dataArrayCount() + 10), OutOfBounds);

    std::vector<std::string> ids;
    for (const auto &name : names) {
        data_array = block.createDataArray(name, "channel", DataType::Double, nix::NDSize({ 0 }));
        CPPUNIT_ASSERT(data_array.name() == name);
        CPPUNIT_ASSERT(data_array.type() == "channel");
        ids.push_back(data_array.id());
    }

    CPPUNIT_ASSERT_THROW(block.createDataArray(names[0], "channel", DataType::Double, nix::NDSize({ 0 })), DuplicateName);
    CPPUNIT_ASSERT_THROW(block.createDataArray("", "channel", DataType::Double, nix::NDSize({ 0 })), EmptyString);

    CPPUNIT_ASSERT(block.hasDataArray(data_array));
    CPPUNIT_ASSERT(!block.hasDataArray(a));
    CPPUNIT_ASSERT(block.dataArrayCount() == names.size());
    CPPUNIT_ASSERT(block.dataArrays().size() == names.size());
    CPPUNIT_ASSERT_THROW(block.getDataArray(block.dataArrayCount() + 10), OutOfBounds);

    for (const auto &name : names) {
        DataArray da_name = block.getDataArray(name);
        CPPUNIT_ASSERT(da_name);
        DataArray da_id = block.getDataArray(da_name.id());
        CPPUNIT_ASSERT(da_id);
        CPPUNIT_ASSERT_EQUAL(da_name.name(), da_id.name());
    }

    std::vector<DataArray> filteredArrays = block.dataArrays(util::TypeFilter<DataArray>("channel"));
    CPPUNIT_ASSERT_EQUAL(names.size(), filteredArrays.size());

    filteredArrays = block.dataArrays(util::NameFilter<DataArray>("data_array_c"));
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), filteredArrays.size());
    if (filteredArrays.size() > 0) {
        boost::optional<std::string> name = filteredArrays[0].name();
        CPPUNIT_ASSERT(name && *name == "data_array_c");
    }

    for (auto it = ids.begin(); it != ids.end(); it++) {
        CPPUNIT_ASSERT(block.hasDataArray(*it));
        CPPUNIT_ASSERT(block.getDataArray(*it).id() == *it);
        block.deleteDataArray(*it);
    }
    CPPUNIT_ASSERT(!block.deleteDataArray(a));
    CPPUNIT_ASSERT(block.dataArrayCount() == 0);
    CPPUNIT_ASSERT(block.dataArrays().size() == 0);
    CPPUNIT_ASSERT(block.getDataArray("invalid_id") == false);
}


void BaseTestBlock::testTagAccess() {
    std::vector<std::string> names = { "tag_a", "tag_b", "tag_c", "tag_d", "tag_e" };
    std::vector<std::string> array_names = { "data_array_a", "data_array_b", "data_array_c",
                                             "data_array_d", "data_array_e" };
    std::vector<DataArray> refs;
    Tag tag, t;
    for (const auto &name : array_names) {
        refs.push_back(block.createDataArray(name,
                                         "reference",
                                         DataType::Double,
                                         nix::NDSize({ 0 })));
    }

    CPPUNIT_ASSERT(block.tagCount() == 0);
    CPPUNIT_ASSERT(block.tags().size() == 0);
    CPPUNIT_ASSERT(block.getTag("invalid_id") == false);
    CPPUNIT_ASSERT(!block.hasTag(t));
    CPPUNIT_ASSERT_THROW(block.getTag(block.tagCount() + 10), OutOfBounds);

    std::vector<std::string> ids;
    for (auto it = names.begin(); it != names.end(); ++it) {
        tag = block.createTag(*it, "segment", {0.0, 2.0, 3.4});
        tag.references(refs);
        CPPUNIT_ASSERT(tag.name() == *it);
        CPPUNIT_ASSERT(block.hasTag(tag));
        ids.push_back(tag.id());
    }
    CPPUNIT_ASSERT_THROW(block.createTag(names[0], "segment", {0.0, 2.0, 3.4}),
                         DuplicateName);

    CPPUNIT_ASSERT(block.tagCount() == names.size());
    CPPUNIT_ASSERT(block.tags().size() == names.size());
    CPPUNIT_ASSERT_THROW(block.getTag(block.tagCount() + 10), OutOfBounds);

    for (auto it = ids.begin(); it != ids.end(); ++it) {
        tag = block.getTag(*it);
        CPPUNIT_ASSERT(block.hasTag(*it));
        CPPUNIT_ASSERT(tag.id() == *it);

        block.deleteTag(*it);
    }

    tag = block.createTag("test", "test", {0.0});
    CPPUNIT_ASSERT(block.hasTag(tag));
    CPPUNIT_ASSERT(block.deleteTag(tag));
    CPPUNIT_ASSERT(!block.deleteTag(t));

    CPPUNIT_ASSERT(block.tagCount() == 0);
    CPPUNIT_ASSERT(block.tags().size() == 0);
    CPPUNIT_ASSERT(block.getTag("invalid_id") == false);
}


void BaseTestBlock::testMultiTagAccess() {
    std::vector<std::string> names = { "tag_a", "tag_b", "tag_c", "tag_d", "tag_e" };
    MultiTag mtag, m;
    // create a valid positions data array below
    DataArray positions = block.createDataArray("array_one", "testdata",
                                                DataType::Double, nix::NDSize({ 3, 4, 2 }));

    CPPUNIT_ASSERT(block.multiTagCount() == 0);
    CPPUNIT_ASSERT(block.multiTags().size() == 0);
    CPPUNIT_ASSERT(block.getMultiTag("invalid_id") == false);
    CPPUNIT_ASSERT(!block.hasMultiTag(m));
    CPPUNIT_ASSERT_THROW(block.getMultiTag(block.multiTagCount() + 10), OutOfBounds);

    std::vector<std::string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        mtag = block.createMultiTag(*it, "segment", positions);
        CPPUNIT_ASSERT(mtag.name() == *it);
        CPPUNIT_ASSERT(block.hasMultiTag(mtag));
        ids.push_back(mtag.id());
    }
    CPPUNIT_ASSERT_THROW(block.createMultiTag(names[0], "segment", positions),
                         DuplicateName);

    CPPUNIT_ASSERT(block.multiTagCount() == names.size());
    CPPUNIT_ASSERT(block.multiTags().size() == names.size());
    CPPUNIT_ASSERT_THROW(block.getMultiTag(block.multiTagCount() + 10), OutOfBounds);

    for (auto it = ids.begin(); it != ids.end(); it++) {
        mtag = block.getMultiTag(*it);
        CPPUNIT_ASSERT(block.hasMultiTag(*it));
        CPPUNIT_ASSERT(mtag.id() == *it);

        block.deleteMultiTag(*it);
    }
    mtag = block.createMultiTag("test", "test", positions);
    CPPUNIT_ASSERT(block.hasMultiTag(mtag));
    CPPUNIT_ASSERT(!block.deleteMultiTag(m));
    CPPUNIT_ASSERT(block.deleteMultiTag(mtag));
    CPPUNIT_ASSERT(block.multiTagCount() == 0);
    CPPUNIT_ASSERT(block.multiTags().size() == 0);
    CPPUNIT_ASSERT(block.getMultiTag("invalid_id") == false);
}


void BaseTestBlock::testGroupAccess() {
    std::vector<std::string> names = { "group_a", "group_b", "group_c", "group_d", "group_e" };
    Group g;
    CPPUNIT_ASSERT(!block.hasGroup(g));
    CPPUNIT_ASSERT(block.groupCount() == 0);
    CPPUNIT_ASSERT(block.groups().size() == 0);
    CPPUNIT_ASSERT(block.getGroup("invalid_id") == false);
    CPPUNIT_ASSERT(!block.hasGroup("invalid_id"));
    CPPUNIT_ASSERT_THROW(block.getGroup(block.groupCount() + 10), OutOfBounds);

    std::vector<std::string> ids;
    for (const auto &name : names) {
        Group gr = block.createGroup(name, "group");
        CPPUNIT_ASSERT(gr.name() == name);
        CPPUNIT_ASSERT(block.hasGroup(name));
        CPPUNIT_ASSERT(block.hasGroup(gr));

        ids.push_back(gr.id());
    }
    CPPUNIT_ASSERT_THROW(block.createGroup(names[0], "group"),
                         DuplicateName);

    CPPUNIT_ASSERT(block.groupCount() == names.size());
    CPPUNIT_ASSERT(block.groups().size() == names.size());
    CPPUNIT_ASSERT_THROW(block.getGroup(block.groupCount() + 10), OutOfBounds);

    for (const auto &id : ids) {
        Group gr = block.getGroup(id);
        CPPUNIT_ASSERT(block.hasGroup(id));
        CPPUNIT_ASSERT(gr.id() == id);
        block.deleteGroup(id);
    }

    g = block.createGroup("test", "test");
    CPPUNIT_ASSERT(block.groupCount() == 1);
    CPPUNIT_ASSERT(block.deleteGroup(g));

    CPPUNIT_ASSERT(block.groupCount() == 0);
    CPPUNIT_ASSERT(block.groups().size() == 0);
    CPPUNIT_ASSERT(block.getGroup("invalid_id") == false);
}


void BaseTestBlock::testOperators() {
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

    std::stringstream str;
    str << block;
}


void BaseTestBlock::testCreatedAt() {
    CPPUNIT_ASSERT(block.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    block.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(block.createdAt() == past_time);
}


void BaseTestBlock::testUpdatedAt() {
    CPPUNIT_ASSERT(block.updatedAt() >= startup_time);
}


void BaseTestBlock::testCompare() {
    std::string other_name = block_other.name();
    std::string block_name = block.name();

    CPPUNIT_ASSERT(block.compare(block) == 0);
    CPPUNIT_ASSERT(block.compare(block_other) ==  block_name.compare(other_name));
}
