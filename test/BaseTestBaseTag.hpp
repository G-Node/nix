// Copyright © 2015 Adrian Stoewer <adrian.stoewer@rz.ifi.lmu.de>
//                  Jan Grewe <jan.grewe@g-node.org>
//                  German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASETESTBASETAG_H
#define NIX_BASETESTBASETAG_H

#include <nix.hpp>

#include <cppunit/TestFixture.h>


class BaseTestBaseTag : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file;
    nix::Block block;
    std::vector<nix::DataArray> refs;

public:
    void testReferences();
    void testFeatures();
};

#endif //NIX_BASETESTBASETAG_H
