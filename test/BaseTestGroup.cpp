// Copyright © 2015 German Neuroinformatics Node (G-Node)
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

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>

#include "BaseTestGroup.hpp"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

using namespace nix;
using namespace valid;


void BaseTestGroup::testId() {
    CPPUNIT_ASSERT(g.id().size() == 36);
}


void BaseTestGroup::testName() {
    CPPUNIT_ASSERT(g.name() == "group one");
}


void BaseTestGroup::testDefinition() {
    std::string def = nix::util::createId();
    g.definition(def);
    CPPUNIT_ASSERT(*g.definition() == def);
    g.definition(boost::none);
    CPPUNIT_ASSERT(g.definition() == nix::none);
}


void BaseTestGroup::testType() {
    CPPUNIT_ASSERT(g.type() == "group");
    std::string typ = nix::util::createId();
    g.type(typ);
    CPPUNIT_ASSERT(g.type() == typ);
}


void BaseTestGroup::testOperators() {
    std::stringstream mystream;
    mystream << g;

    g = none;
    CPPUNIT_ASSERT(g == false);
    CPPUNIT_ASSERT(g == none);
}


void BaseTestGroup::testDataArrays() {

}