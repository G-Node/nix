// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestNamedEntityWithSources.hpp"

void TestNamedEntityWithSources::setUp() {
    f1 = new File("test_block.h5", FileMode::ReadWrite);
}

void TestNamedEntityWithSources::tearDown() {
    delete f1;
}


void TestNamedEntityWithSources::testAddRemoveSource() {

}

