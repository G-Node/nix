// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Jan Grewe <jan.grewe@g-node.org>

#include "TestSimpleTag.hpp"
#include <nix/Exception.hpp>
#include <sstream>
#include <ctime>

using namespace nix;
using namespace nix::hdf5;


void TestSimpleTag::setUp() {
    startup_time = time(NULL);
    file = File::open("test_dataTag.h5", FileMode::Overwrite);
    block = file.createBlock("block", "dataset");
    tag = block.createSimpleTag("tag_one", "test_tag");
    tag_other = block.createSimpleTag("tag_two", "test_tag");
    tag_null = nullptr;

    section = file.createSection("foo_section", "metadata");
}


void TestSimpleTag::tearDown(){
    file.deleteBlock(block.id());
    file.deleteSection(section.id());
    file.close();
}


void TestSimpleTag::testId() {
    CPPUNIT_ASSERT(tag.id().size() == 27);
}


void TestSimpleTag::testName() {
    CPPUNIT_ASSERT(*tag.name() == "tag_one");
    std::string name = util::createId("", 32);
    tag.name(name);
    CPPUNIT_ASSERT(*tag.name() == name);
}


void TestSimpleTag::testType() {
    CPPUNIT_ASSERT(tag.type() == "test_tag");
    std::string type = util::createId("", 32);
    tag.type(type);
    CPPUNIT_ASSERT(tag.type() == type);
}


void TestSimpleTag::testDefinition() {
    std::string def = util::createId("", 128);
    tag.definition(def);
    CPPUNIT_ASSERT(*tag.definition() == def);
}


void TestSimpleTag::testCreateRemove() {
    std::vector<std::string> ids;
    size_t count = block.simpleTagCount();
    const char *names[5] = { "tag_a", "tag_b", "tag_c", "tag_d",
            "tag_e" };
    for (int i = 0; i < 5; i++) {
        std::string type = "Event";
        SimpleTag st1 = block.createSimpleTag(names[i], type);
        SimpleTag st2 = block.getSimpleTag(st1.id());
        ids.push_back(st1.id());

        std::stringstream errmsg;
        errmsg << "Error while accessing simpleTag: st1.id() = " << st1.id()
                                       << " / st2.id() = " << st2.id();
        CPPUNIT_ASSERT_MESSAGE(errmsg.str(), st1.id().compare(st2.id()) == 0);
    }
    std::stringstream errmsg2;
    errmsg2 << "Error creating SimpleTags. Counts do not match!";
    CPPUNIT_ASSERT_MESSAGE(errmsg2.str(), block.simpleTagCount() == (count+5));

    for (size_t i = 0; i < ids.size(); i++) {
        block.deleteSimpleTag(ids[i]);
    }

    std::stringstream errmsg1;
    errmsg1 << "Error while removing dataTags!";
    CPPUNIT_ASSERT_MESSAGE(errmsg1.str(), block.simpleTagCount() == count);
}

//TODO Constraints on References are not tested yet.

void TestSimpleTag::testReferences(){
    DataArray da_1 = block.createDataArray("TestReference 1","Reference");
    DataArray da_2 = block.createDataArray("TestReference 2","Reference");
    SimpleTag st = block.createSimpleTag("TestSimpleTag1","Tag");

    CPPUNIT_ASSERT_THROW(st.getReference(42), nix::OutOfBounds);

    std::stringstream counterrmsg;
    counterrmsg << "TestDataTag::testReference: Counts do not match!";
    CPPUNIT_ASSERT_MESSAGE(counterrmsg.str(), st.referenceCount() == 0);

    st.addReference(da_1.id());
    st.addReference(da_2.id());

    CPPUNIT_ASSERT_MESSAGE(counterrmsg.str(), st.referenceCount() == 2);

    std::stringstream retrieveerrmsg;
    DataArray ref1 = st.getReference(da_1.id());
    retrieveerrmsg << "TestSimpleTag::testReference: Retrieval did not work!";
    CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), ref1.id() == da_1.id());

    std::vector<DataArray> arrays = st.references();
    CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), arrays.size() == 2);

    std::stringstream hasReferrmsg;
    hasReferrmsg << "TestSimpleTag::testReference: hadReference did not work!";
    CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), st.hasReference(da_1.id()));
    CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), st.hasReference(da_2.id()));

    std::stringstream delReferrmsg;
    delReferrmsg << "TestSimpleTag::testReference: removeReference did not work!";
    st.removeReference(da_1.id());
    CPPUNIT_ASSERT_MESSAGE(delReferrmsg.str(), st.referenceCount() == 1);
    st.removeReference(da_2.id());
    CPPUNIT_ASSERT_MESSAGE(delReferrmsg.str(), st.referenceCount() == 0);

    block.deleteDataArray(da_1.id());
    block.deleteDataArray(da_1.id());
    block.deleteSimpleTag(st.id());
}


void TestSimpleTag::testExtent(){
    SimpleTag st = block.createSimpleTag("TestSimpleTag1","Tag");

    std::vector<double> extent = {1.0, 2.0, 3.0};
    st.extent(extent);

    std::vector<double> retrieved = st.extent();
    CPPUNIT_ASSERT(retrieved.size() == extent.size());
    for(size_t i = 0; i < retrieved.size(); i++){
        CPPUNIT_ASSERT(retrieved[i] == extent[i]);
    }

    st.extent(boost::none);
    CPPUNIT_ASSERT(st.extent().size() == 0);
    block.deleteSimpleTag(st.id());
}


void TestSimpleTag::testPosition(){
    SimpleTag st = block.createSimpleTag("TestSimpleTag1","Tag");

    std::vector<double> position = {1.0, 2.0, 3.0};
    std::vector<double> new_position = {2.0};

    CPPUNIT_ASSERT(st.position().size() == 0);
    st.position(position);
    std::vector<double> retrieved = st.position();
    CPPUNIT_ASSERT(retrieved.size() == position.size());

    for(size_t i = 0; i < retrieved.size(); i++){
        CPPUNIT_ASSERT(retrieved[i] == position[i]);
    }

    st.position(new_position);
    retrieved = st.position();
    CPPUNIT_ASSERT(retrieved.size() == new_position.size());

    for(size_t i = 0; i < retrieved.size(); i++){
        CPPUNIT_ASSERT(retrieved[i] == new_position[i]);
    }
    block.deleteSimpleTag(st.id());
}


void TestSimpleTag::testMetadataAccess() {
    CPPUNIT_ASSERT(!tag.hasMetadata());

    tag.metadata(section);
    CPPUNIT_ASSERT(tag.hasMetadata());
    // TODO This test fails due to operator== of Section
    // CPPUNIT_ASSERT(source.metadata() == section);

    tag.metadata(boost::none);
    CPPUNIT_ASSERT(!tag.hasMetadata());
}


void TestSimpleTag::testSourceAccess(){
    std::vector<std::string> names = { "source_a", "source_b", "source_c", "source_d", "source_e" };
    CPPUNIT_ASSERT(tag.sourceCount() == 0);
    CPPUNIT_ASSERT(tag.sources().size() == 0);

    std::vector<std::string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        Source child_source = block.createSource(*it,"channel");
        tag.addSource(child_source);
        CPPUNIT_ASSERT(child_source.name() == *it);
        ids.push_back(child_source.id());
    }

    CPPUNIT_ASSERT(tag.sourceCount() == names.size());
    //TODO the following test crashes!!!
    //std::vector<Source> sources = tag.sources();
    //std::cerr << "\n" <<sources.size() << "\n";
    //CPPUNIT_ASSERT(tag.sources().size() == names.size());

    for (auto it = ids.begin(); it != ids.end(); it++) {
        Source child_source = tag.getSource(*it);
        CPPUNIT_ASSERT(tag.hasSource(*it) == true);
        CPPUNIT_ASSERT(child_source.id() == *it);

        tag.removeSource(*it);
        block.deleteSource(*it);
    }

    CPPUNIT_ASSERT(tag.sourceCount() == 0);
}


void TestSimpleTag::testUnits(){
    SimpleTag st = block.createSimpleTag("TestSimpleTag1","Tag");

    std::vector<std::string> valid_units = {"mV", "cm", "m^2"};
    std::vector<std::string> invalid_units = {"mV", "haha", "qm^2"};
    CPPUNIT_ASSERT_NO_THROW(st.units(valid_units));
    CPPUNIT_ASSERT(st.units().size() == valid_units.size());
    std::vector<std::string> retrieved_units = st.units();
    for(size_t i = 0; i < retrieved_units.size(); i++){
        CPPUNIT_ASSERT(retrieved_units[i] == valid_units[i]);
    }

    st.units(boost::none);
    CPPUNIT_ASSERT(st.units().size() == 0);
    CPPUNIT_ASSERT_THROW(st.units(invalid_units), nix::InvalidUnit);
    CPPUNIT_ASSERT(st.units().size() == 0);
    block.deleteSimpleTag(st.id());
}


void TestSimpleTag::testOperators() {
    CPPUNIT_ASSERT(tag_null == NULL);
    CPPUNIT_ASSERT(tag_null == nullptr);

    CPPUNIT_ASSERT(tag != NULL);
    CPPUNIT_ASSERT(tag != nullptr);

    CPPUNIT_ASSERT(tag == tag);
    CPPUNIT_ASSERT(tag != tag_other);

    tag_other = tag;
    CPPUNIT_ASSERT(tag == tag_other);

    tag_other = nullptr;
    CPPUNIT_ASSERT(tag_null == NULL);
    CPPUNIT_ASSERT(tag_null == nullptr);
}


void TestSimpleTag::testCreatedAt() {
    CPPUNIT_ASSERT(tag.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    tag.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(tag.createdAt() == past_time);
}


void TestSimpleTag::testUpdatedAt() {
    CPPUNIT_ASSERT(tag.updatedAt() >= startup_time);
}




