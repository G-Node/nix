// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TESTSECTIONHDF5_HPP
#define NIX_TESTSECTIONHDF5_HPP

#include "BaseTestSection.hpp"

class TestSectionHDF5 : public BaseTestSection {

    CPPUNIT_TEST_SUITE(TestSectionHDF5);

    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testType);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testDefinition);

    CPPUNIT_TEST(testParent);
    CPPUNIT_TEST(testRepository);
    CPPUNIT_TEST(testLink);
    CPPUNIT_TEST(testMapping);
    CPPUNIT_TEST(testSectionAccess);
    CPPUNIT_TEST(testFindSection);
    CPPUNIT_TEST(testFindRelated);
    CPPUNIT_TEST(testPropertyAccess);
    CPPUNIT_TEST(testReferringData);

    CPPUNIT_TEST(testOperators);
    CPPUNIT_TEST(testUpdatedAt);
    CPPUNIT_TEST(testCreatedAt);

    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        startup_time = time(NULL);
        file = nix::File::open("test_section.h5", nix::FileMode::Overwrite);

        section = file.createSection("section", "metadata");
        section_other = file.createSection("other_section", "metadata");
        section_null  = nullptr;
    }

    void tearDown() {
        file.close();
    }
};


#endif //NIX_TESTSECTIONHDF5_HPP
