// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#include <sstream>
#include <ctime>
#include <iostream>
#include <iterator>
#include <stdexcept>

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>

#include "BaseTestFeature.hpp"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <boost/math/constants/constants.hpp>

using namespace nix;
using namespace valid;


void BaseTestFeature::testValidate() {
    Feature rp = tag.createFeature(data_array, nix::LinkType::Tagged);
    
    valid::Result result = validate(rp);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
}


void BaseTestFeature::testId() {
    Feature rp = tag.createFeature(data_array, nix::LinkType::Tagged);
    CPPUNIT_ASSERT(rp.id().size() == 36);
    tag.deleteFeature(rp.id());
}


void BaseTestFeature::testLinkType(){
    Feature rp = tag.createFeature(data_array, nix::LinkType::Tagged);
    CPPUNIT_ASSERT(rp.linkType() == nix::LinkType::Tagged);

    rp.linkType(nix::LinkType::Untagged);
    CPPUNIT_ASSERT(rp.linkType() == nix::LinkType::Untagged);

    rp.linkType(nix::LinkType::Tagged);
    CPPUNIT_ASSERT(rp.linkType() == nix::LinkType::Tagged);

    rp.linkType(nix::LinkType::Indexed);
    CPPUNIT_ASSERT(rp.linkType() == nix::LinkType::Indexed);

    tag.deleteFeature(rp.id());
}


void BaseTestFeature::testTargetType(){
    CPPUNIT_ASSERT(target_type_to_string(nix::TargetType::DataArray) == "DataArray");
    CPPUNIT_ASSERT(target_type_to_string(nix::TargetType::DataFrame) == "DataFrame");
    
    Feature rp = tag.createFeature(data_array, nix::LinkType::Tagged);
    CPPUNIT_ASSERT(rp.targetType() == nix::TargetType::DataArray); 
    tag.deleteFeature(rp.id());

    Feature ft = tag.createFeature(data_frame, nix::LinkType::Untagged);
    CPPUNIT_ASSERT(ft.targetType() == nix::TargetType::DataFrame);
    tag.deleteFeature(ft.id());
}


void BaseTestFeature::testDataArrayFeature() {
    DataArray a;
    Feature f;
    CPPUNIT_ASSERT_THROW(tag.createFeature(a, nix::LinkType::Tagged), UninitializedEntity);
    CPPUNIT_ASSERT_THROW(f.data(a), UninitializedEntity);
    a = block.createDataArray("Test", "array", DataType::Double, {0, 0});
    f = tag.createFeature(a, nix::LinkType::Untagged);
    f.data(a.name());
    block.deleteDataArray(a);
    CPPUNIT_ASSERT_THROW(f.data(a), UninitializedEntity);

    Feature rp = tag.createFeature(data_array, nix::LinkType::Tagged);
    tag.hasFeature(rp.id());
    tag.getFeature(rp.id());
    Feature ft2 = tag.getFeature(data_array.name());
    CPPUNIT_ASSERT(ft2 && ft2.dataArray().name() == data_array.name());
    CPPUNIT_ASSERT(ft2 && ft2.id() == rp.id());

    DataArray da_2 = block.createDataArray("array2", "Test",
                                           DataType::Double, nix::NDSize({ 0 }));
    CPPUNIT_ASSERT(rp.dataArray().id() == data_array.id());
    CPPUNIT_ASSERT(rp.data().id() == data_array.id());
    rp.data(da_2);
    CPPUNIT_ASSERT(rp.dataArray().id() == da_2.id());
    CPPUNIT_ASSERT_THROW(rp.dataFrame(), std::runtime_error);
    block.deleteDataArray(da_2.id());
    // make sure link is gone with deleted data array
    CPPUNIT_ASSERT(rp.dataArray() == nix::none);
    CPPUNIT_ASSERT_THROW(rp.data(""), EmptyString);
    CPPUNIT_ASSERT_THROW(rp.data("worng_id"), std::runtime_error);
    tag.deleteFeature(rp.id());
}


void BaseTestFeature::testDataFrameFeature() {
    DataFrame df;
    Feature ft;
    CPPUNIT_ASSERT_THROW(tag.createFeature(df, nix::LinkType::Tagged), UninitializedEntity);
    CPPUNIT_ASSERT_THROW(ft.data(df), UninitializedEntity);
    std::vector<nix::Column> cols = {
            {"bool", "", nix::DataType::Bool},
            {"int32", "V", nix::DataType::Int32}};
    df = block.createDataFrame("df", "test_df", cols);
    ft = tag.createFeature(df, nix::LinkType::Untagged);
    ft.data(df.name());
    block.deleteDataFrame(df);
    CPPUNIT_ASSERT_THROW(ft.data(df), UninitializedEntity);
    CPPUNIT_ASSERT(ft.dataFrame() == nix::none);
    CPPUNIT_ASSERT_THROW(ft.data(""), EmptyString);
    CPPUNIT_ASSERT_THROW(ft.data("worng_id"), std::runtime_error);
    CPPUNIT_ASSERT_THROW(ft.dataArray(), std::runtime_error);
    CPPUNIT_ASSERT_THROW(ft.data(), std::runtime_error);

    ft = tag.createFeature(data_frame, nix::LinkType::Indexed);

    DataFrame df2 = ft.dataFrame();
    Feature ft2 = tag.getFeature(data_frame.name());
    CPPUNIT_ASSERT(ft2 && ft2.dataFrame().name() == data_frame.name());
    CPPUNIT_ASSERT(ft2 && ft2.id() == ft.id());
    CPPUNIT_ASSERT(ft.targetType() == nix::TargetType::DataFrame && ft.linkType() == nix::LinkType::Indexed);
    tag.deleteFeature(ft.id());

    ft = tag.createFeature(data_frame, nix::LinkType::Untagged);
    CPPUNIT_ASSERT(ft.targetType() == nix::TargetType::DataFrame && ft.linkType() == nix::LinkType::Untagged);
    tag.deleteFeature(ft.id());
    
    CPPUNIT_ASSERT_THROW(tag.createFeature(data_frame, nix::LinkType::Tagged), InvalidLinkType);
}


void BaseTestFeature::testLinkType2Str() {
    CPPUNIT_ASSERT(link_type_to_string(nix::LinkType::Tagged) == "Tagged");
    CPPUNIT_ASSERT(link_type_to_string(nix::LinkType::Untagged) == "Untagged");
    CPPUNIT_ASSERT(link_type_to_string(nix::LinkType::Indexed) == "Indexed");
}


void BaseTestFeature::testStreamOperator() {
    std::stringstream s1, s2, s3;
    s1 << nix::LinkType::Indexed;
    CPPUNIT_ASSERT(s1.str() == "LinkType::Indexed");
    s2 << nix::LinkType::Tagged;
    CPPUNIT_ASSERT(s2.str() == "LinkType::Tagged");
    s3 << nix::LinkType::Untagged;
    CPPUNIT_ASSERT(s3.str() == "LinkType::Untagged");
}


void BaseTestFeature::testOperator() {
    Feature rp = tag.createFeature(data_array, nix::LinkType::Tagged);
    CPPUNIT_ASSERT(rp != none);
    rp = none;
    CPPUNIT_ASSERT(rp == false);
    CPPUNIT_ASSERT(rp == none);
}
