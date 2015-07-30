// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestAttributesFS.hpp"

using namespace std;
using namespace nix;


void TestAttributesFS::setUp() {
    this->location = boost::filesystem::path("testFileSysAttributes");
    boost::filesystem::create_directories(this->location);
}


void TestAttributesFS::tearDown() {
    boost::filesystem::remove_all(this->location);
}


void TestAttributesFS::testOpenCreate() {
    boost::filesystem::path p = ".attributes";
    CPPUNIT_ASSERT(!boost::filesystem::exists(this->location / p));
    file::AttributesFS attrs(this->location.string());
    CPPUNIT_ASSERT(boost::filesystem::exists(this->location / p));
}
