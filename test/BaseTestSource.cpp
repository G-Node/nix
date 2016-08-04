// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>
#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>

#include "BaseTestSource.hpp"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

using namespace nix;
using namespace valid;


void BaseTestSource::testValidate() {
    valid::Result result = validate(source);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void BaseTestSource::testId() {
    CPPUNIT_ASSERT(source.id().size() == 36);
}


void BaseTestSource::testName() {
    CPPUNIT_ASSERT(source.name() == "source_one");
}


void BaseTestSource::testType() {
    CPPUNIT_ASSERT(source.type() == "channel");
    std::string typ = util::createId();
    source.type(typ);
    CPPUNIT_ASSERT(source.type() == typ);
}


void BaseTestSource::testDefinition() {
    std::string def = util::createId();
    source.definition(def);
    CPPUNIT_ASSERT(*source.definition() == def);
    source.definition(nix::none);
    CPPUNIT_ASSERT(source.definition() == nix::none);
}


void BaseTestSource::testMetadataAccess() {
    CPPUNIT_ASSERT(!source.metadata());

    source.metadata(section);
    CPPUNIT_ASSERT(source.metadata());

    // test none-unsetter
    source.metadata(none);
    CPPUNIT_ASSERT(!source.metadata());
    // test deleter removing link too
    source.metadata(section);
    file.deleteSection(section.id());
    CPPUNIT_ASSERT(!source.metadata());
    // re-create section
    section = file.createSection("foo_section", "metadata");
}


void BaseTestSource::testSourceAccess() {
    std::vector<std::string> names = { "source_a", "source_b", "source_c", "source_d", "source_e" };

    CPPUNIT_ASSERT(source.sourceCount() == 0);
    CPPUNIT_ASSERT(source.sources().size() == 0);
    CPPUNIT_ASSERT(source.getSource("invalid_id") == false);
    CPPUNIT_ASSERT_EQUAL(false, source.hasSource("invalid_id"));
    Source s;
    CPPUNIT_ASSERT(!source.hasSource(s));

    std::vector<std::string> ids;
    for (const auto &name : names) {
        Source child_source = source.createSource(name, "channel");
        CPPUNIT_ASSERT(child_source.name() == name);
        CPPUNIT_ASSERT(source.hasSource(child_source));
        CPPUNIT_ASSERT(source.hasSource(name));
        ids.push_back(child_source.id());
    }
    CPPUNIT_ASSERT_THROW(source.createSource(names[0], "channel"),
                         DuplicateName);
    CPPUNIT_ASSERT_THROW(source.createSource("", "channel"), EmptyString);

    CPPUNIT_ASSERT(source.sourceCount() == names.size());
    CPPUNIT_ASSERT(source.sources().size() == names.size());
    CPPUNIT_ASSERT_THROW(source.getSource(source.sourceCount() + 1), OutOfBounds);

    for (const auto &id : ids) {
        Source child_source = source.getSource(id);
        CPPUNIT_ASSERT(source.hasSource(id));
        CPPUNIT_ASSERT(child_source.id() == id);

        source.deleteSource(id);
    }
    Source s1, s2;
    s1 = source.createSource("name", "type");
    CPPUNIT_ASSERT(!source.deleteSource(s2));
    CPPUNIT_ASSERT_NO_THROW(source.deleteSource(s1));

    CPPUNIT_ASSERT(source.sourceCount() == 0);
    CPPUNIT_ASSERT(source.sources().size() == 0);
    CPPUNIT_ASSERT(source.getSource("invalid_id") == false);
}


void BaseTestSource::testFindSource() {
    /* We create the following tree:
     * 
     * source---l1n1---l2n1---l3n1
     *    |      |      |
     *    |      ------l2n2
     *    |      |
     *    |      |-----l2n3---l3n2
     *    |             |
     *    |             ------l3n3
     *    ------l1n2
     *    | 
     *    ------l1n3---l2n4
     *           |
     *           ------l2n5---l3n4
     *           |
     *           ------l2n6---l3n5
     *                  |      |
     * mtag-------------|      |
     *                         |
     * darray-------------------
     */
    Source l1n1 = source.createSource("l1n1", "typ1");
    Source l1n2 = source.createSource("l1n2", "typ2");
    Source l1n3 = source.createSource("l1n3", "typ3");
    Source l2n1 = l1n1.createSource("l2n1", "typ1");
    Source l2n2 = l1n1.createSource("l2n2", "typ2");
    Source l2n3 = l1n1.createSource("l2n3", "typ2");
    Source l2n4 = l1n3.createSource("l2n4", "typ2");
    Source l2n5 = l1n3.createSource("l2n5", "typ2");
    Source l2n6 = l1n3.createSource("l2n6", "typ3");
    Source l3n1 = l2n1.createSource("l3n1", "typ1");
    Source l3n2 = l2n3.createSource("l3n2", "typ2");
    Source l3n3 = l2n3.createSource("l3n3", "typ2");
    Source l3n4 = l2n5.createSource("l3n4", "typ2");
    Source l3n5 = l2n5.createSource("l3n5", "typ2");
    mtag.addSource(l2n6.id());
    darray.addSource(l3n5.id());
    
    // test if sources are in place
    CPPUNIT_ASSERT(mtag.hasSource(l2n6));
    CPPUNIT_ASSERT(darray.hasSource(l3n5));
    CPPUNIT_ASSERT(mtag.sources().size() == 1);
    CPPUNIT_ASSERT(darray.sources().size() == 1);

    // test depth limit
    CPPUNIT_ASSERT(source.findSources().size() == 15);
    CPPUNIT_ASSERT(source.findSources(util::AcceptAll<Source>(), 2).size() == 10);
    CPPUNIT_ASSERT(source.findSources(util::AcceptAll<Source>(), 1).size() == 4);
    CPPUNIT_ASSERT(source.findSources(util::AcceptAll<Source>(), 0).size() == 1);

    // test filter
    auto filter_typ1 = util::TypeFilter<Source>("typ1");
    auto filter_typ2 = util::TypeFilter<Source>("typ2");
    CPPUNIT_ASSERT(source.findSources(filter_typ1).size() == 3);
    CPPUNIT_ASSERT(source.findSources(filter_typ2).size() == 9);
    
    // test deleter
    /* chop the tree down to:
     * 
     * source---l1n2
     *    | 
     *    ------l1n3---l2n4
     *           |
     *           ------l2n5---l3n4
     *           |
     *           ------l2n6---l3n5
     *                  |      |
     * mtag-------------|      |
     *                         |
     * darray-------------------
     */
    source.deleteSource(l1n1.id());
    CPPUNIT_ASSERT(source.findSources().size() == 8);
    /* chop the tree down to:
     * 
     * source---l1n3---l2n4
     *           |
     *           ------l2n5---l3n4
     *           |
     *           ------l2n6---l3n5
     *                  |      |
     * mtag-------------|      |
     *                         |
     * darray-------------------
     */
    source.deleteSource(l1n2.id());
    CPPUNIT_ASSERT(source.findSources().size() == 7);
    /* chop the tree down to:
     * 
     * source
     * mtag
     * darray
     */
    source.deleteSource(l1n3.id());
    CPPUNIT_ASSERT(source.findSources().size() == 1);
    CPPUNIT_ASSERT(!mtag.hasSource(l2n6));
    CPPUNIT_ASSERT(!darray.hasSource(l3n5));
    CPPUNIT_ASSERT(mtag.sources().size() == 0);
    CPPUNIT_ASSERT(darray.sources().size() == 0);
}


void BaseTestSource::testReferringDataArrays() {
    nix::Source ref_src = block.createSource("referrenced", "test");
    CPPUNIT_ASSERT(ref_src.referringDataArrays(block).size() == 0);
    for (int i = 0; i < 10; i++) {
        std::string name = "data_array_" + nix::util::numToStr(i);
        nix::DataArray da = block.createDataArray(name, "analog signal", nix::DataType::Double, nix::NDSize({ 20, 20 }));
        if (i % 2 == 0) {
            da.addSource(ref_src);
        }
    }
    CPPUNIT_ASSERT(ref_src.referringDataArrays(block).size() == 5);
    CPPUNIT_ASSERT(ref_src.referringDataArrays().size() == 5);
    block.deleteSource(ref_src);
}


void BaseTestSource::testReferringMultiTags() {
    nix::Source ref_src = block.createSource("referrenced", "test");
    CPPUNIT_ASSERT(ref_src.referringMultiTags(block).size() == 0);
    DataArray positions = block.createDataArray("positions", "positions", nix::DataType::Double, nix::NDSize({ 20, 1 }));
    for (int i = 0; i < 10; i++) {
        std::string name = "tag_" + nix::util::numToStr(i);
        nix::MultiTag t = block.createMultiTag(name, "some tag", positions);
        if (i % 2 == 0) {
            t.addSource(ref_src);
        }
    }
    CPPUNIT_ASSERT(ref_src.referringMultiTags(block).size() == 5);
    CPPUNIT_ASSERT(ref_src.referringMultiTags().size() == 5);
    block.deleteSource(ref_src);
    block.deleteDataArray(positions);
}


void BaseTestSource::testReferringTags() {
    nix::Source ref_src = block.createSource("referrenced", "test");
    CPPUNIT_ASSERT(ref_src.referringTags(block).size() == 0);
    for (int i = 0; i < 10; i++) {
        std::string name = "tag_" + nix::util::numToStr(i);
        nix::Tag t = block.createTag(name, "some_tag", {1.});
        if (i % 2 == 0) {
            t.addSource(ref_src);
        }
    }
    CPPUNIT_ASSERT(ref_src.referringTags(block).size() == 5);
    CPPUNIT_ASSERT(ref_src.referringTags().size() == 5);
    block.deleteSource(ref_src);
}


void BaseTestSource::testOperators() {
    CPPUNIT_ASSERT(source_null == false);
    CPPUNIT_ASSERT(source_null == none);

    CPPUNIT_ASSERT(source != false);
    CPPUNIT_ASSERT(source != none);

    CPPUNIT_ASSERT(source == source);
    CPPUNIT_ASSERT(source != source_other);

    source_other = source;
    CPPUNIT_ASSERT(source == source_other);

    source_other = none;
    CPPUNIT_ASSERT(source_null == false);
    CPPUNIT_ASSERT(source_null == none);
}


void BaseTestSource::testCreatedAt() {
    CPPUNIT_ASSERT(source.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    source.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(source.createdAt() == past_time);
}


void BaseTestSource::testUpdatedAt() {
    CPPUNIT_ASSERT(source.updatedAt() >= startup_time);
}
