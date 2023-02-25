// Copyright © 2015 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>

#include "BaseTestGroup.hpp"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

using namespace nix;
using namespace valid;


void BaseTestGroup::testId() {
    CPPUNIT_ASSERT(g.id().size() == 36);
}


void BaseTestGroup::testName() {
    CPPUNIT_ASSERT(g.name() == "group one");
}


void BaseTestGroup::testDefinition() {
    std::string def = nix::util::createId();
    g.definition(def);
    CPPUNIT_ASSERT(*g.definition() == def);
    g.definition(nix::none);
    CPPUNIT_ASSERT(g.definition() == nix::none);
}


void BaseTestGroup::testType() {
    CPPUNIT_ASSERT(g.type() == "group");
    std::string typ = nix::util::createId();
    g.type(typ);
    CPPUNIT_ASSERT(g.type() == typ);
}


void BaseTestGroup::testOperators() {
    std::stringstream mystream;
    mystream << g;

    g = none;
    CPPUNIT_ASSERT(g == false);
    CPPUNIT_ASSERT(g == none);
}


void BaseTestGroup::testDataArrays() {
    DataArray da_1 = block.createDataArray("TestArray 1","array",
                                           DataType::Double, nix::NDSize({ 0 }));
    DataArray da_2 = block.createDataArray("TestArray 2","array",
                                           DataType::Double, nix::NDSize({ 0 }));
    DataArray da_3 = block.createDataArray("TestArray 3","array",
                                           DataType::Double, nix::NDSize({ 0 }));

    Block b = file.createBlock("another block", "test");
    DataArray da = b.createDataArray("TestArray 2","array",
                                     DataType::Double, nix::NDSize({ 0 }));

    Group g = block.createGroup("test group", "group");
    CPPUNIT_ASSERT_THROW(g.getDataArray(42), nix::OutOfBounds);
    CPPUNIT_ASSERT(g.dataArrayCount() == 0);

    g.dataArrays(arrays);
    CPPUNIT_ASSERT(g.dataArrayCount() == arrays.size());
    g.addDataArray(da_1);
    g.addDataArray(da_2.name());
    g.addDataArray(da_3.id());
    CPPUNIT_ASSERT_THROW(g.addDataArray(da), std::runtime_error);
    CPPUNIT_ASSERT(g.dataArrayCount() == arrays.size() + 3);
    DataArray a1 = g.getDataArray(da_1.id());

    CPPUNIT_ASSERT(a1.id() == da_1.id());
    DataArray a2 = g.getDataArray(da_1.name());
    CPPUNIT_ASSERT(a2.id() == da_1.id());
    std::vector<DataArray> all_arrays = g.dataArrays();
    CPPUNIT_ASSERT(all_arrays.size() == arrays.size() + 3);
    CPPUNIT_ASSERT(g.hasDataArray(da_1.id()));
    CPPUNIT_ASSERT(g.hasDataArray(da_1));
    CPPUNIT_ASSERT(g.hasDataArray(da_2.id()));
    CPPUNIT_ASSERT(g.hasDataArray(da_2));
    CPPUNIT_ASSERT(g.hasDataArray(da_3.name()));

    g.removeDataArray(da_1);
    CPPUNIT_ASSERT(g.dataArrayCount() == arrays.size() + 2);
    g.removeDataArray(da_2.id());
    CPPUNIT_ASSERT(g.dataArrayCount() == arrays.size() + 1);
    g.removeDataArray(da_3.name());
    CPPUNIT_ASSERT(g.dataArrayCount() == arrays.size());

    // delete data arrays
    std::vector<std::string> ids;
    block.deleteDataArray(da_1);
    block.deleteDataArray(da_2.id());
    block.deleteDataArray(da_3.name());

    for (auto it = arrays.begin(); it != arrays.end(); it++) {
        ids.push_back((*it).id());
        block.deleteDataArray((*it).id());
    }
    // check if references are gone too!
    CPPUNIT_ASSERT(g.dataArrayCount() == 0);
    for (auto ref_id : ids) {
        CPPUNIT_ASSERT(!g.hasDataArray(ref_id));
    }

    DataArray a;
    CPPUNIT_ASSERT(!g.hasDataArray(a));
    CPPUNIT_ASSERT(!g.removeDataArray(a));
    a = block.createDataArray("Test", "test", DataType::Double, {0,0});
    block.deleteDataArray(a);
    CPPUNIT_ASSERT_THROW(g.addDataArray(a), UninitializedEntity);
    block.deleteGroup(g.id());
    file.deleteBlock(b);
}

void BaseTestGroup::testDataFrames() {
    Group g = block.createGroup("test frame group", "group");

    DataFrame df;

    CPPUNIT_ASSERT(!g.hasDataFrame(df));
    CPPUNIT_ASSERT(g.dataFrameCount() == 0);
    CPPUNIT_ASSERT(g.dataFrames().size() == 0);
    CPPUNIT_ASSERT(g.getDataFrame("invalid_id") == false);
    CPPUNIT_ASSERT(!g.hasDataFrame("invalid_id"));
    CPPUNIT_ASSERT_THROW(g.getDataFrame(g.dataFrameCount() + 10), OutOfBounds);

    const std::vector<nix::Column> cols = {
        {"int32", "V", nix::DataType::Int32},
        {"string", "", nix::DataType::String},
        {"double", "mV", nix::DataType::Double}};

    std::vector<std::string> names = { "a", "b", "c", "d", "e" };
    std::vector<DataFrame> dfs;

    std::transform(names.cbegin(), names.cend(), std::back_inserter(dfs),
                   [this, &g, &cols](const std::string &name) {

                       DataFrame d = block.createDataFrame(name, "df", cols);

                       //via the object
                       g.addDataFrame(d);

                       CPPUNIT_ASSERT(g.hasDataFrame(d.id()));
                       CPPUNIT_ASSERT(g.hasDataFrame(name));
                       CPPUNIT_ASSERT(g.hasDataFrame(d));

                       g.removeDataFrame(d);

                       CPPUNIT_ASSERT(!g.hasDataFrame(d.id()));
                       CPPUNIT_ASSERT(!g.hasDataFrame(name));
                       CPPUNIT_ASSERT(!g.hasDataFrame(d));

                       //via the id
                       g.addDataFrame(d.id());

                       CPPUNIT_ASSERT(g.hasDataFrame(d.id()));
                       CPPUNIT_ASSERT(g.hasDataFrame(name));
                       CPPUNIT_ASSERT(g.hasDataFrame(d));

                       g.removeDataFrame(d.id());

                       CPPUNIT_ASSERT(!g.hasDataFrame(d.id()));
                       CPPUNIT_ASSERT(!g.hasDataFrame(name));
                       CPPUNIT_ASSERT(!g.hasDataFrame(d));

                       //via the ma,e
                       g.addDataFrame(name);

                       CPPUNIT_ASSERT(g.hasDataFrame(d.id()));
                       CPPUNIT_ASSERT(g.hasDataFrame(name));
                       CPPUNIT_ASSERT(g.hasDataFrame(d));

                       g.removeDataFrame(name);

                       CPPUNIT_ASSERT(!g.hasDataFrame(d.id()));
                       CPPUNIT_ASSERT(!g.hasDataFrame(name));
                       CPPUNIT_ASSERT(!g.hasDataFrame(d));

                       return d;
                   });

    CPPUNIT_ASSERT_EQUAL(ndsize_t(0), g.dataFrameCount());
    g.dataFrames(dfs);
    CPPUNIT_ASSERT_EQUAL(ndsize_t(dfs.size()), g.dataFrameCount());

    std::vector<DataFrame> filtered = g.dataFrames(util::TypeFilter<DataFrame>("df"));
    CPPUNIT_ASSERT_EQUAL(dfs.size(), filtered.size());

    filtered = g.dataFrames(util::TypeFilter<DataFrame>("xx"));
    CPPUNIT_ASSERT_EQUAL(size_t(0), filtered.size());

    filtered = g.dataFrames(util::NameFilter<DataFrame>("c"));
    CPPUNIT_ASSERT_EQUAL(size_t(1), filtered.size());

    boost::optional<std::string> name = filtered.at(0).name();
    CPPUNIT_ASSERT(name && *name == "c");

    for (const auto &frame : dfs) {
        const std::string &id = frame.id();

        CPPUNIT_ASSERT(g.hasDataFrame(frame));
        CPPUNIT_ASSERT(g.hasDataFrame(id));

        CPPUNIT_ASSERT_EQUAL(id, g.getDataFrame(id).id());
    }

    /* get rid of them again */
    for (const auto &frame : dfs) {
        g.removeDataFrame(frame);
        block.deleteDataFrame(frame);
    }

    CPPUNIT_ASSERT(!g.removeDataFrame(df));
    CPPUNIT_ASSERT_EQUAL(ndsize_t(0), g.dataFrameCount());
    CPPUNIT_ASSERT_EQUAL(size_t(0), g.dataFrames().size());
    CPPUNIT_ASSERT(g.getDataFrame(names[0]) == false);

}

void BaseTestGroup::testTags() {
    Tag tag_1 = block.createTag("TestTag 1","tag", std::vector<double>{0.0});
    Tag tag_2 = block.createTag("TestTag 2","tag", std::vector<double>{1.0});
    Tag tag_3 = block.createTag("TestTag 3","tag", std::vector<double>{1.0});

    Block b = file.createBlock("another block", "test");
    Tag t = b.createTag("far away tag", "tag", std::vector<double>{1.0});

    Group g = block.createGroup("test group", "group");
    CPPUNIT_ASSERT_THROW(g.getTag(42), nix::OutOfBounds);
    CPPUNIT_ASSERT(g.tagCount() == 0);

    g.tags(tags);
    CPPUNIT_ASSERT(g.tagCount() == tags.size());
    g.addTag(tag_1);
    g.addTag(tag_2.id());
    g.addTag(tag_3.name());
    CPPUNIT_ASSERT_THROW(g.addTag(t), std::runtime_error);
    CPPUNIT_ASSERT(g.tagCount() == tags.size() + 3);

    Tag t1 = g.getTag(tag_1.id());
    CPPUNIT_ASSERT(t1.id() == tag_1.id());
    Tag t2 = g.getTag(tag_1.name());
    CPPUNIT_ASSERT(t2.id() == tag_1.id());
    std::vector<Tag> all_tags = g.tags();
    CPPUNIT_ASSERT(all_tags.size() == tags.size() + 3);
    CPPUNIT_ASSERT(g.hasTag(tag_1.id()));
    CPPUNIT_ASSERT(g.hasTag(tag_1));
    CPPUNIT_ASSERT(g.hasTag(tag_2.id()));
    CPPUNIT_ASSERT(g.hasTag(tag_2));
    CPPUNIT_ASSERT(g.hasTag(tag_3.name()));

    g.removeTag(tag_1);
    CPPUNIT_ASSERT(g.tagCount() == tags.size() + 2);
    g.removeTag(tag_2.id());
    CPPUNIT_ASSERT(g.tagCount() == tags.size() + 1);
    g.removeTag(tag_3.name());
    CPPUNIT_ASSERT(g.tagCount() == tags.size() + 0);

    // delete tags
    std::vector<std::string> ids;
    block.deleteTag(tag_1.id());
    block.deleteTag(tag_2.id());
    for (auto it = tags.begin(); it != tags.end(); it++) {
        ids.push_back((*it).id());
        block.deleteTag((*it).id());
    }
    // check if references are gone too!
    CPPUNIT_ASSERT(g.tagCount() == 0);
    for (auto ref_id : ids) {
        CPPUNIT_ASSERT(!g.hasTag(ref_id));
    }

    Tag tt;
    CPPUNIT_ASSERT(!g.removeTag(tt));
    CPPUNIT_ASSERT(!g.hasTag(tt));
    CPPUNIT_ASSERT_THROW(g.addTag(tt), UninitializedEntity);

    block.deleteGroup(g.id());
    file.deleteBlock(b);
}


void BaseTestGroup::testMultiTags() {
    MultiTag tag_1 = block.createMultiTag("TestTag 1","tag", positions_array);
    MultiTag tag_2 = block.createMultiTag("TestTag 2","tag", positions_array);
    MultiTag tag_3 = block.createMultiTag("TestTag 3","tag", positions_array);

    Block b = file.createBlock("another block", "test");
    DataArray a = b.createDataArray("positions", "nix.events", nix::DataType::Double, nix::NDSize{0.0});
    MultiTag t = b.createMultiTag("far away tag", "tag", a);

    Group g = block.createGroup("test group", "group");
    CPPUNIT_ASSERT_THROW(g.getMultiTag(42), nix::OutOfBounds);
    CPPUNIT_ASSERT(g.multiTagCount() == 0);

    g.multiTags(mtags);
    CPPUNIT_ASSERT(g.multiTagCount() == mtags.size());
    g.addMultiTag(tag_1);
    g.addMultiTag(tag_2.id());
    g.addMultiTag(tag_3.name());
    CPPUNIT_ASSERT_THROW(g.addMultiTag(t), std::runtime_error);
    CPPUNIT_ASSERT(g.multiTagCount() == mtags.size() + 3);

    MultiTag t1 = g.getMultiTag(tag_1.id());
    CPPUNIT_ASSERT(t1.id() == tag_1.id());
    MultiTag t2 = g.getMultiTag(tag_1.name());
    CPPUNIT_ASSERT(t2.id() == tag_1.id());
    std::vector<MultiTag> all_tags = g.multiTags();
    CPPUNIT_ASSERT(all_tags.size() == mtags.size() + 3);
    CPPUNIT_ASSERT(g.hasMultiTag(tag_1.id()));
    CPPUNIT_ASSERT(g.hasMultiTag(tag_1));
    CPPUNIT_ASSERT(g.hasMultiTag(tag_2.id()));
    CPPUNIT_ASSERT(g.hasMultiTag(tag_2.name()));

    g.removeMultiTag(tag_1);
    CPPUNIT_ASSERT(g.multiTagCount() == mtags.size() + 2);
    g.removeMultiTag(tag_2.id());
    CPPUNIT_ASSERT(g.multiTagCount() == mtags.size() + 1);
    g.removeMultiTag(tag_3.name());
    CPPUNIT_ASSERT(g.multiTagCount() == mtags.size());

    // delete tags
    std::vector<std::string> ids;
    block.deleteMultiTag(tag_1);
    block.deleteMultiTag(tag_2.id());
    block.deleteMultiTag(tag_3.name());
    for (auto it = mtags.begin(); it != mtags.end(); it++) {
        ids.push_back((*it).id());
        block.deleteMultiTag((*it).id());
    }
    // check if references are gone too!
    CPPUNIT_ASSERT(g.multiTagCount() == 0);
    for (auto ref_id : ids) {
        CPPUNIT_ASSERT(!g.hasMultiTag(ref_id));
    }

    MultiTag mt;
    CPPUNIT_ASSERT_THROW(g.addMultiTag(mt), UninitializedEntity);
    CPPUNIT_ASSERT(!g.hasMultiTag(mt));
    CPPUNIT_ASSERT(!g.removeMultiTag(mt));

    block.deleteGroup(g.id());
    file.deleteBlock(b);
}
