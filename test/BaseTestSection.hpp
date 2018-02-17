// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASETESTSECTION_HPP
#define NIX_BASETESTSECTION_HPP

#include <nix.hpp>

#include <cppunit/TestFixture.h>


class BaseTestSection : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file;
    nix::Section section, section_other, section_null;
    time_t startup_time;

public:
    void testValidate();
    void testId();
    void testType();
    void testName();
    void testDefinition();

    void testParent();
    void testRepository();
    void testLink();
    void testSectionAccess();
    void testFindSection();
    void testFindRelated();
    void testPropertyAccess();
    void testReferringData();
    void testReferringTags();
    void testReferringMultiTags();
    void testReferringSources();
    void testReferringBlocks();

    void testOperators();
    void testUpdatedAt();
    void testCreatedAt();

};

#endif // NIX_BASETESTSECTION_HPP
