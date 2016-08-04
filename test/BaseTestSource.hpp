// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASETESTSOURCE_HPP
#define NIX_BASETESTSOURCE_HPP

#include <nix/hydra/multiArray.hpp>
#include <nix.hpp>

#include <cppunit/TestFixture.h>

class BaseTestSource : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file;
    nix::Block block;
    nix::Section section;
    nix::Source source, source_other, source_null;
    nix::DataArray darray;
    nix::MultiTag mtag;
    time_t startup_time;

public:
    void testValidate();
    void testId();
    void testType();
    void testName();
    void testDefinition();

    void testMetadataAccess();
    void testSourceAccess();
    void testFindSource();
    void testReferringDataArrays();
    void testReferringTags();

    void testOperators();
    void testUpdatedAt();
    void testCreatedAt();
};

#endif // NIX_BASETESTSOURCE_HPP
