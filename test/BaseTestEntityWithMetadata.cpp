// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>

#include <nix/util/util.hpp>
#include "BaseTestEntityWithMetadata.hpp"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

using namespace std;
using namespace nix;


void BaseTestEntityWithMetadata::testMetadataAccess() {
    CPPUNIT_ASSERT(!block.metadata());

    block.metadata(section);
    CPPUNIT_ASSERT(block.metadata());
    
    // test none-unsetter
    block.metadata(none);
    CPPUNIT_ASSERT(!block.metadata());
    // test deleter removing link too
    block.metadata(section);
    file.deleteSection(section.id());
    CPPUNIT_ASSERT(!block.metadata());
    // re-create section
    section = file.createSection("foo_section", "metadata");
}
