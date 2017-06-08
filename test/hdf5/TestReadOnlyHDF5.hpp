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

protected:
    nix::File openFile(const std::string &name, nix::FileMode mode) override {
        return nix::File::open(name + ".h5", mode);
    }

};

#endif //NIX_TESTREADONLYHDF5_HPP
