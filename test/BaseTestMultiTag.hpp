// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASETESTMULTITAG_HPP
#define NIX_BASETESTMULTITAG_HPP


#include <nix.hpp>

#include <cppunit/TestFixture.h>


class BaseTestMultiTag : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file;
    nix::Block block;
    nix::DataArray positions, extents, wrong_array;
    nix::MultiTag tag, tag_other, tag_null;
    nix::Section section;
    time_t startup_time;

public:
    void testValidate();
    void testId();
    void testName();
    void testType();
    void testDefinition();
    void testCreateRemove();
    void testExtents();
    void testUnits();
    void testPositions();
    void testReferences();
    void testFeatures();
    void testDataAccess();
    void testPositionExtents();
    void testMetadataAccess();
    void testSourceAccess();
    void testOperators();
    void testCreatedAt();
    void testUpdatedAt();
};

#endif // NIX_BASETESTMULTITAG_HPP
