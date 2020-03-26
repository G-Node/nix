// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#ifndef NIX_BASETESTDATAARRAY_HPP
#define NIX_BASETESTDATAARRAY_HPP

#include <nix.hpp>

#include <cppunit/TestFixture.h>


class BaseTestDataArray : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file;
    nix::Block block;
    nix::DataArray array1, array2, array3;
    time_t startup_time;

public:
    void testId();
    void testType();
    void testName();
    void testDefinition();
    void testData();
    void testPolynomial();
    void testPolynomialSetter();
    void testLabel();
    void testUnit();
    void testDimension();
    void testAliasRangeDimension();
    void testDataFrameDimension();
    void testOperator();
    void testValidate();
};

#endif // NIX_BASETESTDATAARRAY_HPP
