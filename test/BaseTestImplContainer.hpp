// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASETESTIMPLCONTAINER_HPP
#define NIX_BASETESTIMPLCONTAINER_HPP

#include <nix.hpp>

#include <cppunit/TestFixture.h>


class BaseTestImplContainer : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file;
    nix::Section section, empty_section;
    time_t startup_time;

public:
    void testBool();
    void testNone();
    void testisNone();
    void testAccess();
};

#endif // NIX_BASETESTIMPLCONTAINER_HPP
