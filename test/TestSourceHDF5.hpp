// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TESTSOURCEHDF5_HPP
#define NIX_TESTSOURCEHDF5_HPP

#include "BaseTestSource.hpp"

class TestSourceHDF5 : public BaseTestSource {


    CPPUNIT_TEST_SUITE(TestSourceHDF5);

    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testType);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testDefinition);

    CPPUNIT_TEST(testMetadataAccess);
    CPPUNIT_TEST(testSourceAccess);
    CPPUNIT_TEST(testFindSource);

    CPPUNIT_TEST(testOperators);
    CPPUNIT_TEST(testUpdatedAt);
    CPPUNIT_TEST(testCreatedAt);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {
        startup_time = time(NULL);
        file = nix::File::open("test_source.h5", nix::FileMode::Overwrite);
        block = file.createBlock("block", "dataset");
        section = file.createSection("foo_section", "metadata");

        source = block.createSource("source_one", "channel");
        source_other = block.createSource("source_two", "channel");
        source_null  = nix::none;

        // create a DataArray & a MultiTag
        darray = block.createDataArray("DataArray", "dataArray",
                                       nix::DataType::Double, {0, 0});
        typedef boost::multi_array<double, 2> array_type;
        typedef array_type::index index;
        array_type A(boost::extents[5][5]);
        for(index i = 0; i < 5; ++i){
            A[i][i] = 100.0*i;
        }
        darray.setData(A);
        mtag = block.createMultiTag("tag_one", "test_tag", darray);
    }


    void tearDown() {
        file.close();
    }

};

#endif //NIX_TESTSOURCEHDF5_HPP
