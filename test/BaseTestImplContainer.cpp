// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>
#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>

#include <nix/util/util.hpp>
#include "BaseTestImplContainer.hpp"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

using namespace std;
using namespace nix;


void BaseTestImplContainer::testBool() {
    CPPUNIT_ASSERT(section);
    CPPUNIT_ASSERT(!empty_section);
}


void BaseTestImplContainer::testNone() {
    CPPUNIT_ASSERT(section != none);
    CPPUNIT_ASSERT(empty_section == none);
}


void BaseTestImplContainer::testisNone() {
    CPPUNIT_ASSERT(!(section.isNone()));
    CPPUNIT_ASSERT(empty_section.isNone());
}

void BaseTestImplContainer::testAccess() {
    CPPUNIT_ASSERT(section.id().size());
    CPPUNIT_ASSERT_THROW(empty_section.id(), UninitializedEntity);
}
