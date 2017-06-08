// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASETESTENTITYWITHSOURCES_HPP
#define NIX_BASETESTENTITYWITHSOURCES_HPP

#include <nix.hpp>

#include <cppunit/TestFixture.h>


class BaseTestEntityWithSources : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file;
    nix::Block block;

public:
    void testSourceAccess();
    void testSourceVectorSetter();
    void testSourceOrder();

};

#endif //NIX_BASETESTENTITYWITHSOURCES_HPP
