// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASETESTFEATUREH_HPP
#define NIX_BASETESTFEATUREH_HPP

#include <nix.hpp>

#include <cppunit/TestFixture.h>


class BaseTestFeature : public CPPUNIT_NS::TestFixture {
protected:
    nix::File file;
    nix::DataArray data_array;
    nix::DataFrame data_frame;
    nix::Block block;
    nix::Tag tag;

public:
    void testValidate();
    void testId();

    void testLinkType();
    void testTargetType();
    void testDataArrayFeature();
    void testDataFrameFeature();
    void testLinkType2Str();
    void testStreamOperator();
    void testOperator();

};

#endif // NIX_BASETESTFEATUREH_HPP
