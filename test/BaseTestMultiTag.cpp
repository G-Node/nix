// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Jan Grewe <jan.grewe@g-node.org>


#include <sstream>
#include <ctime>
#include <iostream>
#include <iterator>
#include <stdexcept>

#include <nix/Exception.hpp>
#include <nix/hydra/multiArray.hpp>
#include <nix/valid/validate.hpp>

#include "BaseTestMultiTag.hpp"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>


using namespace nix;
using namespace valid;


void BaseTestMultiTag::testValidate() {
    valid::Result result = validate(tag);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void BaseTestMultiTag::testId() {
    CPPUNIT_ASSERT(tag.id().size() == 36);
}


void BaseTestMultiTag::testName() {
    CPPUNIT_ASSERT(tag.name() == "tag_one");
}


void BaseTestMultiTag::testType() {
    CPPUNIT_ASSERT(tag.type() == "test_tag");
    std::string type = util::createId();
    tag.type(type);
    CPPUNIT_ASSERT(tag.type() == type);
}


void BaseTestMultiTag::testDefinition() {
    std::string def = util::createId();
    tag.definition(def);
    CPPUNIT_ASSERT(*tag.definition() == def);
    tag.definition(none);
    CPPUNIT_ASSERT(tag.definition() == none);
}


void BaseTestMultiTag::testCreateRemove() {
    std::vector<std::string> ids;
	//issue #473
	ndsize_t count = static_cast<size_t>(block.multiTagCount());
    const char *names[5] = { "tag_a", "tag_b", "tag_c", "tag_d", "tag_e" };
    for (int i = 0; i < 5; i++) {
        std::string type = "Event";
        MultiTag dt1 = block.createMultiTag(names[i], type, positions);
        MultiTag dt2 = block.getMultiTag(dt1.id());
        ids.push_back(dt1.id());

        std::stringstream errmsg;
        errmsg << "Error while accessing multiTag: dt1.id() = " << dt1.id()
               << " / dt2.id() = " << dt2.id();
        CPPUNIT_ASSERT_MESSAGE(errmsg.str(), dt1.id().compare(dt2.id()) == 0);
    }
    std::stringstream errmsg2;
    errmsg2 << "Error creating MultiTags. Counts do not match!";
    CPPUNIT_ASSERT_MESSAGE(errmsg2.str(), block.multiTagCount() == (count+5));

    for (size_t i = 0; i < ids.size(); i++) {
        block.deleteMultiTag(ids[i]);
    }

    std::stringstream errmsg1;
    errmsg1 << "Error while removing multiTags!";
    CPPUNIT_ASSERT_MESSAGE(errmsg1.str(), block.multiTagCount() == count);

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

void BaseTestMultiTag::testUnits() {
    MultiTag dt = block.createMultiTag("TestMultiTag1", "Tag", positions);

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

    block.deleteMultiTag(dt.id());
}

//TODO merge this test into TestBaseTag::testReferences

void BaseTestMultiTag::testReferences(){
    DataArray da_1 = block.createDataArray("TestReference 1",
                                           "Reference",
                                           DataType::Double,
                                           NDSize({ 0 }));
    DataArray da_2 = block.createDataArray("TestReference 2", "Reference",
                                           DataType::Double,
                                           NDSize({ 0 }));
    DataArray a;
    MultiTag dt = block.createMultiTag("TestMultiTag1", "Tag", positions);

    CPPUNIT_ASSERT_THROW(dt.getReference(42), OutOfBounds);
    CPPUNIT_ASSERT(!dt.hasReference(a));

    std::stringstream counterrmsg;
    counterrmsg << "BaseTestMultiTag::testReference: Counts do not match!";
    CPPUNIT_ASSERT_MESSAGE(counterrmsg.str(), dt.referenceCount() == 0);
    
    dt.addReference(da_1);
    dt.addReference(da_2);
    CPPUNIT_ASSERT_THROW(dt.addReference(a), UninitializedEntity);
    CPPUNIT_ASSERT_MESSAGE(counterrmsg.str(), dt.referenceCount() == 2);
    CPPUNIT_ASSERT(dt.hasReference(da_1));
    CPPUNIT_ASSERT(dt.hasReference(da_2));

    std::stringstream haserrmsg;
    haserrmsg << "BaseTestMultiTag::testReference: Has method did not work!";
    CPPUNIT_ASSERT_MESSAGE(haserrmsg.str(), dt.hasReference(da_1.id()));
    CPPUNIT_ASSERT_MESSAGE(haserrmsg.str(), dt.hasReference(da_1.name()));

    DataArray ref1 = dt.getReference(da_1.id());
    std::stringstream retrieveerrmsg;
    retrieveerrmsg << "BaseTestMultiTag::testReference: Retrieval did not work!";
    CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), ref1.id() == da_1.id());
    DataArray ref2 = dt.getReference(da_1.name());
    retrieveerrmsg << "BaseTestMultiTag::testReference: Retrieval by name did not work!";
    CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), ref2.id() == da_1.id());

    std::vector<DataArray> arrays = dt.references();
    CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), arrays.size() == 2);

    std::stringstream hasReferrmsg;
    hasReferrmsg << "BaseTestMultiTag::testReference: hadReference did not work!";
    CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), dt.hasReference(da_1.id()));
    CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), dt.hasReference(da_2.id()));

    std::stringstream delReferrmsg;
    delReferrmsg << "BaseTestMultiTag::testReference: removeReference did not work!";
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
    block.deleteDataArray(da_1.id());
    block.deleteDataArray(da_2.id());
    // check if references are gone too!
    CPPUNIT_ASSERT(dt.referenceCount() == 0);
    CPPUNIT_ASSERT(!dt.hasReference(ids[0]));
    CPPUNIT_ASSERT(!dt.hasReference(ids[1]));
    block.deleteMultiTag(dt.id());
}


void BaseTestMultiTag::testFeatures() {
    DataArray a;
    Feature f;
    CPPUNIT_ASSERT(tag.featureCount() == 0);
    CPPUNIT_ASSERT(!tag.hasFeature(f));
    CPPUNIT_ASSERT(!tag.deleteFeature(f));
    CPPUNIT_ASSERT_THROW(tag.createFeature(a, nix::LinkType::Indexed), nix::UninitializedEntity);
    
    CPPUNIT_ASSERT_NO_THROW(f = tag.createFeature(positions, nix::LinkType::Indexed));
    CPPUNIT_ASSERT(tag.featureCount() == 1);
    CPPUNIT_ASSERT(tag.deleteFeature(f));
    CPPUNIT_ASSERT(tag.featureCount() == 0);
}


void BaseTestMultiTag::testExtents(){
    CPPUNIT_ASSERT_THROW(tag.extents("wrong_data_array_id"), std::runtime_error);
    DataArray a = block.createDataArray("name", "type", DataType::Double, {0,0});
    block.deleteDataArray(a);
    CPPUNIT_ASSERT_THROW(tag.extents(a), UninitializedEntity);
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


void BaseTestMultiTag::testPositions() {
    CPPUNIT_ASSERT_THROW(tag.positions("wrong_data_array_id"), std::runtime_error);

    tag.positions(positions);
    CPPUNIT_ASSERT(tag.positions().id() == positions.id());
    block.deleteDataArray(positions.id());
    // make sure link is gone with data array
    CPPUNIT_ASSERT_THROW(tag.positions(), std::runtime_error);
    
    // re-create positions
    positions = block.createDataArray("positions_DataArray", "dataArray",
                                      DataType::Double, {0, 0});
    typedef boost::multi_array<double, 2> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[5][5]);
    for(index i = 0; i < 5; ++i){
        A[i][i] = 100.0*i;
    }
    positions.setData(A);
}


void BaseTestMultiTag::testPositionExtents() {
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


void BaseTestMultiTag::testDataAccess() {
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


void BaseTestMultiTag::testMetadataAccess() {
    CPPUNIT_ASSERT(!tag.metadata());
    tag.metadata(section);
    CPPUNIT_ASSERT(tag.metadata());
    CPPUNIT_ASSERT(tag.metadata().id() == section.id());
    
    // test none-unsetter
    tag.metadata(none);
    CPPUNIT_ASSERT(!tag.metadata());
    // test deleter removing link too
    tag.metadata(section);
    file.deleteSection(section.id());
    CPPUNIT_ASSERT(!tag.metadata());
    // re-create section
    section = file.createSection("foo_section", "metadata");
}


void BaseTestMultiTag::testSourceAccess(){
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

    std::string name = names[0];
    Source source = tag.getSource(name);
    CPPUNIT_ASSERT(source.name() == name);

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

void BaseTestMultiTag::testOperators() {
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


void BaseTestMultiTag::testCreatedAt() {
    CPPUNIT_ASSERT(tag.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    tag.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(tag.createdAt() == past_time);
}


void BaseTestMultiTag::testUpdatedAt() {
    CPPUNIT_ASSERT(tag.updatedAt() >= startup_time);
}
