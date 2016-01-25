// Copyright (c) 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TESTGROUPHDF5_HPP
#define NIX_TESTGROUPHDF5_HPP

#include "BaseTestGroup.hpp"

class TestGroupHDF5 : public BaseTestGroup {

    CPPUNIT_TEST_SUITE(TestGroupHDF5);

    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testType);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testDefinition);
    CPPUNIT_TEST(testOperators);

    CPPUNIT_TEST(testDataArrays);
    CPPUNIT_TEST(testTags);
    CPPUNIT_TEST(testMultiTags);

    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        startup_time = time(NULL);
        file = nix::File::open("test_group.h5", nix::FileMode::Overwrite);
        block = file.createBlock("test_block", "group_test");
        g = block.createGroup("group one", "group");
        g2 = block.createGroup("group other", "group");
        positions_array = block.createDataArray("positions", "nix.events", nix::DataType::Double, nix::NDSize{0.0});
        std::vector<std::string> array_names = { "data_array_a", "data_array_b", "data_array_c",
                                                 "data_array_d", "data_array_e" };
        arrays.clear();
        for (const auto & name : array_names) {
            arrays.push_back(block.createDataArray(name, "reference",
                                                 nix::DataType::Double, nix::NDSize({ 0 })));
        }
        std::vector<std::string> tag_names = { "tag_a", "tag_b", "tag_c",
                                                 "tag_d", "tag_e" };
        tags.clear();
        for (const auto & name : tag_names) {
            tags.push_back(block.createTag(name, "tag", std::vector<double>{ 0.0 }));
        }

        std::vector<std::string> mtag_names = { "mtag_a", "mtag_b", "mtag_c",
                                               "mtag_d", "mtag_e" };
        mtags.clear();
        for (const auto & name : mtag_names) {
            mtags.push_back(block.createMultiTag(name, "mtag", positions_array));
        }
    }

    void tearDown() {
        file.close();
    }
};


#endif //NIX_TESTGROUPHDF5_HPP