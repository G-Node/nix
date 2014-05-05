// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <boost/math/constants/constants.hpp>

#include <nix.hpp>


/**
 * Convert an unknown type to bool - only handles string explicitely
 * and attempts cast to bool an all other types. Useful when type
 * is either bool-convertible or string, but unknown which of the two.
 *
 * @param type   The variable to convert.
 *
 * @return bool
 */
template<typename T>
bool TtoBool(T var);
template<>
bool TtoBool<std::string>(std::string var);


class TestOptionalObligatory : public CPPUNIT_NS::TestFixture {

private:

    CPPUNIT_TEST_SUITE(TestOptionalObligatory);

    CPPUNIT_TEST(testNamedEntityId);
    CPPUNIT_TEST(testNamedEntityType);
    CPPUNIT_TEST(testNamedEntityName);
    CPPUNIT_TEST(testNamedEntityDefinition);
    CPPUNIT_TEST(testEntityWithMetadataMetadata);
    CPPUNIT_TEST(testEntityWithSourcesSources);
    CPPUNIT_TEST(testDataArrayLabel);
    CPPUNIT_TEST(testDataArrayUnit);
    CPPUNIT_TEST(testDataArrayExpansionOrigin);
    CPPUNIT_TEST(testSetDimensionId);
    CPPUNIT_TEST(testSetDimensionLabels);
    CPPUNIT_TEST(testSampledDimensionId);
    CPPUNIT_TEST(testSampledDimensionLabel);
    CPPUNIT_TEST(testSampledDimensionOffset);
    CPPUNIT_TEST(testSampledDimensionSamplingInterval);
    CPPUNIT_TEST(testSampledDimensionUnit);
    CPPUNIT_TEST(testRangeDimensionId);
    CPPUNIT_TEST(testRangeDimensionLabel);
    CPPUNIT_TEST(testRangeDimensionTicks);
    CPPUNIT_TEST(testRangeDimensionUnit);
    CPPUNIT_TEST(testPropertyMapping);
    CPPUNIT_TEST(testPropertyUnit);
    CPPUNIT_TEST(testPropertyValues);
    CPPUNIT_TEST(testRepresentationData);
    CPPUNIT_TEST(testRepresentationLinkType);
    CPPUNIT_TEST(testSectionLink);
    CPPUNIT_TEST(testSectionMapping);
    CPPUNIT_TEST(testSectionRepository);
    CPPUNIT_TEST(testSimpleTagExtent);
    CPPUNIT_TEST(testSimpleTagPosition);
    CPPUNIT_TEST(testSimpleTagUnits);
    CPPUNIT_TEST(testDataTagExtents);
    CPPUNIT_TEST(testDataTagPositions);
    
    CPPUNIT_TEST_SUITE_END ();

    nix::File file;
    nix::Section section;
    nix::Block block;
    nix::Property property;
    nix::DataArray dataArray, positions;
    nix::SimpleTag simpleTag;
    nix::DataTag dataTag;
    nix::Source source;
    nix::SetDimension setDim;
    nix::SampledDimension sampledDim;
    nix::RangeDimension rangeDim;
    nix::Representation rep;
    time_t startup_time;

    bool isOptional;   // whether getter return value is boost::optional
    bool isSet;        // whether getter return value is set
    bool acceptsNoneT; // whether setter accepts boost::none
    
    std::string summary;
    
public:

    void setUp();
    void tearDown();

    void testNamedEntityId();
    void testNamedEntityType();
    void testNamedEntityName();
    void testNamedEntityDefinition();
    void testEntityWithMetadataMetadata();
    void testEntityWithSourcesSources();
    void testDataArrayLabel();
    void testDataArrayUnit();
    void testDataArrayExpansionOrigin();
    void testSetDimensionId();
    void testSetDimensionLabels();
    void testSampledDimensionId();
    void testSampledDimensionLabel();
    void testSampledDimensionOffset();
    void testSampledDimensionSamplingInterval();
    void testSampledDimensionUnit();
    void testRangeDimensionId();
    void testRangeDimensionLabel();
    void testRangeDimensionTicks();
    void testRangeDimensionUnit();
    void testPropertyMapping();
    void testPropertyUnit();
    void testPropertyValues();
    void testRepresentationData();
    void testRepresentationLinkType();
    void testSectionLink();
    void testSectionMapping();
    void testSectionRepository();
    void testSimpleTagExtent();
    void testSimpleTagPosition();
    void testSimpleTagUnits();
    void testDataTagExtents();
    void testDataTagPositions();
    
    void summarize(std::string name, bool isOptional, bool isSet, bool acceptsNoneT);
    
    ~TestOptionalObligatory() {
        std::cout << summary;
    }
};
