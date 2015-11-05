// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
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

using namespace std;
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


void BaseTestFeature::testData() {
    DataArray a;
    Feature f;
    CPPUNIT_ASSERT_THROW(tag.createFeature(a, nix::LinkType::Tagged), UninitializedEntity);
    CPPUNIT_ASSERT_THROW(f.data(a), UninitializedEntity);
    Feature rp = tag.createFeature(data_array, nix::LinkType::Tagged);
    DataArray da_2 = block.createDataArray("array2", "Test",
                                           DataType::Double, nix::NDSize({ 0 }));
    CPPUNIT_ASSERT(rp.data().id() == data_array.id());
    rp.data(da_2);
    CPPUNIT_ASSERT(rp.data().id() == da_2.id());
    block.deleteDataArray(da_2.id());
    // make sure link is gone with deleted data array
    CPPUNIT_ASSERT(rp.data() == nullptr);
    tag.deleteFeature(rp.id());
}


void BaseTestFeature::testLinkType2Str() {
    CPPUNIT_ASSERT(link_type_to_string(nix::LinkType::Tagged) == "Tagged");
    CPPUNIT_ASSERT(link_type_to_string(nix::LinkType::Untagged) == "Untagged");
    CPPUNIT_ASSERT(link_type_to_string(nix::LinkType::Indexed) == "Indexed");
}


void BaseTestFeature::testStreamOperator() {
    stringstream s1, s2, s3;
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
