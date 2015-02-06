// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestEntityWithSources.hpp"

#include <nix/util/util.hpp>

#include <ctime>

using namespace std;
using namespace nix;

void TestEntityWithSources::setUp() {
    file = File::open("test_block.h5", FileMode::Overwrite);

    block = file.createBlock("block_one", "dataset");
}


void TestEntityWithSources::tearDown() {
    file.close();
}


void TestEntityWithSources::testSourceAccess() {
    vector<string> names = { "source_a", "source_b", "source_c", "source_d", "source_e" };

    CPPUNIT_ASSERT(block.sourceCount() == 0);
    CPPUNIT_ASSERT(block.sources().size() == 0);

    vector<string> ids;
    for (auto it = names.begin(); it != names.end(); it++) {
        Source src = block.createSource(*it, "channel");
        CPPUNIT_ASSERT(src.name() == *it);

        ids.push_back(src.id());
    }

    std::string name = names[0];
    Source source = block.getSource(name);
    CPPUNIT_ASSERT(source.name() == name);

    CPPUNIT_ASSERT(block.sourceCount() == names.size());
    CPPUNIT_ASSERT(block.sources().size() == names.size());


    for (auto it = ids.begin(); it != ids.end(); it++) {
        Source src = block.getSource(*it);
        CPPUNIT_ASSERT(block.hasSource(*it) == true);
        CPPUNIT_ASSERT(src.id() == *it);

        block.deleteSource(*it);
    }

    CPPUNIT_ASSERT(block.sourceCount() == 0);
    CPPUNIT_ASSERT(block.sources().size() == 0);
}


