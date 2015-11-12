// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TESTREADONLYHDF5_HPP
#define NIX_TESTREADONLYHDF5_HPP

#include "BaseTestReadOnly.hpp"

class TestReadOnlyHDF5 : public BaseTestReadOnly {

    CPPUNIT_TEST_SUITE(TestReadOnlyHDF5);
    CPPUNIT_TEST(testRead);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        nix::File file = nix::File::open("test_read_only.h5", nix::FileMode::Overwrite);
        init(file);
    }

    void tearDown() {}

};

#endif //NIX_TESTREADONLYHDF5_HPP
