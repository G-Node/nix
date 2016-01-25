// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TESTMULTITAGHDF5_HPP
#define NIX_TESTMULTITAGHDF5_HPP

#include "BaseTestMultiTag.hpp"

class TestMultiTagHDF5 : public BaseTestMultiTag {

    CPPUNIT_TEST_SUITE(TestMultiTagHDF5);
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

public:

    void setUp() {
        startup_time = time(NULL);
        file = nix::File::open("test_multiTag.h5", nix::FileMode::Overwrite);
        block = file.createBlock("block", "dataset");

        positions = block.createDataArray("positions_DataArray", "dataArray",
                                          nix::DataType::Double, nix::NDSize({ 0, 0 }));
        extents = block.createDataArray("extents_DataArray", "dataArray",
                                        nix::DataType::Double, nix::NDSize({ 0, 0 }));

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
    }


    void tearDown(){
        file.deleteBlock(block.id());
        file.deleteSection(section.id());
        file.close();
    }
};


#endif //NIX_TESTMULTITAGHDF5_HPP
