// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestImplContainer.hpp"

#include <nix/util/util.hpp>
#include <ctime>

using namespace std;
using namespace nix;

void TestImplContainer::setUp() {
    startup_time = time(NULL);
    file = File::open("test_implcontainer.h5", FileMode::Overwrite);

    section = file.createSection("foo_section", "metadata");
}


void TestImplContainer::tearDown() {
    file.close();
}


void TestImplContainer::testBool() {
    CPPUNIT_ASSERT(section);
    CPPUNIT_ASSERT(!empty_section);
}


void TestImplContainer::testNone() {
    CPPUNIT_ASSERT(section != none);
    CPPUNIT_ASSERT(empty_section == none);
}


void TestImplContainer::testisNone() {
    CPPUNIT_ASSERT(!(section.isNone()));
    CPPUNIT_ASSERT(empty_section.isNone());
}

void TestImplContainer::testAccess() {
    CPPUNIT_ASSERT(section.id().size());
    CPPUNIT_ASSERT_THROW(empty_section.id(), UninitializedEntity);
}
