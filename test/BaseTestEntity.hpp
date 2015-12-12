// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASETESTENTITY_HPP
#define NIX_BASETESTENTITY_HPP

#include <nix.hpp>

#include <cppunit/TestFixture.h>


class BaseTestEntity : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file;
    nix::Block block;
    time_t startup_time;

public:
    void testId();
    void testType();
    void testName();
    void testDefinition();
    void testUpdatedAt();
    void testCreatedAt();
    void testIsValidEntity();

};

#endif // NIX_BASETESTENTITY_HPP
