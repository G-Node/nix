// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASETESTDIMENSION_HPP
#define NIX_BASETESTDIMENSION_HPP

#include <nix.hpp>

#include <cppunit/TestFixture.h>

class BaseTestDimension : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file;
    nix::DataArray data_array;
    nix::Block block;

public:
    void testValidate();
    void testSetValidate();
    void testSampleValidate();
    void testRangeValidate();
    
    void testIndex();

    void testSampledDimLabel();
    void testSampledDimOffset();
    void testSampledDimUnit();
    void testSampledDimSamplingInterval();
    void testSampledDimOperators();
    void testSampledDimIndexOf();
    void testSampledDimPositionAt();
    void testSampledDimAxis();

    void testSetDimLabels();

    void testRangeDimLabel();
    void testRangeTicks();
    void testRangeDimUnit();
    void testRangeDimIndexOfOld();
    void testRangeDimIndexOf();
    void testRangeDimTickAt();
    void testRangeDimAxis();
    void testRangeDimPositionInRange();
    
    void testAsDimensionMethods();
};

#endif //NIX_BASETESTDIMENSION_HPP
