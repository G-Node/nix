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

namespace test {
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

    bool isValidOptional(bool isOptional, bool isSet, bool acceptsNoneT) {
        return (isOptional && acceptsNoneT);
    }

    bool isValidObligatory(bool isOptional, bool isSet, bool acceptsNoneT) {
        return (!isOptional && !acceptsNoneT && isSet);
    }
} // namespace test

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
    simpleTag = block.createSimpleTag("featureTest", "Test", refs);

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

    // feature
    feature = simpleTag.createFeature(dataArray, nix::LinkType::Tagged);
}

void TestOptionalObligatory::tearDown() {
    file.close();
}

void TestOptionalObligatory::testNamedEntityId() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::base::NamedEntity<base::IBlock>, test::id>::value;
    // using SFINAE if-else block instead of normal one to avoid "unreachable code" warnings
    // equivalent: std::is_class<decltype(block.id())>::value ? acceptsNoneT : util::is_optional<decltype(block.id());
    isOptional   = std::conditional<std::is_class<decltype(block.id())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(block.id())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(block.id()));
    summarize("NamedEntity::id", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testNamedEntityType() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::base::NamedEntity<base::IBlock>, test::type>::value;
    isOptional   = std::conditional<std::is_class<decltype(block.type())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(block.type())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(block.type()));
    summarize("NamedEntity::type", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testNamedEntityName() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::base::NamedEntity<base::IBlock>, test::name>::value;
    isOptional   = std::conditional<std::is_class<decltype(block.name())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(block.name())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(block.name()));
    summarize("NamedEntity::name", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testNamedEntityDefinition() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::base::NamedEntity<base::IBlock>, test::definition>::value;
    isOptional   = std::conditional<std::is_class<decltype(block.definition())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(block.definition())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(block.definition()));
    summarize("NamedEntity::definition", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testEntityWithMetadataMetadata() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::base::EntityWithMetadata<base::IBlock>, test::metadata>::value;
    isOptional   = std::conditional<std::is_class<decltype(block.metadata())>::value,
                                    std::integral_constant<bool, acceptsNoneT>,
                                    std::integral_constant<bool, util::is_optional<decltype(block.metadata())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(block.metadata()));
    summarize("EntityWithMetadata::metadata", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) ||
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testEntityWithSourcesSources() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::base::EntityWithSources<base::IBlock>, test::sources>::value;
    isOptional   = std::conditional<std::is_class<decltype(block.sources())>::value,
                                    std::integral_constant<bool, acceptsNoneT>,
                                    std::integral_constant<bool, util::is_optional<decltype(block.sources())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(block.sources()));
    summarize("EntityWithSources::sources", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) ||
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testDataArrayLabel() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::DataArray, test::label>::value;
    isOptional   = std::conditional<std::is_class<decltype(dataArray.label())>::value,
                                    std::integral_constant<bool, acceptsNoneT>,
                                    std::integral_constant<bool, util::is_optional<decltype(dataArray.label())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(dataArray.label()));
    summarize("DataArray::label", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) ||
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testDataArrayUnit() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::DataArray, test::unit>::value;
    isOptional   = std::conditional<std::is_class<decltype(dataArray.unit())>::value,
                                    std::integral_constant<bool, acceptsNoneT>,
                                    std::integral_constant<bool, util::is_optional<decltype(dataArray.unit())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(dataArray.unit()));
    summarize("DataArray::unit", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) ||
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testDataArrayExpansionOrigin() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::DataArray, test::expansionOrigin>::value;
    isOptional   = std::conditional<std::is_class<decltype(dataArray.expansionOrigin())>::value,
                                    std::integral_constant<bool, acceptsNoneT>,
                                    std::integral_constant<bool, util::is_optional<decltype(dataArray.expansionOrigin())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(dataArray.expansionOrigin()));
    summarize("DataArray::expansionOrigin", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) ||
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSetDimensionIndex() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::SetDimension, test::index>::value;
    isOptional   = std::conditional<std::is_class<decltype(setDim.index())>::value,
                                    std::integral_constant<bool, acceptsNoneT>,
                                    std::integral_constant<bool, util::is_optional<decltype(setDim.index())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(setDim.index()));
    summarize("SetDimension::index", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) ||
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSetDimensionLabels() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::SetDimension, test::labels>::value;
    isOptional   = std::conditional<std::is_class<decltype(setDim.labels())>::value,
                                    std::integral_constant<bool, acceptsNoneT>,
                                    std::integral_constant<bool, util::is_optional<decltype(setDim.labels())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(setDim.labels()));
    summarize("SetDimension::labels", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) ||
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSampledDimensionIndex() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::SampledDimension, test::index>::value;
    isOptional   = std::conditional<std::is_class<decltype(sampledDim.index())>::value,
                                    std::integral_constant<bool, acceptsNoneT>,
                                    std::integral_constant<bool, util::is_optional<decltype(sampledDim.index())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(sampledDim.index()));
    summarize("SampledDimension::index", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) ||
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSampledDimensionLabel() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::SampledDimension, test::label>::value;
    isOptional   = std::conditional<std::is_class<decltype(sampledDim.label())>::value,
                                    std::integral_constant<bool, acceptsNoneT>,
                                    std::integral_constant<bool, util::is_optional<decltype(sampledDim.label())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(sampledDim.label()));
    summarize("SampledDimension::label", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) ||
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSampledDimensionOffset() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::SampledDimension, test::offset>::value;
    isOptional   = std::conditional<std::is_class<decltype(sampledDim.offset())>::value,
                                    std::integral_constant<bool, acceptsNoneT>,
                                    std::integral_constant<bool, util::is_optional<decltype(sampledDim.offset())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(sampledDim.offset()));
    summarize("SampledDimension::offset", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) ||
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSampledDimensionSamplingInterval() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::SampledDimension, test::samplingInterval>::value;
    isOptional   = std::conditional<std::is_class<decltype(sampledDim.samplingInterval())>::value,
                                    std::integral_constant<bool, acceptsNoneT>,
                                    std::integral_constant<bool, util::is_optional<decltype(sampledDim.samplingInterval())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(sampledDim.samplingInterval()));
    summarize("SampledDimension::samplingInterval", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) ||
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSampledDimensionUnit() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::SampledDimension, test::unit>::value;
    isOptional   = std::conditional<std::is_class<decltype(sampledDim.unit())>::value,
                                    std::integral_constant<bool, acceptsNoneT>,
                                    std::integral_constant<bool, util::is_optional<decltype(sampledDim.unit())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(sampledDim.unit()));
    summarize("SampledDimension::unit", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) ||
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testRangeDimensionIndex() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::RangeDimension, test::index>::value;
    isOptional   = std::conditional<std::is_class<decltype(rangeDim.index())>::value,
                                    std::integral_constant<bool, acceptsNoneT>,
                                    std::integral_constant<bool, util::is_optional<decltype(rangeDim.index())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(rangeDim.index()));
    summarize("RangeDimension::index", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) ||
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testRangeDimensionLabel() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::RangeDimension, test::label>::value;
    isOptional   = std::conditional<std::is_class<decltype(rangeDim.label())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(rangeDim.label())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(rangeDim.label()));
    summarize("RangeDimension::label", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testRangeDimensionTicks() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::RangeDimension, test::ticks>::value;
    isOptional   = std::conditional<std::is_class<decltype(rangeDim.ticks())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(rangeDim.ticks())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(rangeDim.ticks()));
    summarize("RangeDimension::ticks", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testRangeDimensionUnit() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::RangeDimension, test::unit>::value;
    isOptional   = std::conditional<std::is_class<decltype(rangeDim.unit())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(rangeDim.unit())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(rangeDim.unit()));
    summarize("RangeDimension::unit", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testPropertyMapping() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::Property, test::mapping>::value;
    isOptional   = std::conditional<std::is_class<decltype(property.mapping())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(property.mapping())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(property.mapping()));
    summarize("Property::mapping", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testPropertyUnit() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::Property, test::unit>::value;
    isOptional   = std::conditional<std::is_class<decltype(property.unit())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(property.unit())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(property.unit()));
    summarize("Property::unit", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testPropertyValues() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::Property, test::values>::value;
    isOptional   = std::conditional<std::is_class<decltype(property.values())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(property.values())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(property.values()));
    summarize("Property::values", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testFeatureData() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::Feature, test::data>::value;
    isOptional   = std::conditional<std::is_class<decltype(feature.data())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(feature.data())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(feature.data()));
    summarize("Feature::data", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testFeatureLinkType() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::Feature, test::linkType>::value;
    isOptional   = std::conditional<std::is_class<decltype(feature.linkType())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(feature.linkType())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(feature.linkType()));
    summarize("Feature::linkType", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSectionLink() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::Section, test::link>::value;
    isOptional   = std::conditional<std::is_class<decltype(section.link())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(section.link())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(section.link()));
    summarize("Section::link", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSectionMapping() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::Section, test::mapping>::value;
    isOptional   = std::conditional<std::is_class<decltype(section.mapping())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(section.mapping())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(section.mapping()));
    summarize("Mapping::mapping", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSectionRepository() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::Section, test::repository>::value;
    isOptional   = std::conditional<std::is_class<decltype(section.repository())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(section.repository())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(section.repository()));
    summarize("Repository::repository", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSimpleTagExtent() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::SimpleTag, test::extent>::value;
    isOptional   = std::conditional<std::is_class<decltype(simpleTag.extent())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(simpleTag.extent())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(simpleTag.extent()));
    summarize("SimpleTag::extent", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSimpleTagPosition() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::SimpleTag, test::position>::value;
    isOptional   = std::conditional<std::is_class<decltype(simpleTag.position())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(simpleTag.position())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(simpleTag.position()));
    summarize("SimpleTag::position", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testSimpleTagUnits() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::SimpleTag, test::units>::value;
    isOptional   = std::conditional<std::is_class<decltype(simpleTag.units())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(simpleTag.units())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(simpleTag.units()));
    summarize("SimpleTag::units", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testDataTagExtents() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::DataTag, test::extents>::value;
    isOptional   = std::conditional<std::is_class<decltype(dataTag.extents())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(dataTag.extents())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(dataTag.extents()));
    summarize("DataTag::extents", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}
void TestOptionalObligatory::testDataTagPositions() {
    static const bool acceptsNoneT = test::accepts_noneT<nix::DataTag, test::positions>::value;
    isOptional   = std::conditional<std::is_class<decltype(dataTag.positions())>::value, 
                                    std::integral_constant<bool, acceptsNoneT>, 
                                    std::integral_constant<bool, util::is_optional<decltype(dataTag.positions())>::value>
                                    >::type::value;
    isSet        = test::TtoBool(util::deRef(dataTag.positions()));
    summarize("DataTag::positions", isOptional, isSet, acceptsNoneT);
    CPPUNIT_ASSERT(test::isValidOptional(isOptional, isSet, acceptsNoneT) || 
                   test::isValidObligatory(isOptional, isSet, acceptsNoneT));
}


void TestOptionalObligatory::summarize(std::string name, bool isOptional, bool isSet, bool acceptsNoneT)
{
    summary += name; summary += ": ";
    if(test::isValidOptional(isOptional, isSet, acceptsNoneT)) {
        summary += "OPTIONAL";
    }
    else if(test::isValidObligatory(isOptional, isSet, acceptsNoneT)) {
        summary += "OBLIGATORY";
    }
    else {
        summary += "FAIL"; summary += "\n    isOptional:"; summary += util::numToStr(isOptional);
        summary += "\n    isSet:"; summary += util::numToStr(isSet); summary += "\n    acceptsNoneT:";
        summary += util::numToStr(acceptsNoneT);
    }
    summary += "\n";
}

