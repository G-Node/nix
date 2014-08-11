// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Jan Grewe <jan.grewe@g-node.org>

#include "TestDataTag.hpp"
#include <nix/Exception.hpp>
#include <sstream>
#include <ctime>

#include <nix/valid/validate.hpp>

using namespace nix;
using namespace nix::hdf5;
using namespace valid;


void TestDataTag::setUp() {
    startup_time = time(NULL);
    file = File::open("test_dataTag.h5", FileMode::Overwrite);
    block = file.createBlock("block", "dataset");

    positions = block.createDataArray("positions_DataArray", "dataArray",
                                      DataType::Double, NDSize({ 0, 0 }));
    extents = block.createDataArray("extents_DataArray", "dataArray",
                                    DataType::Double, NDSize({ 0, 0 }));

    typedef boost::multi_array<double, 2> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[5][5]);
    for(index i = 0; i < 5; ++i){
        A[i][i] = 100.0*i;
    }
    positions.setData(A);

    array_type B(boost::extents[5][5]);
    for(index i = 0; i < 5; ++i){
        B[i][i] = 100.0*i;
    }
    extents.setData(B);

    tag = block.createDataTag("tag_one", "test_tag", positions);
    tag_other = block.createDataTag("tag_two", "test_tag", positions);
    tag_null = nullptr;

    section = file.createSection("foo_section", "metadata");
}


void TestDataTag::tearDown(){
    file.deleteBlock(block.id());
    file.deleteSection(section.id());
    file.close();
}


void TestDataTag::testValidate() {
    valid::Result result = validate(tag);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void TestDataTag::testId() {
    CPPUNIT_ASSERT(tag.id().size() == 25);
}


void TestDataTag::testName() {
    CPPUNIT_ASSERT(tag.name() == "tag_one");
    std::string name = util::createId("", 32);
    tag.name(name);
    CPPUNIT_ASSERT(tag.name() == name);
}


void TestDataTag::testType() {
    CPPUNIT_ASSERT(tag.type() == "test_tag");
    std::string type = util::createId("", 32);
    tag.type(type);
    CPPUNIT_ASSERT(tag.type() == type);
}


void TestDataTag::testDefinition() {
    std::string def = util::createId("", 128);
    tag.definition(def);
    CPPUNIT_ASSERT(*tag.definition() == def);
    tag.definition(none);
    CPPUNIT_ASSERT(tag.definition() == none);
}


void TestDataTag::testCreateRemove() {
    std::vector<std::string> ids;
    size_t count = block.dataTagCount();
    const char *names[5] = { "tag_a", "tag_b", "tag_c", "tag_d", "tag_e" };
    for (int i = 0; i < 5; i++) {
        std::string type = "Event";
        DataTag dt1 = block.createDataTag(names[i], type, positions);
        DataTag dt2 = block.getDataTag(dt1.id());
        ids.push_back(dt1.id());

        std::stringstream errmsg;
        errmsg << "Error while accessing dataTag: dt1.id() = " << dt1.id()
               << " / dt2.id() = " << dt2.id();
        CPPUNIT_ASSERT_MESSAGE(errmsg.str(), dt1.id().compare(dt2.id()) == 0);
    }
    std::stringstream errmsg2;
    errmsg2 << "Error creating DataTags. Counts do not match!";
    CPPUNIT_ASSERT_MESSAGE(errmsg2.str(), block.dataTagCount() == (count+5));

    for (size_t i = 0; i < ids.size(); i++) {
        block.deleteDataTag(ids[i]);
    }

    std::stringstream errmsg1;
    errmsg1 << "Error while removing dataTags!";
    CPPUNIT_ASSERT_MESSAGE(errmsg1.str(), block.dataTagCount() == count);
}

void TestDataTag::testUnits() {
    DataTag dt = block.createDataTag("TestDataTag1", "Tag", positions);

    std::vector<std::string> valid_units = {"mV", "cm", "m^2"};
    std::vector<std::string> invalid_units = {"mV", "haha", "qm^2"};
    std::vector<std::string> insane_units = {"muV ", " muS"};

    CPPUNIT_ASSERT_NO_THROW(dt.units(valid_units));
    CPPUNIT_ASSERT(dt.units().size() == valid_units.size());
    std::vector<std::string> retrieved_units = dt.units();
    for (size_t i = 0; i < retrieved_units.size(); i++) {
        CPPUNIT_ASSERT(retrieved_units[i] == valid_units[i]);
    }

    dt.units(none);
    CPPUNIT_ASSERT(dt.units().size() == 0);
    CPPUNIT_ASSERT_THROW(dt.units(invalid_units), InvalidUnit);
    CPPUNIT_ASSERT(dt.units().size() == 0);

    dt.units(insane_units);
    retrieved_units = dt.units();
    CPPUNIT_ASSERT(retrieved_units.size() == 2);
    CPPUNIT_ASSERT(retrieved_units[0] == "uV");
    CPPUNIT_ASSERT(retrieved_units[1] == "uS");

    block.deleteDataTag(dt.id());
}

//TODO Constraints on References are not tested yet.

void TestDataTag::testReferences(){
    DataArray da_1 = block.createDataArray("TestReference 1",
                                           "Reference",
                                           DataType::Double,
                                           NDSize({ 0 }));
    DataArray da_2 = block.createDataArray("TestReference 2", "Reference",
                                           DataType::Double,
                                           NDSize({ 0 }));

    DataTag dt = block.createDataTag("TestDataTag1", "Tag", positions);

    CPPUNIT_ASSERT_THROW(dt.getReference(42), OutOfBounds);

    std::stringstream counterrmsg;
    counterrmsg << "TestDataTag::testReference: Counts do not match!";
    CPPUNIT_ASSERT_MESSAGE(counterrmsg.str(), dt.referenceCount() == 0);

    dt.addReference(da_1);
    dt.addReference(da_2);
    CPPUNIT_ASSERT_MESSAGE(counterrmsg.str(), dt.referenceCount() == 2);

    DataArray ref1 = dt.getReference(da_1.id());
    std::stringstream retrieveerrmsg;
    retrieveerrmsg << "TestDataTag::testReference: Retrieval did not work!";
    CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), ref1.id() == da_1.id());

    std::vector<DataArray> arrays = dt.references();
    CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), arrays.size() == 2);

    std::stringstream hasReferrmsg;
    hasReferrmsg << "TestDataTag::testReference: hadReference did not work!";
    CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), dt.hasReference(da_1.id()));
    CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), dt.hasReference(da_2.id()));

    std::stringstream delReferrmsg;
    delReferrmsg << "TestDataTag::testReference: removeReference did not work!";
    dt.removeReference(da_1.id());
    CPPUNIT_ASSERT_MESSAGE(delReferrmsg.str(), dt.referenceCount() == 1);
    dt.removeReference(da_2.id());
    CPPUNIT_ASSERT_MESSAGE(delReferrmsg.str(), dt.referenceCount() == 0);

    block.deleteDataArray(da_1.id());
    block.deleteDataArray(da_1.id());
    block.deleteDataTag(dt.id());
}


void TestDataTag::testExtents(){
    CPPUNIT_ASSERT_THROW(tag.extents("wrong_data_array_id"), std::runtime_error);

    typedef boost::multi_array<double, 2> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[5][5]);
    for(index i = 0; i < 5; ++i){
        A[i][i] = 100.0*i;
    }
    positions.setData(A);
    extents.setData(A);

    tag.positions(positions);
    tag.extents(extents);
    CPPUNIT_ASSERT(tag.extents() == true);
    tag.extents(none);
    CPPUNIT_ASSERT(tag.extents() == false);
}


void TestDataTag::testPositions() {
    CPPUNIT_ASSERT_THROW(tag.positions("wrong_data_array_id"), std::runtime_error);

    tag.positions(positions);
    CPPUNIT_ASSERT(tag.positions() == true);
}


void TestDataTag::testPositionExtents() {
    typedef boost::multi_array<double, 2> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[10][10]);
    for(index i = 0; i < 10; ++i){
        A[i][i] = 100.0*i;
    }
    positions.setData(A);

    tag.positions(positions);
    CPPUNIT_ASSERT_THROW(tag.extents(extents), std::runtime_error);
    tag.extents(none);
    CPPUNIT_ASSERT(tag.extents() == false);
}


void TestDataTag::testMetadataAccess() {
    CPPUNIT_ASSERT(!tag.metadata());
    tag.metadata(section);
    CPPUNIT_ASSERT(tag.metadata());
    // TODO This test fails due to operator== of Section
    // CPPUNIT_ASSERT(tag.metadata() == section);

    tag.metadata(none);
    CPPUNIT_ASSERT(!tag.metadata());
}


void TestDataTag::testSourceAccess(){
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
    CPPUNIT_ASSERT(tag.sources().size() == names.size());

    for (auto it = ids.begin(); it != ids.end(); it++) {
        Source child_source = tag.getSource(*it);
        CPPUNIT_ASSERT(tag.hasSource(*it) == true);
        CPPUNIT_ASSERT(child_source.id() == *it);

        tag.removeSource(*it);
        block.deleteSource(*it);
    }

    CPPUNIT_ASSERT(tag.sourceCount() == 0);
    CPPUNIT_ASSERT(tag.sources().size() == 0);
}

void TestDataTag::testOperators() {
    CPPUNIT_ASSERT(tag_null == false);
    CPPUNIT_ASSERT(tag_null == none);

    CPPUNIT_ASSERT(tag != false);
    CPPUNIT_ASSERT(tag != none);

    CPPUNIT_ASSERT(tag == tag);
    CPPUNIT_ASSERT(tag != tag_other);

    tag_other = tag;
    CPPUNIT_ASSERT(tag == tag_other);

    tag_other = none;
    CPPUNIT_ASSERT(tag_null == false);
    CPPUNIT_ASSERT(tag_null == none);
}


void TestDataTag::testCreatedAt() {
    CPPUNIT_ASSERT(tag.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    tag.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(tag.createdAt() == past_time);
}


void TestDataTag::testUpdatedAt() {
    CPPUNIT_ASSERT(tag.updatedAt() >= startup_time);
}
