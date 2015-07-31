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

void TestAttributesFS::testHasField() {
    file::AttributesFS attrs(this->location.string());
    CPPUNIT_ASSERT(!attrs.has("format"));
    attrs.set("format", "nix");
    CPPUNIT_ASSERT(attrs.has("format"));
}

void TestAttributesFS::testWriteField() {
    file::AttributesFS attrs(this->location.string());
    attrs.set("format", "nix");
    CPPUNIT_ASSERT(attrs.attributeCount() == 1);
    attrs.set("created_at", "2015-01-01");
    CPPUNIT_ASSERT(attrs.attributeCount() == 2);
    attrs.remove("created_at");
    CPPUNIT_ASSERT(attrs.attributeCount() == 1);
}

void TestAttributesFS::testReadField() {
    string string_field = "test_string";
    string string_value = "hallo welt";
    string string_return;

    string float_field = "float_field";
    float float_value = 3.14;
    float float_return;

    string vector_field = "vector_field";
    vector<int> vector_values{1, 2, 3, 4, 5};
    vector<int> vector_return;

    file::AttributesFS attrs(this->location.string());
    attrs.set(string_field, string_value);
    attrs.get(string_field, string_return);
    CPPUNIT_ASSERT(string_value == string_return);

    attrs.set(float_field, float_value);
    attrs.get(float_field, float_return);
    CPPUNIT_ASSERT(float_value == float_return);

    attrs.set(vector_field, vector_values);
    attrs.get(vector_field, vector_return);
    CPPUNIT_ASSERT(vector_values == vector_return);
}
