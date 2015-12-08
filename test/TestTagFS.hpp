// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TESTTAGFS_HPP
#define NIX_TESTTAGFS_HPP

#include "BaseTestTag.hpp"

class TestTagFS : public BaseTestTag {

    CPPUNIT_TEST_SUITE(TestTagFS);
    CPPUNIT_TEST(testValidate);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testType);
    CPPUNIT_TEST(testDefinition);
    CPPUNIT_TEST(testCreateRemove);
    // CPPUNIT_TEST(testExtent);
    // CPPUNIT_TEST(testPosition);
    // CPPUNIT_TEST(testDataAccess);
    CPPUNIT_TEST(testMetadataAccess);
    CPPUNIT_TEST(testSourceAccess);
    CPPUNIT_TEST(testUnits);
    CPPUNIT_TEST(testReferences);
    CPPUNIT_TEST(testFeatures);
    CPPUNIT_TEST(testCreatedAt);
    CPPUNIT_TEST(testUpdatedAt);
    CPPUNIT_TEST(testOperators);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        startup_time = time(NULL);
        file = nix::File::open("test_multiTag", nix::FileMode::Overwrite, "file");
        block = file.createBlock("block", "dataset");

        std::vector<std::string> array_names = { "data_array_a", "data_array_b", "data_array_c",
                                                 "data_array_d", "data_array_e" };
        refs.clear();
        for (const auto & name : array_names) {
            refs.push_back(block.createDataArray(name, "reference",
                                                 nix::DataType::Double, nix::NDSize({ 0 })));
        }

        tag = block.createTag("tag_one", "test_tag", {0.0, 2.0, 3.4});
        tag_other = block.createTag("tag_two", "test_tag", {0.0, 2.0, 3.4});
        tag_null = nix::none;

        section = file.createSection("foo_section", "metadata");
    }


    void tearDown() {
        file.deleteBlock(block.id());
        file.deleteSection(section.id());
        file.close();
    }

};

#endif //NIX_TESTTAGFS_HPP
