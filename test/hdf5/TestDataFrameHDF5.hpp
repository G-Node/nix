// Copyright © 2017 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <ckellner@redhat.com>

#ifndef NIX_TESTDATAFRAMEHDF5_HPP
#define NIX_TESTDATAFRAMEHDF5_HPP

#include "BaseTestDataFrame.hpp"

class TestDataFrameHDF5 : public BaseTestDataFrame {

    CPPUNIT_TEST_SUITE(TestDataFrameHDF5);
    CPPUNIT_TEST(testBasic);
    CPPUNIT_TEST(testRowIO);
    CPPUNIT_TEST(testColIO);
    CPPUNIT_TEST(testCellIO);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        startup_time = time(NULL);
        file = nix::File::open("test_DataFrame.h5", nix::FileMode::Overwrite);
        block = file.createBlock("b1", "dataset");
    }

    void tearDown() {
        file.close();
    }

};

#endif //NIX_TESTDATAFRAMEHDF5_HPP
