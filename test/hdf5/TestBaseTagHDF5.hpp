// Copyright © 2015 Adrian Stoewer <adrian.stoewer@rz.ifi.lmu.de>
//                  Jan Grewe <jan.grewe@g-node.org>
//                  German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TESTBASETAGHDF5_H
#define NIX_TESTBASETAGHDF5_H

#include "BaseTestBaseTag.hpp"

class TestBaseTagHDF5 : public BaseTestBaseTag {

    CPPUNIT_TEST_SUITE(TestBaseTagHDF5);
    CPPUNIT_TEST(testReferences);
    CPPUNIT_TEST(testFeatures);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        file = nix::File::open("test_multiTag.h5", nix::FileMode::Overwrite);
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

#endif //NIX_TESTBASETAGHDF5_H
