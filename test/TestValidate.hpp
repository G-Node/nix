// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

// define some tag like class with units- & unit-getter that allows compound units
struct tag_tmp {
    std::vector<std::string> units_ref;
    
    tag_tmp() : units_ref(std::vector<std::string>()) {}
    tag_tmp(std::vector<std::string> units) : units_ref(units) {}
    
    std::vector<std::string> units() const { 
        return units_ref; 
    }
    
    std::string unit() const { 
        return units_ref.front(); 
    }
    
    boost::optional<std::string> unito() const { 
        boost::optional<std::string> ret = units_ref.front(); 
        return ret; 
    }
};
    
class TestValidate : public CPPUNIT_NS::TestFixture {

private:

    CPPUNIT_TEST_SUITE(TestValidate);

    CPPUNIT_TEST(test);

    CPPUNIT_TEST_SUITE_END ();

    time_t startup_time;
    
    nix::File file;
    nix::Block block;
    nix::DataArray array1;
    nix::DataArray array2;
    nix::DataArray array3;
    std::vector<nix::DataArray> refs;
    std::vector<double> extent, position;
    nix::DataArray positions;
    nix::DataArray extents;
    std::vector<std::string> atomic_units;
    std::vector<std::string> compound_units;
    std::vector<std::string> invalid_units;
    nix::MultiTag dtag;
    nix::Tag tag;
    tag_tmp units_tmp;
    nix::SetDimension dim_set1;
    nix::SetDimension dim_set2;
    nix::SetDimension dim_set3;
    nix::SampledDimension dim_sample1;
    nix::SampledDimension dim_sample2;
    nix::SampledDimension dim_sample3;
    nix::RangeDimension dim_range1;
    nix::RangeDimension dim_range2;
    nix::RangeDimension dim_range3;

    void setValid();    
    void setInvalid();
    
public:

    void setUp();
    void tearDown();

    void test();
};
