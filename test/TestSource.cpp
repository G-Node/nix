// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include <nix/util/util.hpp>
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
    CPPUNIT_ASSERT(source.name() == "source_one");
    string name = util::createId("", 32);
    source.name(name);
    CPPUNIT_ASSERT(source.name() == name);
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
    CPPUNIT_ASSERT(source.definition() == def);
}


void TestSource::testMetadataAccess() {
    CPPUNIT_ASSERT(!source.hasMetadata());

    source.metadata(section);
    CPPUNIT_ASSERT(source.hasMetadata());
    // TODO This test fails due to operator== of Section
    // CPPUNIT_ASSERT(source.metadata() == section);

    source.removeMetadata();
    CPPUNIT_ASSERT(!source.hasMetadata());
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

        source.removeSource(*it);
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
    CPPUNIT_ASSERT(source.findSources(acceptAllFilter, 2).size() == 10);
    CPPUNIT_ASSERT(source.findSources(acceptAllFilter, 1).size() == 4);
    CPPUNIT_ASSERT(source.findSources(acceptAllFilter, 0).size() == 1);

    // test filter
    auto filter_typ1 = [](const Source &s) { return s.type() == "typ1"; };
    auto filter_typ2 = [](const Source &s) { return s.type() == "typ2"; };

    CPPUNIT_ASSERT(source.findSources(filter_typ1).size() == 3);
    CPPUNIT_ASSERT(source.findSources(filter_typ2).size() == 8);
}


void TestSource::testOperators() {
    CPPUNIT_ASSERT(source_null == NULL);
    CPPUNIT_ASSERT(source_null == nullptr);

    CPPUNIT_ASSERT(source != NULL);
    CPPUNIT_ASSERT(source != nullptr);

    CPPUNIT_ASSERT(source == source);
    CPPUNIT_ASSERT(source != source_other);

    source_other = source;
    CPPUNIT_ASSERT(source == source_other);

    source_other = nullptr;
    CPPUNIT_ASSERT(source_null == NULL);
    CPPUNIT_ASSERT(source_null == nullptr);
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

/*

void TestSource::testCreateAndRemove() {
    Block b1 = f1->createBlock("test block","test");
    Source s1 = b1.createSource("S1","test");
    Source s2 = b1.createSource("S2","test");

    stringstream msg;
    msg << "Creating s1 or s2 failed!" ;
    CPPUNIT_ASSERT_MESSAGE(msg.str(), b1.hasSource(s1.id()) && b1.hasSource(s2.id()) );

    size_t count = b1.sourceCount();
    stringstream errmsg;
    errmsg << "Source count does not match! Found " << count << " should have been 2";
    CPPUNIT_ASSERT_MESSAGE(errmsg.str(), count == 2);

    b1.removeSource(s1.id());
    stringstream msg2;
    msg2 << "Removing s1 failed!" ;
    CPPUNIT_ASSERT_MESSAGE(msg2.str(), !b1.hasSource(s1.id()));
    b1.removeSource(s2.id());
    f1->removeBlock(b1.id());
}

void TestSource::testIterators(){
    Block b1 = f1->createBlock("test block","test");
    Source s1 = b1.createSource("S1","test");
    s1.createSource("S3","test");
    s1.createSource("S4","test");
    Source s2 = b1.createSource("S2","test");
    s2.createSource("S5","test");

    size_t count = s1.childCount();
    CPPUNIT_ASSERT_EQUAL(count,(size_t)2);
    count = s2.childCount();
    CPPUNIT_ASSERT_EQUAL(count,(size_t)1);

    b1.removeSource(s1.id());
    b1.removeSource(s2.id());
    f1->removeBlock(b1.id());

}

void TestSource::testFindSources(){
    Block b1 = f1->createBlock("test block","test");
    Source s1 = b1.createSource("S1","test");
    Source s2 = b1.createSource("S2","test");
    Source s3 = s1.createSource("S3","test");
    Source s4 = s1.createSource("S4","test");
    Source s5 = s2.createSource("S5","test");

    //sanity check
    vector<Source> res = s1.collectIf([&](const Source &source) {
        return false;
    });

    CPPUNIT_ASSERT_EQUAL(static_cast<vector<Source>::size_type>(0), res.size());


    //now some actual work
    res = s1.collectIf([&](const Source &source) {
        bool found = source.id() == s4.id();
        return found;
    });

    CPPUNIT_ASSERT_EQUAL(static_cast<vector<Source>::size_type>(1), res.size());
    CPPUNIT_ASSERT_EQUAL(s4.id(), res[0].id());

    res = s1.collectIf([&](const Source &source) {
        return true;
    }, true, 1);

    CPPUNIT_ASSERT_EQUAL(res.size(), s1.childCount());
    vector<Source> children = s1.sources();

    for (size_t i = 0; i < res.size(); i++) {
        CPPUNIT_ASSERT_EQUAL(res[i].id(), children[i].id());
    }


    //    CPPUNIT_ASSERT_EQUAL(b1.hasSource("invalid_id"),false);
    //    CPPUNIT_ASSERT_EQUAL(b1.hasSource(s3.id()),true);
    //    CPPUNIT_ASSERT_EQUAL(b1.existsSource(s3.id(),"test"),true);
    //    CPPUNIT_ASSERT_EQUAL(b1.existsSource(s3.id(),"no test"),false);
    //    CPPUNIT_ASSERT_EQUAL(b1.existsSource(s3.id(),"test",1),false);
    //    CPPUNIT_ASSERT_EQUAL(b1.existsSource(s3.id(),"test",2),true);

    b1.removeSource(s1.id());
    b1.removeSource(s2.id());
    f1->removeBlock(b1.id());
}

*/
