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

#include "BaseTestTag.hpp"
#include <nix/hydra/multiArray.hpp>
#include <nix/hdf5/TagHDF5.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <boost/math/constants/constants.hpp>

using namespace nix;
using namespace valid;


void BaseTestTag::testValidate() {
    valid::Result result = validate(tag);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void BaseTestTag::testId() {
    CPPUNIT_ASSERT(tag.id().size() == 36);
}


void BaseTestTag::testName() {
    CPPUNIT_ASSERT(tag.name() == "tag_one");
}


void BaseTestTag::testType() {
    CPPUNIT_ASSERT(tag.type() == "test_tag");
    std::string type = util::createId();
    tag.type(type);
    CPPUNIT_ASSERT(tag.type() == type);
}


void BaseTestTag::testDefinition() {
    std::string def = util::createId();
    tag.definition(def);
    CPPUNIT_ASSERT(*tag.definition() == def);
    tag.definition(nix::none);
    CPPUNIT_ASSERT(tag.definition() == nix::none);
}


void BaseTestTag::testCreateRemove() {
    std::vector<std::string> ids;
    ndsize_t count = block.tagCount();
    const char *names[5] = { "tag_a", "tag_b", "tag_c", "tag_d", "tag_e" };

    for (int i = 0; i < 5; i++) {
        std::string type = "Event";
        Tag st1 = block.createTag(names[i], type, {0.0, 2.0, 3.4});
        st1.references(refs);
        Tag st2 = block.getTag(st1.id());
        ids.push_back(st1.id());

        std::stringstream errmsg;
        errmsg << "Error while accessing tag: st1.id() = " << st1.id()
        << " / st2.id() = " << st2.id();
        CPPUNIT_ASSERT_MESSAGE(errmsg.str(), st1.id().compare(st2.id()) == 0);
    }
    std::stringstream errmsg2;
    errmsg2 << "Error creating Tags. Counts do not match!";
    CPPUNIT_ASSERT_MESSAGE(errmsg2.str(), block.tagCount() == (count+5));

    for (auto it = refs.begin(); it != refs.end(); it++) {
        block.deleteDataArray((*it).id());
    }
    for (const auto &id : ids) {
        block.deleteTag(id);
    }

    std::stringstream errmsg1;
    errmsg1 << "Error while removing tags!";
    CPPUNIT_ASSERT_MESSAGE(errmsg1.str(), block.tagCount() == count);
}


void BaseTestTag::testExtent() {
    Tag st = block.createTag("TestTag1", "Tag", {0.0, 2.0, 3.4});
    st.references(refs);
    std::vector<double> extent = {1.0, 2.0, 3.0};
    st.extent(extent);

    std::vector<double> retrieved = st.extent();
    CPPUNIT_ASSERT(retrieved.size() == extent.size());
    for(size_t i = 0; i < retrieved.size(); i++){
        CPPUNIT_ASSERT(retrieved[i] == extent[i]);
    }

    st.extent(none);
    CPPUNIT_ASSERT(st.extent().size() == 0);
    for (auto it = refs.begin(); it != refs.end(); it++) {
        block.deleteDataArray((*it).id());
    }
    block.deleteTag(st.id());
}


void BaseTestTag::testPosition() {
    Tag st = block.createTag("TestTag1", "Tag", {0.0, 2.0, 3.4});
    st.references(refs);

    std::vector<double> position = {1.0, 2.0, 3.0};
    std::vector<double> new_position = {2.0};

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
    for (auto it = refs.begin(); it != refs.end(); it++) {
        block.deleteDataArray((*it).id());
    }
    block.deleteTag(st.id());
}


void BaseTestTag::testMetadataAccess() {
    CPPUNIT_ASSERT(!tag.metadata());
    tag.metadata(section);
    CPPUNIT_ASSERT(tag.metadata());
    // TODO This test fails due to operator== of Section
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


void BaseTestTag::testSourceAccess() {
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
}


void BaseTestTag::testUnits() {
    Tag st = block.createTag("TestTag1", "Tag", {0.0, 2.0, 3.4});
    st.references(refs);

    std::vector<std::string> valid_units = {"mV", "cm", "m^2"};
    std::vector<std::string> invalid_units = {"mV", "haha", "qm^2"};
    std::vector<std::string> insane_units = {"muV ", " muS"};

    CPPUNIT_ASSERT_NO_THROW(st.units(valid_units));
    CPPUNIT_ASSERT(st.units().size() == valid_units.size());
    std::vector<std::string> retrieved_units = st.units();
    for(size_t i = 0; i < retrieved_units.size(); i++){
        CPPUNIT_ASSERT(retrieved_units[i] == valid_units[i]);
    }

    st.units(none);
    CPPUNIT_ASSERT(st.units().size() == 0);
    CPPUNIT_ASSERT_THROW(st.units(invalid_units), nix::InvalidUnit);
    CPPUNIT_ASSERT(st.units().size() == 0);
    for (auto it = refs.begin(); it != refs.end(); it++) {
        block.deleteDataArray((*it).id());
    }

    st.units(insane_units);
    retrieved_units = st.units();
    CPPUNIT_ASSERT(retrieved_units.size() == 2);
    CPPUNIT_ASSERT(retrieved_units[0] == "uV");
    CPPUNIT_ASSERT(retrieved_units[1] == "uS");

    block.deleteTag(st.id());
}


void BaseTestTag::testReferences() {
    CPPUNIT_ASSERT(tag.referenceCount() == 0);
    for (size_t i = 0; i < refs.size(); ++i) {
        CPPUNIT_ASSERT(!tag.hasReference(refs[i]));
        CPPUNIT_ASSERT_NO_THROW(tag.addReference(refs[i]));
        CPPUNIT_ASSERT(tag.hasReference(refs[i]));
    }
    CPPUNIT_ASSERT(tag.referenceCount() == refs.size());
    for (size_t i = 0; i < refs.size(); ++i) {
        CPPUNIT_ASSERT(tag.removeReference(refs[i]));
    }
    CPPUNIT_ASSERT(tag.referenceCount() == 0);
    DataArray a;
    CPPUNIT_ASSERT(!tag.hasReference(a));
    CPPUNIT_ASSERT_THROW(tag.addReference(a), UninitializedEntity);
    CPPUNIT_ASSERT(!tag.removeReference(a));
}


void BaseTestTag::testFeatures() {
    DataArray a;
    Feature f;

    CPPUNIT_ASSERT(tag.featureCount() == 0);
    CPPUNIT_ASSERT(!tag.hasFeature(f));
    CPPUNIT_ASSERT(!tag.deleteFeature(f));
    CPPUNIT_ASSERT_THROW(tag.createFeature(a, nix::LinkType::Indexed), UninitializedEntity);
    CPPUNIT_ASSERT_NO_THROW(f = tag.createFeature(refs[0], nix::LinkType::Indexed));
    CPPUNIT_ASSERT(tag.hasFeature(f));
    CPPUNIT_ASSERT(tag.featureCount() == 1);
    CPPUNIT_ASSERT(tag.deleteFeature(f));
    CPPUNIT_ASSERT(tag.featureCount() == 0);
}


void BaseTestTag::testDataAccess() {
    double samplingInterval = 1.0;
    std::vector<double> ticks {1.2, 2.3, 3.4, 4.5, 6.7};
    std::string unit = "ms";
    SampledDimension sampledDim;
    RangeDimension rangeDim;
    SetDimension setDim;
    std::vector<double> position {0.0, 2.0, 3.4};
    std::vector<double> extent {0.0, 6.0, 2.3};
    std::vector<std::string> units {"none", "ms", "ms"};

    DataArray data_array = block.createDataArray("dimensionTest",
                                                 "test",
                                                 DataType::Double,
                                                 NDSize({0, 0, 0}));
    std::vector<DataArray> reference;
    reference.push_back(data_array);

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

    setDim = data_array.appendSetDimension();
    std::vector<std::string> labels = {"label_a", "label_b"};
    setDim.labels(labels);

    sampledDim = data_array.appendSampledDimension(samplingInterval);
    sampledDim.unit(unit);

    rangeDim = data_array.appendRangeDimension(ticks);
    rangeDim.unit(unit);

    Tag position_tag = block.createTag("position tag", "event", position);
    position_tag.references(reference);
    position_tag.units(units);

    Tag segment_tag = block.createTag("region tag", "segment", position);
    segment_tag.references(reference);
    segment_tag.extent(extent);
    segment_tag.units(units);

    DataView retrieved_data = position_tag.retrieveData(0);
    NDSize data_size = retrieved_data.dataExtent();
    CPPUNIT_ASSERT(data_size.size() == 3);
    CPPUNIT_ASSERT(data_size[0] == 1 && data_size[1] == 1 &&  data_size[2] == 1);

    retrieved_data = segment_tag.retrieveData( 0);
    data_size = retrieved_data.dataExtent();
    CPPUNIT_ASSERT(data_size.size() == 3);
    CPPUNIT_ASSERT(data_size[0] == 1 && data_size[1] == 6 && data_size[2] == 2);

    block.deleteTag(position_tag);
    block.deleteTag(segment_tag);
}


void BaseTestTag::testOperators() {
    CPPUNIT_ASSERT(tag_null == false);
    CPPUNIT_ASSERT(tag_null == none);

    CPPUNIT_ASSERT(tag == tag);
    CPPUNIT_ASSERT(tag != tag_other);

    tag_other = tag;
    CPPUNIT_ASSERT(tag == tag_other);

    tag_other = none;
    CPPUNIT_ASSERT(tag_other == false);
    CPPUNIT_ASSERT(tag_other == none);

    std::stringstream str1, str2;
    str1 <<  "Tag: {name = " << tag.name();
    str1 << ", type = " << tag.type();
    str1 << ", id = " << tag.id() << "}";
    str2 << tag;
    CPPUNIT_ASSERT(str1.str() == str2.str());
}


void BaseTestTag::testCreatedAt() {
    CPPUNIT_ASSERT(tag.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    tag.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(tag.createdAt() == past_time);
}


void BaseTestTag::testUpdatedAt() {
    CPPUNIT_ASSERT(tag.updatedAt() >= startup_time);
}
