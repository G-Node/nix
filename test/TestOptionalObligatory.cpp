// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>
#include <typeinfo>
#include <boost/any.hpp>

#include <nix/util/util.hpp>
#include "TestOptionalObligatory.hpp"

using namespace std;
using namespace nix;

template<typename T>
bool TtoBool(T var) {
    return (bool)var;
}
template<>
bool TtoBool<std::vector<std::string>>(std::vector<std::string> var) {
    return !(var.empty());
}
template<>
bool TtoBool<std::vector<double>>(std::vector<double> var) {
    return !(var.empty());
}
template<>
bool TtoBool<nix::Section>(nix::Section var) {
    return !(var.isNone());
}
template<>
bool TtoBool<nix::DataArray>(nix::DataArray var) {
    return !(var.isNone());
}
template<>
bool TtoBool<nix::LinkType>(nix::LinkType var) {
    return true; // LinkType as enum is always set
}
template<>
bool TtoBool<std::vector<nix::Value>>(std::vector<nix::Value> var) {
    return !(var.empty());
}
template<>
bool TtoBool<std::vector<nix::DataArray>>(std::vector<nix::DataArray> var) {
    return !(var.empty());
}
template<>
bool TtoBool<std::vector<nix::Source>>(std::vector<nix::Source> var) {
    return !(var.empty());
}
template<>
bool TtoBool<std::string>(std::string var) {
    return (bool)var.length();
}


void TestOptionalObligatory::setUp() {
    startup_time = time(NULL);
    
    // File-------------------------------------------------------------
    file = File::open("test_block.h5", FileMode::Overwrite);
    
    // Section---------------------------------------------------------
    section = file.createSection("foo_section", "metadata");
    
    // Block------------------------------------------------------------
    block = file.createBlock("block_one", "dataset");
    
    // property---------------------------------------------------------
    property = section.createProperty("prop", "sometype");
    
    // dataAray---------------------------------------------------------
    dataArray = block.createDataArray("dimensionTest", "test");
    
    // simpleTag--------------------------------------------------------
    vector<string> array_names = { "data_array_a", "data_array_b", "data_array_c",
                                   "data_array_d", "data_array_e" };
    vector<DataArray> refs;
    for (auto it = array_names.begin(); it != array_names.end(); it++) {
        refs.push_back(block.createDataArray(*it, "reference"));
    }
    simpleTag = block.createSimpleTag("representationTest", "Test", refs);
    
    // dataTag----------------------------------------------------------
    positions = block.createDataArray("positions_DataArray", "dataArray");
    typedef boost::multi_array<double, 1> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[5]);
    for(index i = 0; i < 5; ++i){
        A[i] = 100.0*i;
    }
    positions.setData(A);
    dataTag = block.createDataTag("tag_one", "test_tag", positions);
    
    // source-----------------------------------------------------------
    source = block.createSource("source_one", "channel");
    
    // setDimension-----------------------------------------------------
    setDim = dataArray.appendSetDimension();
    
    // sampledDimension-------------------------------------------------
    double samplingInterval = boost::math::constants::pi<double>();
    sampledDim = dataArray.appendSampledDimension(samplingInterval);
    
    // rangeDimension---------------------------------------------------
    std::vector<double> ticks;
    for (size_t i = 0; i < 5; i++) {
        ticks.push_back(i * boost::math::constants::pi<double>());
    }
    rangeDim = dataArray.appendRangeDimension(ticks);
    
    // representation
    rep = simpleTag.createRepresentation(dataArray, nix::LinkType::Tagged);
}

void TestOptionalObligatory::tearDown() {
    file.close();
}

void TestOptionalObligatory::testNamedEntityId() {
    acceptsNoneT = util::accepts_noneT<nix::base::NamedEntity<base::IBlock>, util::T_FN::id>::value;
    isOptional   = std::is_class<decltype(block.id())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(block.id())>::value;
    isSet        = TtoBool(util::deRef(block.id()));
    summarize("NamedEntity::id", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testNamedEntityType() {
    acceptsNoneT = util::accepts_noneT<nix::base::NamedEntity<base::IBlock>, util::T_FN::type>::value;
    isOptional   = std::is_class<decltype(block.type())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(block.type())>::value;
    isSet        = TtoBool(util::deRef(block.type()));
    summarize("NamedEntity::type", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testNamedEntityName() {
    acceptsNoneT = util::accepts_noneT<nix::base::NamedEntity<base::IBlock>, util::T_FN::name>::value;
    isOptional   = std::is_class<decltype(block.name())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(block.name())>::value;
    isSet        = TtoBool(util::deRef(block.name()));
    summarize("NamedEntity::name", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testNamedEntityDefinition() {
    acceptsNoneT = util::accepts_noneT<nix::base::NamedEntity<base::IBlock>, util::T_FN::definition>::value;
    isOptional   = std::is_class<decltype(block.definition())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(block.definition())>::value;
    isSet        = TtoBool(util::deRef(block.definition()));
    summarize("NamedEntity::definition", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testEntityWithMetadataMetadata() {
    acceptsNoneT = util::accepts_noneT<nix::base::EntityWithMetadata<base::IBlock>, util::T_FN::metadata>::value;
    isOptional   = std::is_class<decltype(block.metadata())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(block.metadata())>::value;
    isSet        = TtoBool(util::deRef(block.metadata()));
    summarize("EntityWithMetadata::metadata", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testEntityWithSourcesSources() {
    acceptsNoneT = util::accepts_noneT<nix::base::EntityWithSources<base::IBlock>, util::T_FN::sources>::value;
    isOptional   = std::is_class<decltype(block.sources())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(block.sources())>::value;
    isSet        = TtoBool(util::deRef(block.sources()));
    summarize("EntityWithSources::sources", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testDataArrayLabel() {
    acceptsNoneT = util::accepts_noneT<nix::DataArray, util::T_FN::label>::value;
    isOptional   = std::is_class<decltype(dataArray.label())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(dataArray.label())>::value;
    isSet        = TtoBool(util::deRef(dataArray.label()));
    summarize("DataArray::label", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testDataArrayUnit() {
    acceptsNoneT = util::accepts_noneT<nix::DataArray, util::T_FN::unit>::value;
    isOptional   = std::is_class<decltype(dataArray.unit())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(dataArray.unit())>::value;
    isSet        = TtoBool(util::deRef(dataArray.unit()));
    summarize("DataArray::unit", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testDataArrayExpansionOrigin() {
    acceptsNoneT = util::accepts_noneT<nix::DataArray, util::T_FN::expansionOrigin>::value;
    isOptional   = std::is_class<decltype(dataArray.expansionOrigin())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(dataArray.expansionOrigin())>::value;
    isSet        = TtoBool(util::deRef(dataArray.expansionOrigin()));
    summarize("DataArray::expansionOrigin", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSetDimensionId() {
    acceptsNoneT = util::accepts_noneT<nix::SetDimension, util::T_FN::id>::value;
    isOptional   = std::is_class<decltype(setDim.id())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(setDim.id())>::value;
    isSet        = TtoBool(util::deRef(setDim.id()));
    summarize("SetDimension::id", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSetDimensionLabels() {
    acceptsNoneT = util::accepts_noneT<nix::SetDimension, util::T_FN::labels>::value;
    isOptional   = std::is_class<decltype(setDim.labels())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(setDim.labels())>::value;
    isSet        = TtoBool(util::deRef(setDim.labels()));
    summarize("SetDimension::labels", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSampledDimensionId() {
    acceptsNoneT = util::accepts_noneT<nix::SampledDimension, util::T_FN::id>::value;
    isOptional   = std::is_class<decltype(sampledDim.id())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(sampledDim.id())>::value;
    isSet        = TtoBool(util::deRef(sampledDim.id()));
    summarize("SampledDimension::id", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSampledDimensionLabel() {
    acceptsNoneT = util::accepts_noneT<nix::SampledDimension, util::T_FN::label>::value;
    isOptional   = std::is_class<decltype(sampledDim.label())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(sampledDim.label())>::value;
    isSet        = TtoBool(util::deRef(sampledDim.label()));
    summarize("SampledDimension::label", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSampledDimensionOffset() {
    acceptsNoneT = util::accepts_noneT<nix::SampledDimension, util::T_FN::offset>::value;
    isOptional   = std::is_class<decltype(sampledDim.offset())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(sampledDim.offset())>::value;
    isSet        = TtoBool(util::deRef(sampledDim.offset()));
    summarize("SampledDimension::offset", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSampledDimensionSamplingInterval() {
    acceptsNoneT = util::accepts_noneT<nix::SampledDimension, util::T_FN::samplingInterval>::value;
    isOptional   = std::is_class<decltype(sampledDim.samplingInterval())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(sampledDim.samplingInterval())>::value;
    isSet        = TtoBool(util::deRef(sampledDim.samplingInterval()));
    summarize("SampledDimension::samplingInterval", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSampledDimensionUnit() {
    acceptsNoneT = util::accepts_noneT<nix::SampledDimension, util::T_FN::unit>::value;
    isOptional   = std::is_class<decltype(sampledDim.unit())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(sampledDim.unit())>::value;
    isSet        = TtoBool(util::deRef(sampledDim.unit()));
    summarize("SampledDimension::unit", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testRangeDimensionId() {
    acceptsNoneT = util::accepts_noneT<nix::RangeDimension, util::T_FN::id>::value;
    isOptional   = std::is_class<decltype(rangeDim.id())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(rangeDim.id())>::value;
    isSet        = TtoBool(util::deRef(rangeDim.id()));
    summarize("RangeDimension::id", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testRangeDimensionLabel() {
    acceptsNoneT = util::accepts_noneT<nix::RangeDimension, util::T_FN::label>::value;
    isOptional   = std::is_class<decltype(rangeDim.label())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(rangeDim.label())>::value;
    isSet        = TtoBool(util::deRef(rangeDim.label()));
    summarize("RangeDimension::label", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testRangeDimensionTicks() {
    acceptsNoneT = util::accepts_noneT<nix::RangeDimension, util::T_FN::ticks>::value;
    isOptional   = std::is_class<decltype(rangeDim.ticks())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(rangeDim.ticks())>::value;
    isSet        = TtoBool(util::deRef(rangeDim.ticks()));
    summarize("RangeDimension::ticks", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testRangeDimensionUnit() {
    acceptsNoneT = util::accepts_noneT<nix::RangeDimension, util::T_FN::unit>::value;
    isOptional   = std::is_class<decltype(rangeDim.unit())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(rangeDim.unit())>::value;
    isSet        = TtoBool(util::deRef(rangeDim.unit()));
    summarize("RangeDimension::unit", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testPropertyMapping() {
    acceptsNoneT = util::accepts_noneT<nix::Property, util::T_FN::mapping>::value;
    isOptional   = std::is_class<decltype(property.mapping())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(property.mapping())>::value;
    isSet        = TtoBool(util::deRef(property.mapping()));
    summarize("Property::mapping", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testPropertyUnit() {
    acceptsNoneT = util::accepts_noneT<nix::Property, util::T_FN::unit>::value;
    isOptional   = std::is_class<decltype(property.unit())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(property.unit())>::value;
    isSet        = TtoBool(util::deRef(property.unit()));
    summarize("Property::unit", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testPropertyValues() {
    acceptsNoneT = util::accepts_noneT<nix::Property, util::T_FN::values>::value;
    isOptional   = std::is_class<decltype(property.values())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(property.values())>::value;
    isSet        = TtoBool(util::deRef(property.values()));
    summarize("Property::values", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testRepresentationData() {
    acceptsNoneT = util::accepts_noneT<nix::Representation, util::T_FN::data>::value;
    isOptional   = std::is_class<decltype(rep.data())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(rep.data())>::value;
    isSet        = TtoBool(util::deRef(rep.data()));
    summarize("Representation::data", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testRepresentationLinkType() {
    acceptsNoneT = util::accepts_noneT<nix::Representation, util::T_FN::linkType>::value;
    isOptional   = std::is_class<decltype(rep.linkType())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(rep.linkType())>::value;
    isSet        = TtoBool(util::deRef(rep.linkType()));
    summarize("Representation::linkType", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSectionLink() {
    acceptsNoneT = util::accepts_noneT<nix::Section, util::T_FN::link>::value;
    isOptional   = std::is_class<decltype(section.link())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(section.link())>::value;
    isSet        = TtoBool(util::deRef(section.link()));
    summarize("Section::link", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSectionMapping() {
    acceptsNoneT = util::accepts_noneT<nix::Section, util::T_FN::mapping>::value;
    isOptional   = std::is_class<decltype(section.mapping())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(section.mapping())>::value;
    isSet        = TtoBool(util::deRef(section.mapping()));
    summarize("Mapping::mapping", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSectionRepository() {
    acceptsNoneT = util::accepts_noneT<nix::Section, util::T_FN::repository>::value;
    isOptional   = std::is_class<decltype(section.repository())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(section.repository())>::value;
    isSet        = TtoBool(util::deRef(section.repository()));
    summarize("Repository::repository", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSimpleTagExtent() {
    acceptsNoneT = util::accepts_noneT<nix::SimpleTag, util::T_FN::extent>::value;
    isOptional   = std::is_class<decltype(simpleTag.extent())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(simpleTag.extent())>::value;
    isSet        = TtoBool(util::deRef(simpleTag.extent()));
    summarize("SimpleTag::extent", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSimpleTagPosition() {
    acceptsNoneT = util::accepts_noneT<nix::SimpleTag, util::T_FN::position>::value;
    isOptional   = std::is_class<decltype(simpleTag.position())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(simpleTag.position())>::value;
    isSet        = TtoBool(util::deRef(simpleTag.position()));
    summarize("SimpleTag::position", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSimpleTagUnits() {
    acceptsNoneT = util::accepts_noneT<nix::SimpleTag, util::T_FN::units>::value;
    isOptional   = std::is_class<decltype(simpleTag.units())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(simpleTag.units())>::value;
    isSet        = TtoBool(util::deRef(simpleTag.units()));
    summarize("SimpleTag::units", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testDataTagExtents() {
    acceptsNoneT = util::accepts_noneT<nix::DataTag, util::T_FN::extents>::value;
    isOptional   = std::is_class<decltype(dataTag.extents())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(dataTag.extents())>::value;
    isSet        = TtoBool(util::deRef(dataTag.extents()));
    summarize("DataTag::extents", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testDataTagPositions() {
    acceptsNoneT = util::accepts_noneT<nix::DataTag, util::T_FN::positions>::value;
    isOptional   = std::is_class<decltype(dataTag.positions())>::value ? 
                   acceptsNoneT : 
                   util::is_optional<decltype(dataTag.positions())>::value;
    isSet        = TtoBool(util::deRef(dataTag.positions()));
    summarize("DataTag::positions", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(util::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   util::isValidObligatory(isOptional, isSet, acceptsNoneT));
}


void TestOptionalObligatory::summarize(std::string name, bool isOptional, bool isSet, bool acceptsNoneT)
{
    summary += name; summary += ": ";
    if(util::isValidOptional(isOptional, isSet, acceptsNoneT)) {
        summary += "OPTIONAL";
    }
    else if(util::isValidObligatory(isOptional, isSet, acceptsNoneT)) {
        summary += "OBLIGATORY";
    }
    else {
        summary += "FAIL"; summary += "\n    isOptional:"; summary += util::numToStr(isOptional);
        summary += "\n    isSet:"; summary += util::numToStr(isSet); summary += "\n    acceptsNoneT:";
        summary += util::numToStr(acceptsNoneT);
    }
    summary += "\n";
}

