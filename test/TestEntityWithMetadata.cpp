// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestEntityWithMetadata.hpp"

void TestEntityWithMetadata::setUp() {
    f1 = new File("test_block.h5", FileMode::ReadWrite);
}

void TestEntityWithMetadata::tearDown() {
    delete f1;
}


void TestEntityWithMetadata::testMetadata() {
    Block b = f1->createBlock("MetadataTest","Test");
    Section s1(f1->createSection("TestSection", "Test"));
    CPPUNIT_ASSERT(!b.hasMetadata());

    CPPUNIT_ASSERT_THROW(b.metadata(),std::runtime_error);

    b.metadata(s1);
    CPPUNIT_ASSERT(b.hasMetadata());

    Section s2 = b.metadata();
    CPPUNIT_ASSERT(s2 == s1);

    b.removeMetadata();
    CPPUNIT_ASSERT(!b.hasMetadata());

    f1->removeSection(s1.id());
    f1->removeBlock(b.id());
}
