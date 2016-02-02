// Copyright © 2015 Adrian Stoewer <adrian.stoewer@rz.ifi.lmu.de>
//                  Jan Grewe <jan.grewe@g-node.org>
//                  German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TESTBASETAGFS_HPP
#define NIX_TESTBASETAGFS_HPP

#include "BaseTestBaseTag.hpp"

class TestBaseTagFS : public BaseTestBaseTag {

    CPPUNIT_TEST_SUITE(TestBaseTagFS);
    CPPUNIT_TEST(testReferences);
    CPPUNIT_TEST(testFeatures);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        file = nix::File::open("test_multiTag", nix::FileMode::Overwrite, "file");
        block = file.createBlock("block", "dataset");

        std::vector<std::string> array_names = { "data_array_a", "data_array_b", "data_array_c",
                                                 "data_array_d", "data_array_e" };

        refs.clear();
        for (const auto & name : array_names) {
            refs.push_back(block.createDataArray(name, "reference",
                                                 nix::DataType::Double, nix::NDSize({ 0 })));
        }
    }

    void tearDown() {
        file.deleteBlock(block.id());
        file.close();
    }

};

#endif //NIX_TESTBASETAGFS_HPP
