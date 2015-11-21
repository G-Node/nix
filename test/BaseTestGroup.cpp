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
    g.definition(boost::none);
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

    Group g = block.createGroup("test group", "group");
    CPPUNIT_ASSERT_THROW(g.getDataArray(42), nix::OutOfBounds);
    CPPUNIT_ASSERT(g.dataArrayCount() == 0);

    g.dataArrays(arrays);
    CPPUNIT_ASSERT(g.dataArrayCount() == arrays.size());
    g.addDataArray(da_1);
    g.addDataArray(da_2);
    CPPUNIT_ASSERT(g.dataArrayCount() == arrays.size() + 2);
    DataArray a1 = g.getDataArray(da_1.id());

    CPPUNIT_ASSERT(a1.id() == da_1.id());
    DataArray a2 = g.getDataArray(da_1.name());
    CPPUNIT_ASSERT(a2.id() == da_1.id());
    std::vector<DataArray> all_arrays = g.dataArrays();
    CPPUNIT_ASSERT(all_arrays.size() == arrays.size() + 2);
    CPPUNIT_ASSERT(g.hasDataArray(da_1.id()));
    CPPUNIT_ASSERT(g.hasDataArray(da_2.id()));

    g.removeDataArray(da_1.id());
    CPPUNIT_ASSERT(g.dataArrayCount() == arrays.size() + 1);
    g.removeDataArray(da_2.id());
    CPPUNIT_ASSERT(g.dataArrayCount() == arrays.size());

    // delete data arrays
    std::vector<std::string> ids;
    block.deleteDataArray(da_1.id());
    block.deleteDataArray(da_2.id());
    for (auto it = arrays.begin(); it != arrays.end(); it++) {
        ids.push_back((*it).id());
        block.deleteDataArray((*it).id());
    }
    // check if references are gone too!
    CPPUNIT_ASSERT(g.dataArrayCount() == 0);
    for (auto ref_id : ids) {
        CPPUNIT_ASSERT(!g.hasDataArray(ref_id));
    }
    block.deleteGroup(g.id());
}


void BaseTestGroup::testTags() {
    Tag tag_1 = block.createTag("TestTag 1","tag", std::vector<double>{0.0});
    Tag tag_2 = block.createTag("TestTag 2","tag", std::vector<double>{1.0});

    Group g = block.createGroup("test group", "group");
    CPPUNIT_ASSERT_THROW(g.getTag(42), nix::OutOfBounds);
    CPPUNIT_ASSERT(g.tagCount() == 0);

    g.tags(tags);
    CPPUNIT_ASSERT(g.tagCount() == tags.size());
    g.addTag(tag_1);
    g.addTag(tag_2);
    CPPUNIT_ASSERT(g.tagCount() == tags.size() + 2);

    Tag t1 = g.getTag(tag_1.id());
    CPPUNIT_ASSERT(t1.id() == tag_1.id());
    Tag t2 = g.getTag(tag_1.name());
    CPPUNIT_ASSERT(t2.id() == tag_1.id());
    std::vector<Tag> all_tags = g.tags();
    CPPUNIT_ASSERT(all_tags.size() == tags.size() + 2);
    CPPUNIT_ASSERT(g.hasTag(tag_1.id()));
    CPPUNIT_ASSERT(g.hasTag(tag_2.id()));

    g.removeTag(tag_1.id());
    CPPUNIT_ASSERT(g.tagCount() == tags.size() + 1);
    g.removeTag(tag_2.id());
    CPPUNIT_ASSERT(g.tagCount() == tags.size());

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
    block.deleteGroup(g.id());
}


void BaseTestGroup::testMultiTags() {
    MultiTag tag_1 = block.createMultiTag("TestTag 1","tag", positions_array);
    MultiTag tag_2 = block.createMultiTag("TestTag 2","tag", positions_array);

    Group g = block.createGroup("test group", "group");
    CPPUNIT_ASSERT_THROW(g.getMultiTag(42), nix::OutOfBounds);
    CPPUNIT_ASSERT(g.multiTagCount() == 0);

    g.multiTags(mtags);
    CPPUNIT_ASSERT(g.multiTagCount() == mtags.size());
    g.addMultiTag(tag_1);
    g.addMultiTag(tag_2);
    CPPUNIT_ASSERT(g.multiTagCount() == mtags.size() + 2);

    MultiTag t1 = g.getMultiTag(tag_1.id());
    CPPUNIT_ASSERT(t1.id() == tag_1.id());
    MultiTag t2 = g.getMultiTag(tag_1.name());
    CPPUNIT_ASSERT(t2.id() == tag_1.id());
    std::vector<MultiTag> all_tags = g.multiTags();
    CPPUNIT_ASSERT(all_tags.size() == mtags.size() + 2);
    CPPUNIT_ASSERT(g.hasMultiTag(tag_1.id()));
    CPPUNIT_ASSERT(g.hasMultiTag(tag_2.id()));

    g.removeMultiTag(tag_1.id());
    CPPUNIT_ASSERT(g.multiTagCount() == mtags.size() + 1);
    g.removeMultiTag(tag_2.id());
    CPPUNIT_ASSERT(g.multiTagCount() == mtags.size());

    // delete tags
    std::vector<std::string> ids;
    block.deleteMultiTag(tag_1.id());
    block.deleteMultiTag(tag_2.id());
    for (auto it = mtags.begin(); it != mtags.end(); it++) {
        ids.push_back((*it).id());
        block.deleteMultiTag((*it).id());
    }
    // check if references are gone too!
    CPPUNIT_ASSERT(g.multiTagCount() == 0);
    for (auto ref_id : ids) {
        CPPUNIT_ASSERT(!g.hasMultiTag(ref_id));
    }
    block.deleteGroup(g.id());
}