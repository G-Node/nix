// Copyright © 2014 - 2015 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Jan Grewe <jan.grewe@g-node.org>

#include "TestMultiTag.hpp"

#include <nix/Exception.hpp>
#include <nix/valid/validate.hpp>

#include <sstream>
#include <ctime>


using namespace nix;
using namespace valid;


void TestMultiTag::setUp() {
    startup_time = time(NULL);
    file = File::open("test_multiTag.h5", FileMode::Overwrite);
    block = file.createBlock("block", "dataset");
    positions = block.createDataArray("positions_DataArray", "dataArray",
                                      DataType::Double, NDSize({ 5, 5 }));
    extents = block.createDataArray("extents_DataArray", "dataArray",
                                    DataType::Double, NDSize({ 5, 5 }));
    wrongArray = block.createDataArray("wrong data", "test", DataType::Double, NDSize({5}));

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

    tag = block.createMultiTag("tag_one", "test_tag", positions);
    tag_other = block.createMultiTag("tag_two", "test_tag", positions);
    tag_null = nix::none;

    section = file.createSection("foo_section", "metadata");

    file_fs = File::open("test_multi_tag", FileMode::Overwrite, Implementation::FileSys);
    block_fs = file_fs.createBlock("block", "dataset");
    positions_fs = block_fs.createDataArray("positions_DataArray", "dataArray",
                                            DataType::Double, NDSize({ 0, 0 }));
    extents_fs = block_fs.createDataArray("extents_DataArray", "dataArray",
                                          DataType::Double, NDSize({ 0, 0 }));

    tag_fs = block_fs.createMultiTag("tag_one", "test_tag", positions_fs);
    tag_other_fs = block_fs.createMultiTag("tag_two", "test_tag", positions_fs);
    section_fs = file_fs.createSection("foo_section", "metadata");
}


void TestMultiTag::tearDown(){
    file.deleteBlock(block.id());
    file.deleteSection(section.id());
    file.close();
}


void TestMultiTag::testValidate() {
    test_validate(tag);
    // test_validate(tag_fs); // does not work probably due to missing data support
}

void TestMultiTag::test_validate(MultiTag &t) {
    valid::Result result = validate(t);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void TestMultiTag::testId() {
    CPPUNIT_ASSERT(tag.id().size() == 36);
    CPPUNIT_ASSERT(tag_fs.id().size() == 36);
}


void TestMultiTag::testName() {
    CPPUNIT_ASSERT(tag.name() == "tag_one");
    CPPUNIT_ASSERT(tag_fs.name() == "tag_one");
}


void TestMultiTag::testType() {
    test_type(tag);
    test_type(tag_fs);
}

void TestMultiTag::test_type(MultiTag &t) {
    CPPUNIT_ASSERT(t.type() == "test_tag");
    std::string type = util::createId();
    t.type(type);
    CPPUNIT_ASSERT(t.type() == type);
}

void TestMultiTag::testDefinition() {
    test_definition(tag);
    test_definition(tag_fs);
}

void TestMultiTag::test_definition(MultiTag &t) {
    std::string def = util::createId();
    t.definition(def);
    CPPUNIT_ASSERT(*t.definition() == def);
    t.definition(none);
    CPPUNIT_ASSERT(t.definition() == none);
}


void TestMultiTag::testCreateRemove() {
    test_create_remove(block, positions);
    test_create_remove(block_fs, positions_fs);
}

void TestMultiTag::test_create_remove(Block &b, DataArray &p) {
    std::vector<std::string> ids;
    //issue #473
    ndsize_t count = static_cast<size_t>(b.multiTagCount());
    const char *names[5] = { "tag_a", "tag_b", "tag_c", "tag_d", "tag_e" };
    for (int i = 0; i < 5; i++) {
        std::string type = "Event";
        MultiTag dt1 = b.createMultiTag(names[i], type, p);
        MultiTag dt2 = b.getMultiTag(dt1.id());
        ids.push_back(dt1.id());

        std::stringstream errmsg;
        errmsg << "Error while accessing multiTag: dt1.id() = " << dt1.id()
        << " / dt2.id() = " << dt2.id();
        CPPUNIT_ASSERT_MESSAGE(errmsg.str(), dt1.id().compare(dt2.id()) == 0);
    }
    std::stringstream errmsg2;
    errmsg2 << "Error creating MultiTags. Counts do not match!";
    CPPUNIT_ASSERT_MESSAGE(errmsg2.str(), b.multiTagCount() == (count+5));

    CPPUNIT_ASSERT_THROW(b.createMultiTag(names[4], "test", p), DuplicateName);

    for (size_t i = 0; i < ids.size(); i++) {
        b.deleteMultiTag(ids[i]);
    }
    std::stringstream errmsg1;
    errmsg1 << "Error while removing multiTags!";
    CPPUNIT_ASSERT_MESSAGE(errmsg1.str(), b.multiTagCount() == count);

    DataArray a;
    MultiTag mtag;
    CPPUNIT_ASSERT_THROW(mtag = block.createMultiTag("test", "test", a), nix::UninitializedEntity);
    mtag = block.createMultiTag("test", "test", positions);
    mtag.extents(positions);
    CPPUNIT_ASSERT_THROW(mtag.positions(a), UninitializedEntity);
    CPPUNIT_ASSERT(mtag.extents().id() == positions.id());
    CPPUNIT_ASSERT_NO_THROW(mtag.extents(a));
    CPPUNIT_ASSERT(!mtag.extents());
}


void TestMultiTag::testUnits() {
    test_units(block, positions);
    test_units(block_fs, positions_fs);
}

void TestMultiTag::test_units(Block &b, DataArray &p) {
    MultiTag dt = b.createMultiTag("TestMultiTag1", "Tag", p);

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

    b.deleteMultiTag(dt.id());
}

//TODO merge this test into TestBaseTag::testReferences

void TestMultiTag::testReferences(){
    test_references(block, positions);
    test_references(block_fs, positions_fs);
}

void TestMultiTag::test_references(Block &b, DataArray &pos) {
    DataArray da_1 = b.createDataArray("TestReference 1",
                                       "Reference",
                                       DataType::Double,
                                       NDSize({ 0 }));
    DataArray da_2 = b.createDataArray("TestReference 2", "Reference",
                                       DataType::Double,
                                       NDSize({ 0 }));
    DataArray a;
    MultiTag dt = b.createMultiTag("TestMultiTag1", "Tag", pos);

    CPPUNIT_ASSERT_THROW(dt.getReference(42), OutOfBounds);
    CPPUNIT_ASSERT_THROW(dt.hasReference(a), UninitializedEntity);

    std::stringstream counterrmsg;
    counterrmsg << "TestMultiTag::testReference: Counts do not match!";
    CPPUNIT_ASSERT_MESSAGE(counterrmsg.str(), dt.referenceCount() == 0);

    dt.addReference(da_1);
    dt.addReference(da_2);
    CPPUNIT_ASSERT_THROW(dt.addReference(a), UninitializedEntity);
    CPPUNIT_ASSERT_THROW(dt.removeReference(a), UninitializedEntity);

    CPPUNIT_ASSERT_MESSAGE(counterrmsg.str(), dt.referenceCount() == 2);
    CPPUNIT_ASSERT(dt.hasReference(da_1));
    CPPUNIT_ASSERT(dt.hasReference(da_2));

    std::stringstream haserrmsg;
    haserrmsg << "TestMultiTag::testReference: Has method did not work!";
    CPPUNIT_ASSERT_MESSAGE(haserrmsg.str(), dt.hasReference(da_1.id()));
    CPPUNIT_ASSERT_MESSAGE(haserrmsg.str(), dt.hasReference(da_1.name()));

    DataArray ref1 = dt.getReference(da_1.id());
    std::stringstream retrieveerrmsg;
    retrieveerrmsg << "TestMultiTag::testReference: Retrieval did not work!";
    CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), ref1.id() == da_1.id());
    DataArray ref2 = dt.getReference(da_1.name());
    retrieveerrmsg << "TestMultiTag::testReference: Retrieval by name did not work!";
    CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), ref2.id() == da_1.id());

    std::vector<DataArray> arrays = dt.references();
    CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), arrays.size() == 2);

    std::stringstream hasReferrmsg;
    hasReferrmsg << "TestMultiTag::testReference: hadReference did not work!";
    CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), dt.hasReference(da_1.id()));
    CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), dt.hasReference(da_2.id()));

    std::stringstream delReferrmsg;
    delReferrmsg << "TestMultiTag::testReference: removeReference did not work!";
    dt.removeReference(da_1.id());
    CPPUNIT_ASSERT_MESSAGE(delReferrmsg.str(), dt.referenceCount() == 1);
    dt.removeReference("NONEXISTENT");
    CPPUNIT_ASSERT_MESSAGE(delReferrmsg.str(), dt.referenceCount() == 1);
    dt.removeReference(da_2.name());
    CPPUNIT_ASSERT_MESSAGE(delReferrmsg.str(), dt.referenceCount() == 0);
    dt.addReference(da_1);
    CPPUNIT_ASSERT(dt.referenceCount() == 1);
    CPPUNIT_ASSERT_NO_THROW(dt.removeReference(da_1));
    CPPUNIT_ASSERT(dt.referenceCount() == 0);

    // delete data arrays
    std::vector<std::string> ids = {da_1.id(), da_2.id()};
    b.deleteDataArray(da_1.id());
    b.deleteDataArray(da_2.id());
    // check if references are gone too!
    CPPUNIT_ASSERT(dt.referenceCount() == 0);
    CPPUNIT_ASSERT(!dt.hasReference(ids[0]));
    CPPUNIT_ASSERT(!dt.hasReference(ids[1]));
    b.deleteMultiTag(dt.id());
}


void TestMultiTag::testFeatures() {
    test_features(tag, positions);
    test_features(tag_fs, positions_fs);
}

void TestMultiTag::test_features(MultiTag &mt, DataArray &pos) {
    DataArray a;
    Feature f;

    CPPUNIT_ASSERT(tag.featureCount() == 0);
    CPPUNIT_ASSERT_THROW(tag.hasFeature(f), UninitializedEntity);
    CPPUNIT_ASSERT_THROW(tag.deleteFeature(f), UninitializedEntity);
    CPPUNIT_ASSERT_THROW(tag.createFeature(a, nix::LinkType::Indexed), nix::UninitializedEntity);
    
    CPPUNIT_ASSERT_NO_THROW(f = tag.createFeature(positions, nix::LinkType::Indexed));
    CPPUNIT_ASSERT(mt.hasFeature(f));
    CPPUNIT_ASSERT(tag.featureCount() == 1);
    CPPUNIT_ASSERT_NO_THROW(tag.deleteFeature(f));
    CPPUNIT_ASSERT(tag.featureCount() == 0);
    CPPUNIT_ASSERT(mt.featureCount() == 1);
}


void TestMultiTag::testExtents(){
    test_extents(tag, positions, extents, wrongArray);
    test_extents(tag_fs, positions_fs, extents_fs, wrongArray);
}

void TestMultiTag::test_extents(nix::MultiTag &mt, DataArray &pos, DataArray &ext, DataArray &wrongArray) {
    CPPUNIT_ASSERT_THROW(mt.extents("wrong_data_array_id"), std::runtime_error);
    CPPUNIT_ASSERT_THROW(mt.extents(""), EmptyString);
    CPPUNIT_ASSERT_THROW(mt.extents(wrongArray), std::runtime_error);

    mt.positions(pos);
    mt.extents(ext);
    CPPUNIT_ASSERT(mt.extents() == true);
    mt.extents(none);
    CPPUNIT_ASSERT(mt.extents() == false);
}


void TestMultiTag::testPositions() {
    test_positions(block, tag, positions);
    test_positions(block_fs, tag_fs, positions_fs);
}

void TestMultiTag::test_positions(Block &b, MultiTag &mt, DataArray &pos) {
    CPPUNIT_ASSERT_THROW(mt.positions("wrong_data_array_id"), std::runtime_error);
    CPPUNIT_ASSERT_THROW(mt.positions(""), EmptyString);

    mt.positions(pos);
    CPPUNIT_ASSERT(mt.positions().id() == pos.id());
    CPPUNIT_ASSERT(mt.hasPositions());
    b.deleteDataArray(pos.id());
    // make sure link is gone with data array
    CPPUNIT_ASSERT_THROW(mt.positions(), std::runtime_error);
}

void TestMultiTag::testPositionExtents() {
    tag.extents(extents);
    CPPUNIT_ASSERT(tag.extents().id() == extents.id());
    block.deleteDataArray(extents.id());
    // make sure that link is gone with data array
    CPPUNIT_ASSERT(!tag.extents());
    // re-create extents
    extents = block.createDataArray("extents_DataArray", "dataArray",
                                    DataType::Double, {0, 0});
    typedef boost::multi_array<double, 2> array_type;
    typedef array_type::index index;
    array_type B(boost::extents[5][5]);
    for(index i = 0; i < 5; ++i){
        B[i][i] = 100.0*i;
    }
    extents.setData(B);

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


void TestMultiTag::testDataAccess() {
    DataArray data_array = block.createDataArray("dimensionTest",
                                       "test",
                                       DataType::Double,
                                       NDSize({0, 0, 0}));
    double samplingInterval = 1.0;
    std::vector<double> ticks {1.2, 2.3, 3.4, 4.5, 6.7};
    std::string unit = "ms";

    typedef boost::multi_array<double, 3> array_type;
    typedef array_type::index index;
    array_type data(boost::extents[2][10][5]);
    int value;
    for(index i = 0; i != 2; ++i) {
        value = 0;
        for(index j = 0; j != 10; ++j) {
            for(index k = 0; k != 5; ++k) {
                data[i][j][k] = value++;
            }
        }
    }
    data_array.setData(data);

    SetDimension setDim = data_array.appendSetDimension();
    std::vector<std::string> labels = {"label_a", "label_b"};
    setDim.labels(labels);

    SampledDimension sampledDim = data_array.appendSampledDimension(samplingInterval);
    sampledDim.unit(unit);

    RangeDimension rangeDim = data_array.appendRangeDimension(ticks);
    rangeDim.unit(unit);

    typedef boost::multi_array<double, 2> position_type;
    position_type event_positions(boost::extents[2][3]);
    position_type event_extents(boost::extents[2][3]);
    event_positions[0][0] = 0.0;
    event_positions[0][1] = 3.0;
    event_positions[0][2] = 3.4;

    event_extents[0][0] = 0.0;
    event_extents[0][1] = 6.0;
    event_extents[0][2] = 2.3;

    event_positions[1][0] = 0.0;
    event_positions[1][1] = 8.0;
    event_positions[1][2] = 2.3;

    event_extents[1][0] = 0.0;
    event_extents[1][1] = 3.0;
    event_extents[1][2] = 2.0;

    std::vector<std::string> event_labels = {"event 1", "event 2"};
    std::vector<std::string> dim_labels = {"dim 0", "dim 1", "dim 2"};

    DataArray event_array = block.createDataArray("positions", "test",
                                                  DataType::Double, NDSize({ 0, 0 }));
    event_array.setData(event_positions);
    SetDimension event_set_dim;
    event_set_dim = event_array.appendSetDimension();
    event_set_dim.labels(event_labels);
    event_set_dim = event_array.appendSetDimension();
    event_set_dim.labels(dim_labels);

    DataArray extent_array = block.createDataArray("extents", "test",
                                                   DataType::Double, NDSize({ 0, 0 }));
    extent_array.setData(event_extents);
    SetDimension extent_set_dim;
    extent_set_dim = extent_array.appendSetDimension();
    extent_set_dim.labels(event_labels);
    extent_set_dim = extent_array.appendSetDimension();
    extent_set_dim.labels(dim_labels);

    MultiTag multi_tag = block.createMultiTag("multi_tag", "events", event_array);
    multi_tag.extents(extent_array);
    multi_tag.addReference(data_array);

    CPPUNIT_ASSERT_THROW(multi_tag.retrieveData(0, -1), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(multi_tag.retrieveData(0, 1), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(multi_tag.retrieveData(-1, 0), nix::OutOfBounds);
    CPPUNIT_ASSERT_THROW(multi_tag.retrieveData(10, 0), nix::OutOfBounds);

    DataView ret_data = multi_tag.retrieveData(0, 0);
    NDSize data_size = ret_data.dataExtent();
    CPPUNIT_ASSERT(data_size.size() == 3);
    CPPUNIT_ASSERT(data_size[0] == 1 && data_size[1] == 6 && data_size[2] == 2);

    CPPUNIT_ASSERT_THROW(multi_tag.retrieveData(1, 0), nix::OutOfBounds);
    
    block.deleteMultiTag(multi_tag);
    block.deleteDataArray(data_array);
    block.deleteDataArray(event_array);
    block.deleteDataArray(event_array);
}


void TestMultiTag::testMetadataAccess() {
    test_metadata_access(file, tag, section);
    test_metadata_access(file_fs, tag_fs, section_fs);
}

void TestMultiTag::test_metadata_access(File &f, MultiTag &t, Section &s) {
    CPPUNIT_ASSERT(!t.metadata());
    t.metadata(s);
    CPPUNIT_ASSERT(t.metadata());
    CPPUNIT_ASSERT(t.metadata().id() == s.id());

    // test none-unsetter
    t.metadata(none);
    CPPUNIT_ASSERT(!t.metadata());
    // test deleter removing link too
    t.metadata(s);
    f.deleteSection(s.id());
    CPPUNIT_ASSERT(!t.metadata());
    // re-create section
    s = f.createSection("foo_section", "metadata");
}

void TestMultiTag::testSourceAccess(){
    test_source_access(block, tag);
    test_source_access(block_fs, tag_fs);
}

void TestMultiTag::test_source_access(Block &b, MultiTag &t) {
    std::vector<std::string> names = { "source_a", "source_b", "source_c", "source_d", "source_e" };
    CPPUNIT_ASSERT(t.sourceCount() == 0);
    CPPUNIT_ASSERT(t.sources().size() == 0);

    std::vector<std::string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        Source child_source = b.createSource(*it,"channel");
        t.addSource(child_source);
        CPPUNIT_ASSERT(child_source.name() == *it);
        ids.push_back(child_source.id());
    }

    CPPUNIT_ASSERT(t.sourceCount() == names.size());
    CPPUNIT_ASSERT(t.sources().size() == names.size());

    std::string name = names[0];
    Source source = t.getSource(name);
    CPPUNIT_ASSERT(source.name() == name);

    for (auto it = ids.begin(); it != ids.end(); it++) {
        Source child_source = t.getSource(*it);
        CPPUNIT_ASSERT(t.hasSource(*it) == true);
        CPPUNIT_ASSERT(child_source.id() == *it);

        t.removeSource(*it);
        b.deleteSource(*it);
    }

    CPPUNIT_ASSERT(t.sourceCount() == 0);
    CPPUNIT_ASSERT(t.sources().size() == 0);
}


void TestMultiTag::testOperators() {
    test_operators(tag, tag_other, tag_null);
    test_operators(tag_fs, tag_other_fs, tag_null);
}

void TestMultiTag::test_operators(MultiTag &t, MultiTag &other, MultiTag &null) {
    CPPUNIT_ASSERT(null == false);
    CPPUNIT_ASSERT(null == none);

    CPPUNIT_ASSERT(t != false);
    CPPUNIT_ASSERT(t != none);

    CPPUNIT_ASSERT(t == t);
    CPPUNIT_ASSERT(t != other);

    other = t;
    CPPUNIT_ASSERT(t == other);

    other = none;
    CPPUNIT_ASSERT(other == false);
    CPPUNIT_ASSERT(other == none);

    std::stringstream str1, str2;
    str1 << "MultiTag: {name = " << t.name();
    str1 << ", type = " << t.type();
    str1 << ", id = " << t.id() << "}";
    str2 << t;
    CPPUNIT_ASSERT(str1.str() == str2.str());
}


void TestMultiTag::testCreatedAt() {
    CPPUNIT_ASSERT(tag.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    tag.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(tag.createdAt() == past_time);
}


void TestMultiTag::testUpdatedAt() {
    CPPUNIT_ASSERT(tag.updatedAt() >= startup_time);
}
