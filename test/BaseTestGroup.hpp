// Copyright (c) 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix.hpp>

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#ifndef NIX_BASETESTGROUP_HPP
#define NIX_BASETESTGROUP_HPP

class BaseTestGroup : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file;
    nix::Group g, g2;
    nix::Block block;
    nix::DataArray array1, array2, array3;
    time_t startup_time;
    std::vector<nix::DataArray> arrays;
    std::vector<nix::Tag> tags;

public:
    void testId();
    void testType();
    void testName();
    void testDefinition();
    void testOperators();

    void testDataArrays();
    void testTags();
    // void testMultiTags();
};


#endif // NIX_BASETESTGROUP_HPP
