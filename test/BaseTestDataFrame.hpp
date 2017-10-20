// Copyright © 2017 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <ckellner@redhat.com

#ifndef NIX_BASETESTDATAFRAME_HPP
#define NIX_BASETESTDATAFRAME_HPP

#include <nix.hpp>

#include <cppunit/TestFixture.h>


class BaseTestDataFrame : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file;
    nix::Block block;
    time_t startup_time;

public:
    void testBasic();
    void testRowIO();
    void testColIO();
    void testCellIO();
};

#endif // NIX_BASETESTDATAFRAME_HPP
