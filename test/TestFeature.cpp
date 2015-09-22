// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestFeature.hpp"

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>

#include <sstream>
#include <ctime>

using namespace std;
using namespace nix;
using namespace valid;


void TestFeature::setUp() {
    file = File::open("test_feature.h5", FileMode::Overwrite);
    block = file.createBlock("featureTest","test");
    data_array = block.createDataArray("featureTest", "Test",
                                       DataType::Double, nix::NDSize({ 0 }));
    tag = block.createTag("featureTest", "Test", {0.0, 2.0, 3.4});

    file_fs = File::open("test_feature", FileMode::Overwrite, Implementation::FileSys);
    block_fs = file_fs.createBlock("featureTest", "test");
    data_array_fs = block_fs.createDataArray("featureTest", "Test",
                                             DataType::Double, nix::NDSize({ 0 }));
    tag_fs = block_fs.createTag("featureTest", "Test", {0.0, 2.0, 3.4});
}


void TestFeature::tearDown() {
    file.deleteBlock(block.id());
    file.close();
}


void TestFeature::testValidate() {
    Feature rp = tag.createFeature(data_array, nix::LinkType::Tagged);
    
    valid::Result result = validate(rp);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void TestFeature::testId() {
    Feature rp = tag.createFeature(data_array, nix::LinkType::Tagged);
    CPPUNIT_ASSERT(rp.id().size() == 36);
    tag.deleteFeature(rp.id());
}


void TestFeature::testLinkType(){
    test_link_type(tag, data_array);
    test_link_type(tag_fs, data_array_fs);
}

void TestFeature::test_link_type(Tag &t, DataArray &da) {
    Feature rp = t.createFeature(da, nix::LinkType::Tagged);
    CPPUNIT_ASSERT(rp.linkType() == nix::LinkType::Tagged);

    rp.linkType(nix::LinkType::Untagged);
    CPPUNIT_ASSERT(rp.linkType() == nix::LinkType::Untagged);

    rp.linkType(nix::LinkType::Tagged);
    CPPUNIT_ASSERT(rp.linkType() == nix::LinkType::Tagged);

    rp.linkType(nix::LinkType::Indexed);
    CPPUNIT_ASSERT(rp.linkType() == nix::LinkType::Indexed);

    t.deleteFeature(rp.id());
}


void TestFeature::testData() {
    test_data(block, tag, data_array);
    test_data(block_fs, tag_fs, data_array_fs);
}

void TestFeature::test_data(Block &b, Tag &t, DataArray &da) {
    DataArray a;
    Feature f;
    CPPUNIT_ASSERT_THROW(t.createFeature(a, nix::LinkType::Tagged), UninitializedEntity);
    CPPUNIT_ASSERT_THROW(f.data(a), UninitializedEntity);
    Feature rp = t.createFeature(da, nix::LinkType::Tagged);
    DataArray da_2 = b.createDataArray("array2", "Test",
                                       DataType::Double, nix::NDSize({ 0 }));
    CPPUNIT_ASSERT(rp.data().id() == da.id());
    rp.data(da_2);
    CPPUNIT_ASSERT(rp.data().id() == da_2.id());
    b.deleteDataArray(da_2.id());
    // make sure link is gone with deleted data array
    CPPUNIT_ASSERT(rp.data() == nullptr);

    CPPUNIT_ASSERT_THROW(rp.data(""), EmptyString);
    CPPUNIT_ASSERT_THROW(rp.data("worng_id"), runtime_error);
    t.deleteFeature(rp.id());
}


void TestFeature::testLinkType2Str() {
    CPPUNIT_ASSERT(link_type_to_string(nix::LinkType::Tagged) == "Tagged");
    CPPUNIT_ASSERT(link_type_to_string(nix::LinkType::Untagged) == "Untagged");
    CPPUNIT_ASSERT(link_type_to_string(nix::LinkType::Indexed) == "Indexed");
}


void TestFeature::testStreamOperator() {
    stringstream s1, s2, s3;
    s1 << nix::LinkType::Indexed;
    CPPUNIT_ASSERT(s1.str() == "LinkType::Indexed");
    s2 << nix::LinkType::Tagged;
    CPPUNIT_ASSERT(s2.str() == "LinkType::Tagged");
    s3 << nix::LinkType::Untagged;
    CPPUNIT_ASSERT(s3.str() == "LinkType::Untagged");
}


void TestFeature::testOperator() {
    test_operator(tag, data_array);
    test_operator(tag_fs, data_array_fs);
}

void TestFeature::test_operator(Tag &t, DataArray &da) {
    Feature rp = t.createFeature(da, nix::LinkType::Tagged);

    CPPUNIT_ASSERT(rp != none);
    rp = none;
    CPPUNIT_ASSERT(rp == false);
    CPPUNIT_ASSERT(rp == none);
}
