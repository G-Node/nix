// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in Section and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include <nix/util/util.hpp>
#include "TestDimension.hpp"

using namespace std;
using namespace nix;


void TestDimension::setUp() {
    file = File::open("test_dimension.h5", FileMode::Overwrite);
    Block b = file.createBlock("dimensionTest","test");
    data_array = b.createDataArray("dimensionTest", "Test");
}


void TestDimension::tearDown() {
    file.close();
}


void TestDimension::testId() {
	//TODO
}


void TestDimension::testSampledDimLabel(){
	//TODO
}


void TestDimension::testSampledDimUnit() {
    //TODO
}


void TestDimension::testSampledDimSamplingInterval() {
    //TODO
}


void TestDimension::testSampledDimOffset() {
    //TODO
}


void TestDimension::testSampledDimOperators() {
    //TODO
}

