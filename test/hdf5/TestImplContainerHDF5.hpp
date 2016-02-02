// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TESTIMPLCONTAINERHDF5_HPP
#define NIX_TESTIMPLCONTAINERHDF5_HPP

#include "BaseTestImplContainer.hpp"

class TestImplContainerHDF5 : public BaseTestImplContainer {

    CPPUNIT_TEST_SUITE(TestImplContainerHDF5);
    CPPUNIT_TEST(testBool);
    CPPUNIT_TEST(testNone);
    CPPUNIT_TEST(testisNone);
    CPPUNIT_TEST(testAccess);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp() {
        startup_time = time(NULL);
        file = nix::File::open("test_implcontainer.h5", nix::FileMode::Overwrite);

        section = file.createSection("foo_section", "metadata");
    }

    void tearDown() {
        file.close();
    }

};


#endif //NIX_TESTIMPLCONTAINERHDF5_HPP
