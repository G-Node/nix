// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASETESTTAG_HPP
#define NIX_BASETESTTAG_HPP


#include <nix.hpp>

#include <cppunit/TestFixture.h>


class BaseTestTag : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file;
    nix::Block block;
    nix::DataArray positions, extents;
    nix::Tag tag, tag_other, tag_null;
    nix::Section section;
    time_t startup_time;
    std::vector<nix::DataArray> refs;

public:
    void testValidate();

    void testId();
    void testName();
    void testType();
    void testDefinition();
    void testCreateRemove();
    void testExtent();
    void testPosition();
    void testDataAccess();
    void testMetadataAccess();
    void testSourceAccess();
    void testUnits();
    void testReferences();
    void testFeatures();
    void testOperators();
    void testCreatedAt();
    void testUpdatedAt();
};

#endif // NIX_BASETESTTAG_HPP
