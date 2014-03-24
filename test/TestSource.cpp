// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include "TestSource.hpp"


using namespace std;
using namespace nix;


void TestSource::setUp() {
    startup_time = time(NULL);
    file = File::open("test_source.h5", FileMode::Overwrite);
    block = file.createBlock("block", "dataset");
    section = file.createSection("foo_section", "metadata");

    source = block.createSource("source_one", "channel");
    source_other = block.createSource("source_two", "channel");
    source_null  = nullptr;
}


void TestSource::tearDown() {
    file.close();
}


void TestSource::testId() {
    CPPUNIT_ASSERT(source.id().size() == 23);
}


void TestSource::testName() {
    CPPUNIT_ASSERT(*source.name() == "source_one");
    string name = util::createId("", 32);
    source.name(name);
    CPPUNIT_ASSERT(*source.name() == name);
}


void TestSource::testType() {
    CPPUNIT_ASSERT(source.type() == "channel");
    string typ = util::createId("", 32);
    source.type(typ);
    CPPUNIT_ASSERT(source.type() == typ);
}


void TestSource::testDefinition() {
    string def = util::createId("", 128);
    source.definition(def);
    CPPUNIT_ASSERT(*source.definition() == def);
}


void TestSource::testMetadataAccess() {
    CPPUNIT_ASSERT(!source.metadata());

    source.metadata(section);
    CPPUNIT_ASSERT(source.metadata());

    source.metadata(none);
    CPPUNIT_ASSERT(!source.metadata());
}


void TestSource::testSourceAccess() {
    vector<string> names = { "source_a", "source_b", "source_c", "source_d", "source_e" };

    CPPUNIT_ASSERT(source.sourceCount() == 0);
    CPPUNIT_ASSERT(source.sources().size() == 0);


    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        Source child_source = source.createSource(*it, "channel");
        CPPUNIT_ASSERT(child_source.name() == *it);

        ids.push_back(child_source.id());
    }


    CPPUNIT_ASSERT(source.sourceCount() == names.size());
    CPPUNIT_ASSERT(source.sources().size() == names.size());


    for (auto it = ids.begin(); it != ids.end(); it++) {
        Source child_source = source.getSource(*it);
        CPPUNIT_ASSERT(source.hasSource(*it) == true);
        CPPUNIT_ASSERT(child_source.id() == *it);

        source.deleteSource(*it);
    }

    CPPUNIT_ASSERT(source.sourceCount() == 0);
    CPPUNIT_ASSERT(source.sources().size() == 0);
}


void TestSource::testFindSource() {
    // prepare
    Source l1n1 = source.createSource("l1n1", "typ1");
    Source l1n2 = source.createSource("l1n2", "typ2");
    Source l1n3 = source.createSource("l1n3", "typ3");

    Source l2n1 = l1n1.createSource("l2n1", "typ1");
    Source l2n2 = l1n1.createSource("l2n2", "typ2");
    Source l2n3 = l1n1.createSource("l2n3", "typ2");
    Source l2n4 = l1n3.createSource("l2n4", "typ2");
    Source l2n5 = l1n3.createSource("l2n5", "typ2");
    Source l2n6 = l1n3.createSource("l2n6", "typ3");

    Source l3n1 = l2n1.createSource("l2n3", "typ1");
    Source l3n2 = l2n3.createSource("l2n3", "typ2");
    Source l3n3 = l2n3.createSource("l2n3", "typ2");
    Source l3n4 = l2n5.createSource("l2n3", "typ2");

    // test depth limit
    CPPUNIT_ASSERT(source.findSources().size() == 14);
    CPPUNIT_ASSERT(source.findSources(util::AcceptAll<Source>(), 2).size() == 10);
    CPPUNIT_ASSERT(source.findSources(util::AcceptAll<Source>(), 1).size() == 4);
    CPPUNIT_ASSERT(source.findSources(util::AcceptAll<Source>(), 0).size() == 1);

    // test filter
    auto filter_typ1 = util::TypeFilter<Source>("typ1");
    auto filter_typ2 = util::TypeFilter<Source>("typ2");

    CPPUNIT_ASSERT(source.findSources(filter_typ1).size() == 3);
    CPPUNIT_ASSERT(source.findSources(filter_typ2).size() == 8);
}


void TestSource::testOperators() {
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


void TestSource::testCreatedAt() {
    CPPUNIT_ASSERT(source.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    source.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(source.createdAt() == past_time);
}


void TestSource::testUpdatedAt() {
    CPPUNIT_ASSERT(source.updatedAt() >= startup_time);
}

