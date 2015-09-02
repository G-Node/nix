// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hydra/multiArray.hpp>
#include <nix.hpp>

#include <nix/hdf5/MultiTagHDF5.hpp>
#include <nix/util/util.hpp>
#include <nix/util/dataAccess.hpp>

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>


class TestMultiTag: public CPPUNIT_NS::TestFixture {
private:

    nix::File file, file_fs;
    nix::Block block, block_fs;
    nix::DataArray positions, extents, positions_fs, extents_fs, wrongArray;
    nix::MultiTag tag, tag_other, tag_null, tag_fs, tag_other_fs;
    nix::Section section, section_fs;
    time_t startup_time;


    CPPUNIT_TEST_SUITE(TestMultiTag);
    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testType);
    CPPUNIT_TEST(testDefinition);
    CPPUNIT_TEST(testCreateRemove);
    CPPUNIT_TEST(testExtents);
    CPPUNIT_TEST(testUnits);
    CPPUNIT_TEST(testPositions);
    CPPUNIT_TEST(testPositionExtents);
    CPPUNIT_TEST(testReferences);
    CPPUNIT_TEST(testFeatures);
    CPPUNIT_TEST(testDataAccess);
    CPPUNIT_TEST(testMetadataAccess);
    CPPUNIT_TEST(testSourceAccess);
    CPPUNIT_TEST(testCreatedAt);
    CPPUNIT_TEST(testUpdatedAt);
    CPPUNIT_TEST(testOperators);
    CPPUNIT_TEST_SUITE_END ();

    void test_type(nix::MultiTag &tag);
    void test_validate(nix::MultiTag &tag);
    void test_definition(nix::MultiTag &tag);
    void test_units(nix::Block &b, nix::DataArray &p);
    void test_create_remove(nix::Block &b, nix::DataArray &p);
    void test_positions(nix::Block &b, nix::MultiTag &mt, nix::DataArray &pos);
    void test_extents(nix::MultiTag &tag, nix::DataArray &pos, nix::DataArray &ext, nix::DataArray &wrong);
    void test_references(nix::Block &b, nix::DataArray &pos);
    void test_features(nix::MultiTag &mt, nix::DataArray &pos);
    void test_metadata_access(nix::File &f, nix::MultiTag &t, nix::Section &s);
    void test_source_access(nix::Block &b, nix::MultiTag &t);

public:

    void setUp();
    void tearDown();
    void testValidate();
    void testId();
    void testName();
    void testType();
    void testDefinition();
    void testCreateRemove();
    void testExtents();
    void testUnits();
    void testPositions();
    void testReferences();
    void testFeatures();
    void testDataAccess();
    void testPositionExtents();
    void testMetadataAccess();
    void testSourceAccess();
    void testOperators();
    void testCreatedAt();
    void testUpdatedAt();
};
