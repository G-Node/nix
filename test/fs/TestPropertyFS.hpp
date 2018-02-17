// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
#ifndef NIX_TESTPROPERTYFS_HPP
#define NIX_TESTPROPERTYFS_HPP

#include "BaseTestProperty.hpp"

class TestPropertyFS : public BaseTestProperty {

    CPPUNIT_TEST_SUITE(TestPropertyFS);

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

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {
        startup_time = time(NULL);
        file = nix::File::open("test_property", nix::FileMode::Overwrite, "file");
        section = file.createSection("cool section", "metadata");
        int_dummy = nix::Value(10);
        str_dummy = nix::Value("test");
        property = section.createProperty("prop", int_dummy);
        property_other = section.createProperty("other", int_dummy);
        property_null = nix::none;
    }


    void tearDown() {
        file.close();
    }

    void testValues() {
        // TODO Value is not implemented yet
    }

    void testValidate() {
        // TODO Value is not implemented yet
    }
};

#endif //NIX_TESTPROPERTYFS_HPP
