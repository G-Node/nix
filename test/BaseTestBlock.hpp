// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASETESTBLOCK_HPP
#define NIX_BASETESTBLOCK_HPP

#include <nix.hpp>

#include <cppunit/TestFixture.h>


class BaseTestBlock : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file;
    nix::Section section;
    nix::Block block, block_other, block_null;
    time_t startup_time;

public:
    void testValidate();
    void testId();
    void testType();
    void testName();
    void testDefinition();

    void testMetadataAccess();
    void testSourceAccess();
    void testDataArrayAccess();
    void testTagAccess();
    void testMultiTagAccess();
    void testGroupAccess();

    void testOperators();
    void testUpdatedAt();
    void testCreatedAt();

    void testCompare();
};

#endif // NIX_BASETESTBLOCK_HPP
