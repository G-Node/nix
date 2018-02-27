// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TESTPROPERTYHDF5_HPP
#define NIX_TESTPROPERTYHDF5_HPP

#include "BaseTestProperty.hpp"

class TestPropertyHDF5 : public BaseTestProperty {

    CPPUNIT_TEST_SUITE(TestPropertyHDF5);

    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testDefinition);

    CPPUNIT_TEST(testValues);
    CPPUNIT_TEST(testDataType);
    CPPUNIT_TEST(testUnit);

    CPPUNIT_TEST(testOperators);
    CPPUNIT_TEST(testUpdatedAt);
    CPPUNIT_TEST(testCreatedAt);
    CPPUNIT_TEST(testIsValidEntity);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {
        startup_time = time(NULL);
        file = nix::File::open("test_property.h5", nix::FileMode::Overwrite);
        section = file.createSection("cool section", "metadata");
        int_dummy = nix::Variant(10);
        str_dummy = nix::Variant("test");
        property = section.createProperty("prop", int_dummy);
        property_other = section.createProperty("other", int_dummy);
        property_null = nix::none;
    }


    void tearDown() {
        file.close();
    }

};

#endif //NIX_TESTPROPERTYHDF5_HPP
