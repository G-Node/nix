// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include "TestUtil.hpp"

using namespace std;
using namespace nix;


void TestUtil::testUnitScaling(){
	CPPUNIT_ASSERT_THROW(util::getScaling("mOhm","ms"), std::runtime_error);
	CPPUNIT_ASSERT(util::getScaling("mV","kV") == 1e+6);
	CPPUNIT_ASSERT(util::getScaling("V","kV") == 1e+03);
	CPPUNIT_ASSERT(util::getScaling("kV","V") == 1e-03);
}




