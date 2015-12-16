// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASETESTENTITYWITHMETADATA_HPP
#define NIX_BASETESTENTITYWITHMETADATA_HPP

#include <nix.hpp>

#include <cppunit/TestFixture.h>


class BaseTestEntityWithMetadata : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file;
    nix::Section section, wrong;
    nix::Block block;

public:
    void testMetadataAccess();

};

#endif //NIX_BASETESTENTITYWITHMETADATA_HPP
